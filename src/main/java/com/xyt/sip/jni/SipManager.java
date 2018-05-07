package com.xyt.sip.jni;

import android.content.Context;
import android.util.Log;

import java.util.LinkedHashSet;
import java.util.Set;

import rx.Observable;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;

/**
 * Created by zhangyangjing on 2018/5/3.
 */

// TODO: make as service
public class SipManager implements JniCallback {
    private static final String TAG = SipManager.class.getSimpleName();

    private State mState;
    private Context mContext;
    private Set<SipListener> mListeners;

    public SipManager(Context context) {
        Log.d(TAG, "SipManager() called with: context = [" + context + "]");
        mState = State.EXIT;
        mContext = context;
        mListeners = new LinkedHashSet<>();
        new SipThread(mContext, this).start();
    }

    public int reg(String name, String pswd, String domain) {
        Log.d(TAG, "reg() called with: name = [" + name + "], pswd = [" + pswd + "], domain = [" + domain + "]");
        String aor = String.format("sip:%s:%s@%s", name, pswd, domain);
        return Jni.reg(aor);
    }

    public boolean isReg(String name, String domain) {
        String aor = String.format("sip:%s@%s", name, domain);
        return Jni.is_reg(aor);
    }

    public void unreg(String name, String domain) {
        String aor = String.format("sip:%s@%s", name, domain);
        Jni.unreg(aor);
    }

    public void unregAll() {
        Jni.unreg_all();
    }

    public boolean isBusy() {
        // TODO: fix this
        return false;
    }

    public void call(String number) {
        Jni.call(number);
    }

    public void answer() {
        Jni.answer();
    }

    public void hangup() {
        Jni.hangup();
    }

    public void dtmf() {

    }

    public State getState() {
        return mState;
    }

    public void registerListener(SipListener listener) {
        mListeners.add(listener);
    }

    public void unregisterListener(SipListener listener) {
        mListeners.remove(listener);
    }

    public void release() {
        Jni.close();
    }

    @Override
    public void handle(Event call) {
//        Log.d(TAG, "handle() called with: call = [" + call + "]");
        Observable
            .just(call)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribe(new Action1<Event>() {
                @Override
                public void call(Event call) {
                    processSipMsg(call);
                }
            });
    }

    private void processSipMsg(Event call) {
        State state = State.valueOf(call.event);
        if (mState == state)
            return;

        mState = state;
        if (mState.toString().startsWith("CALL_")) {
            mState.callState = CallState.valueOf(call.status);
        }

        Log.v(TAG, "processSipMsg state:" + state + " callState:" + mState.callState);

        for (SipListener l : mListeners) {
            l.sipStateChange(mState);
        }
    }

    private static class SipThread extends Thread {
        private Context mContext;
        private JniCallback mCallback;

        public SipThread(Context context, JniCallback callback) {
            super("SipThread");
            mContext = context;
            mCallback = callback;
        }

        @Override
        public void run() {
            Util.ensureSipAssets(mContext, mCallback);
            String configPath = Util.getSipConfigPath(mContext);
            int result = Jni.init(configPath, mCallback);
            assert(0 == result);
            Jni.run();
        }
    }

    public interface SipListener {
        void sipStateChange(State state);
    }

    public enum State {
        // TODO: 详细的注释
        REGISTERING(0),
        REGISTER_OK(1),
        REGISTER_FAIL(2),
        UNREGISTERING(3),
        SHUTDOWN(4),
        EXIT(5),
        CALL_INCOMING(6),
        CALL_RINGING(7),
        CALL_PROGRESS(8),
        CALL_ESTABLISHED(9),
        CALL_CLOSED(10),
        CALL_TRANSFER_FAILED(11),
        CALL_DTMF_START(12),
        CALL_DTMF_END(13),
        CALL_RTCP(14);

        private int mId;
        public CallState callState;
        State(int id) {
            mId = id;
            callState = CallState.IDLE;
        }

        public static State valueOf(int id) {
            for (State state : State.values()) {
                if (state.mId == id)
                    return state;
            }
            return null;
        }
    }

    public enum CallState {
        IDLE(0),
        INCOMING(1),
        OUTGOING(2),
        RINGING(3),
        EARLY(4),
        ESTABLISHED(5),
        TERMINATED(6);

        private int mId;
        CallState(int id) {
            mId = id;
        }

        public static CallState valueOf(int id) {
            for (CallState state : CallState.values()) {
                if (state.mId == id)
                    return state;
            }
            return null;
        }
    }
}
