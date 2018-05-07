//
// Created by apple on 16/7/27.
//
#include <stdlib.h>
#include <stdio.h>
#include "re/include/re.h"
#include "baresip/include/baresip.h"
#include <unistd.h>
#include <android/log.h>
#include <pthread.h>
#include <android/native_window_jni.h>


#define LOG_TAG "jni"
bool g_blog = true;
#define LOGD(...) if (g_blog) {__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);}

#define RETURN_ERR LOGD("return %d\n", err); \
        return err;

#define MID_NAME "handle"
#define SIG "(Lcom/xyt/sip/jni/Event;)V"
#define PARAM_CLZ_NAME "com/xyt/sip/jni/Event"

static JavaVM *g_jvm = NULL;
static bool g_is_remain = false;
static bool g_is_init = false;

static struct {
    struct {
        jclass jni_clz;     //java方法类的jclass对象
        jobject jni_obj;    //java方法类的object
        jmethodID method_id;//java回调方法
        jclass param_clz;   //java回调方法的参数类
    } event_cb;
    JavaVM *javaVM;
} g_jni_ctx;

static int stdout_handler(const char *p, size_t size, void *arg)
{
    char buf[4096];
    memcpy(buf, p, size);
    buf[size] = '\0';
    info(buf);
    return 0;
}

static struct re_printf g_rpf = {
    .vph = stdout_handler,
    .arg = NULL
};

/*
 * 生成java参数对象
 * c的struct call -> java的jobject
 */
jobject new_jobject(JNIEnv *env, enum ua_event ev, struct call *call) {
    jclass clz = g_jni_ctx.event_cb.param_clz;
    jobject obj = (*env)->AllocObject(env, clz);
    jfieldID fid;
    char *p;

    fid = (*env)->GetFieldID(env, clz, "event", "I");
    (*env)->SetIntField(env, obj, fid, (jint) ((int) ev));

    fid = (*env)->GetFieldID(env, clz, "start_time", "I");
    (*env)->SetIntField(env, obj, fid, (jint) ((int) get_time_start(call)));

    fid = (*env)->GetFieldID(env, clz, "stop_time", "I");
    (*env)->SetIntField(env, obj, fid, (jint) ((int) get_time_stop(call)));

    fid = (*env)->GetFieldID(env, clz, "conn_time", "I");
    (*env)->SetIntField(env, obj, fid, (jint) ((int) get_time_conn(call)));

    fid = (*env)->GetFieldID(env, clz, "status", "I");
    (*env)->SetIntField(env, obj, fid, (jint) ((int) get_call_status(call)));

    p = get_local_uri(call);
    if (p) {
        fid = (*env)->GetFieldID(env, clz, "local_url", "Ljava/lang/String;");
        jstring jstr = (*env)->NewStringUTF(env, p);
        (*env)->SetObjectField(env, obj, fid, jstr);
        (*env)->DeleteLocalRef(env, jstr);
    }

    p = get_peer_uri(call);
    if (p) {
        fid = (*env)->GetFieldID(env, clz, "peer_url", "Ljava/lang/String;");
        jstring jstr = (*env)->NewStringUTF(env, p);
        (*env)->SetObjectField(env, obj, fid, jstr);
        (*env)->DeleteLocalRef(env, jstr);
    }

    return obj;
}

void baresip_event(enum ua_event ev, struct call *call) {
    JNIEnv *env;
    JavaVM *javaVM = g_jni_ctx.javaVM;
    jint res = (*javaVM)->GetEnv(javaVM, (void **) &env, JNI_VERSION_1_6);

    jobject jobj = new_jobject(env, ev, call);
    (*env)->CallVoidMethod(env, g_jni_ctx.event_cb.jni_obj, g_jni_ctx.event_cb.method_id, jobj);

    jclass clz = g_jni_ctx.event_cb.param_clz;
    jfieldID fid = (*env)->GetFieldID(env, clz, "local_url", "Ljava/lang/String;");
    jobject member_obj = (*env)->GetObjectField(env, jobj, fid);
    (*env)->DeleteLocalRef(env, member_obj);

    fid = (*env)->GetFieldID(env, clz, "peer_url", "Ljava/lang/String;");
    member_obj = (*env)->GetObjectField(env, jobj, fid);
    (*env)->DeleteLocalRef(env, member_obj);

    (*env)->DeleteLocalRef(env, jobj);
}

static void ua_exit_handler(int sig) {
    re_cancel();
}

static void signal_handler(int sig) {
    static bool term = false;

    if (term) {
        mod_close();
        exit(0);
    }

    term = true;

    ua_stop_all(false);
}

static void init_callback(JNIEnv *env, jobject callback) {
    (*env)->GetJavaVM(env, &g_jni_ctx.javaVM);
    g_jni_ctx.event_cb.jni_obj = (*env)->NewGlobalRef(env, callback);
    g_jni_ctx.event_cb.jni_clz = (*env)->GetObjectClass(env, callback);
    g_jni_ctx.event_cb.method_id = (*env)->GetMethodID(env, g_jni_ctx.event_cb.jni_clz, MID_NAME, SIG);
    jclass param_clz = (*env)->FindClass(env, PARAM_CLZ_NAME);
    g_jni_ctx.event_cb.param_clz = (*env)->NewGlobalRef(env, param_clz);
}

JNIEXPORT jint JNICALL
Java_com_xyt_sip_jni_Jni_init(JNIEnv *env, jobject obj, jstring _configPath, jobject callback) {
    int err = 0;

    if (g_is_init) {
        LOGD("baresip is running");
        return err;
    }

    init_callback(env, callback);

    const char *configPath = (*env)->GetStringUTFChars(env, _configPath, 0);
    conf_path_set(configPath);
    LOGD("%s", configPath);
    (*env)->ReleaseStringUTFChars(env, _configPath, configPath);

    set_event(baresip_event);

    //保存全局JVM以便在回调函数中使用
    (*env)->GetJavaVM(env, &g_jvm);

    (void) sys_coredump_set(true);
    err = libre_init();
    if (err) {
        LOGD("libre_init error!");
        return err;
    }

    err = conf_configure();
    if (err) {
        LOGD("conf_configure return %d\n", err);
        return err;
    }

    err = baresip_init(conf_config(), false);
    if (err) {
        LOGD("baresip_init return %d\n", err);
        return err;
    }

    /* Initialise User Agents */
    err = ua_init("baresip v" BARESIP_VERSION " (" ARCH "/" OS ")",
                  true, true, true, 0);
    if (err) {
        return err;
    }

    uag_set_exit_handler(ua_exit_handler, NULL);


    err = conf_modules();
    if (err) {
        LOGD("conf_modules return %d\n", err);
        return err;
    }

    g_is_init = true;
    return err;
}


JNIEXPORT jint JNICALL
Java_com_xyt_sip_jni_Jni_close(JNIEnv *env, jobject obj) {
    ua_close();
    conf_close();

    baresip_close();

    libre_close();

    /* Check for memory leaks */
    tmr_debug();
    mem_debug();

    g_is_init = false;

    return 0;
}

JNIEXPORT jint JNICALL
Java_com_xyt_sip_jni_Jni_reg(JNIEnv *env, jobject obj, jstring _aor) {
    LOGD("Java_com_xyt_sip_jni_Jni_reg run");

    const char* aor = (*env)->GetStringUTFChars(env, _aor, 0);
    int err = ua_alloc(NULL, aor);
    (*env)->ReleaseStringUTFChars(env, _aor, aor);

    RETURN_ERR
}

JNIEXPORT jint JNICALL
Java_com_xyt_sip_jni_Jni_run(JNIEnv *env, jobject obj) {
    int err = 0;
    if (!g_is_remain) {
        g_is_remain = true;
        err = re_main(signal_handler);
    }

    g_is_remain = false;

    RETURN_ERR
}

JNIEXPORT jint JNICALL
Java_com_xyt_sip_jni_Jni_call(JNIEnv *env, jobject obj, jstring _number) {
    int err = 0;
    struct cmd_arg arg;

    const char* number = (*env)->GetStringUTFChars(env, _number, 0);

    arg.key = 'd';
    arg.complete = true;
    arg.prm = number;

    struct cmd *cmd = cmd_find_by_key(baresip_commands(), arg.key);

    if (cmd) {
        LOGD("jni call %s\n", arg.prm);
        err = cmd->h(&g_rpf, &arg);
        LOGD("jni call %s, return %d\n", arg.prm, err);
    } else {
        LOGD("no find cmd %c", arg.key);
        err = -1;
    }

    if (arg.prm) {
        free(arg.prm);
    }

    (*env)->ReleaseStringUTFChars(env, _number, number);
    RETURN_ERR
}

JNIEXPORT jint JNICALL
Java_com_xyt_sip_jni_Jni_answer(JNIEnv *env, jobject obj) {
    int err = 0;

    struct cmd_arg arg;
    arg.key = 'a';
    arg.complete = true;

    struct cmd *cmd = cmd_find_by_key(baresip_commands(), arg.key);

    if (cmd) {
        err = cmd->h(&g_rpf, &arg);
    } else {
        LOGD("no find cmd %c", arg.key);
        err = -1;
    }

    RETURN_ERR
}

JNIEXPORT jint JNICALL
Java_com_xyt_sip_jni_Jni_hangup(JNIEnv *env, jobject obj) {
    int err = 0;

    struct cmd_arg arg;
    arg.key = 'b';
    arg.complete = true;
    struct cmd *cmd = cmd_find_by_key(baresip_commands(), arg.key);

    if (cmd) {
        err = cmd->h(&g_rpf, &arg);
    } else {
        LOGD("no find cmd %c", arg.key);
        err = -1;
    }

    RETURN_ERR
}


JNIEXPORT void JNICALL
Java_com_xyt_sip_jni_Jni_unreg(JNIEnv *env, jobject obj, jstring _aor) {
    const char* aor = (*env)->GetStringUTFChars(env, _aor, 0);
    ua_account_unreg(aor);
    (*env)->ReleaseStringUTFChars(env, _aor, aor);
}


JNIEXPORT void JNICALL
Java_com_xyt_sip_jni_Jni_unreg_1all(JNIEnv *env, jobject obj) {
    LOGD("accounts unregister\n");
    ua_accounts_unreg();
}

JNIEXPORT jint JNICALL
Java_com_xyt_sip_jni_Jni_set_1current_1account(JNIEnv *env, jobject obj, jstring _aor) {
    const char* aor = (*env)->GetStringUTFChars(env, _aor, 0);
    ua_set_current_account(aor);
    (*env)->ReleaseStringUTFChars(env, _aor, aor);
}


JNIEXPORT void JNICALL
Java_com_xyt_sip_jni_Jni_set_1play_1path(JNIEnv *env, jobject obj, jstring _path) {
    const char* path = (*env)->GetStringUTFChars(env, _path, 0);
    play_set_path(baresip_player(), path);
    (*env)->ReleaseStringUTFChars(env, _path, path);
}

JNIEXPORT jstring JNICALL
Java_com_xyt_sip_jni_Jni_get_1play_1path(JNIEnv *env, jobject obj) {
    return (*env)->NewStringUTF(env, play_get_path(baresip_player()));
}

JNIEXPORT jboolean JNICALL
Java_com_xyt_sip_jni_Jni_is_1reg(JNIEnv *env, jobject obj, jstring _aor) {
    const char* aor = (*env)->GetStringUTFChars(env, _aor, 0);
    bool b_is_reg = is_reg(aor);
    (*env)->ReleaseStringUTFChars(env, _aor, aor);
    return b_is_reg ? JNI_TRUE : JNI_FALSE;
}


JNIEXPORT jstring JNICALL
Java_com_xyt_sip_jni_Jni_get_1current_1aor(JNIEnv *env, jobject obj) {
    struct ua *ua = uag_current();
    if (!ua) {
        return (*env)->NewStringUTF(env, "未注册");;
    } else {
        return (*env)->NewStringUTF(env, ua_aor(ua));
    }
}

JNIEXPORT jint JNICALL
Java_com_xyt_sip_jni_Jni_digit_1handler(JNIEnv *env, jobject obj, jbyte key) {
    struct call *call;
    int err = 0;

    call = ua_call(uag_current());
    if (call) {

        LOGD("key=%c\n", key);
        err = call_send_digit(call, key);
        /*
         * 发0的目的是,让baresip发送DTMF
         * baresip在收到key>0,缓存DTMF
         * key<=0或收到下一个DTMF时,发送缓存的DTMF消息
         */
        err += call_send_digit(call, 0);
    }

    RETURN_ERR
}
