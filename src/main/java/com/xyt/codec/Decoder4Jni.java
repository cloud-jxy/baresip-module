package com.xyt.codec;

import android.view.Surface;

/**
 * Created by jxy on 2018/4/2.
 */

public class Decoder4Jni {
    static public Surface mSurface = null;
    public AvcDecoder mDecoder = new AvcDecoder();

    public Decoder4Jni() {
    }

    public void start(int w, int h) {
        mDecoder.start(mSurface, w, h);
    }

    public void decode(byte[] data, int byteCount) {
        mDecoder.decode(data, byteCount);
    }

    public void stop() {
        mDecoder.stopDecode();
    }
}
