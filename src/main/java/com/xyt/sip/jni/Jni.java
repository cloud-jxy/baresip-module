package com.xyt.sip.jni;

/**
 * Created by jxy on 2018/4/4.
 */

class Jni {
    public static native int init(String configPath, JniCallback callback);

    public static native int close();
    public static native int run();

    public static native int reg(String text);
    public static native void unreg(String aor);
    public static native void unreg_all();
    public static native int call(String number);
    public static native int answer();
    public static native int hangup();
    public static native int set_current_account(String aor);

    public static native boolean is_reg(String aor);

    /*
        set/get语音文件路径.
    */
    public static native void set_play_path(String path);
    public static native String get_play_path();

    public static native String get_current_aor();

    public static native int digit_handler(byte key);
}
