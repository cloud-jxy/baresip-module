package com.xyt.codec;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.util.Log;
import android.view.Surface;

import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * Created by jxy on 2018/3/23.
 */

public class AvcDecoder {
    private MediaCodec mDecoder = null;
    private Surface mSurface = null;
    private int mCount = 0;

    public void start(Surface surface, int width, int height) {
        this.mSurface = surface;

        MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc", width, height);
        mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, 2500000);
        mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, 25);
        try {
            mDecoder = MediaCodec.createDecoderByType("video/avc");
        } catch (IOException e) {
            Log.e("Fuck", "Fail to create MediaCodec: " + e.toString());
        }

        mDecoder.configure(mediaFormat, surface, null, 0);
        mDecoder.start();
        mCount = 0;
    }

    public void decode(byte[] h264, int byteCount) {
        Log.e("Fuck", "decode XXXXXX\n");
        ByteBuffer[] inputBuffers = mDecoder.getInputBuffers();
        ByteBuffer[] outputBuffers = mDecoder.getOutputBuffers();
        Log.e("Fuck", String.format("addr: %s, %s", inputBuffers.toString(), outputBuffers.toString()));
        if (null == inputBuffers) {
            Log.e("Fuck", "null == inputBuffers");
        }
        if (null == outputBuffers) {
            Log.e("Fuck", "null == outbputBuffers 111");
        }

        int inputBufferIndex = mDecoder.dequeueInputBuffer(10000);
        if (inputBufferIndex >= 0) {
            ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
            inputBuffer.clear();
            inputBuffer.put(h264);
            // long sample_time = ;
            mDecoder.queueInputBuffer(inputBufferIndex, 0, h264.length, mCount * 1000000 / 20, 0);
            ++mCount;
        } else {
            Log.d("Fuck", "dequeueInputBuffer error");
        }

        ByteBuffer outputBuffer = null;
        MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
        int outputBufferIndex = mDecoder.dequeueOutputBuffer(bufferInfo, 10000);
        while (outputBufferIndex >= 0) {
            outputBuffer = outputBuffers[outputBufferIndex];
            mDecoder.releaseOutputBuffer(outputBufferIndex, true);
            outputBufferIndex = mDecoder.dequeueOutputBuffer(bufferInfo, 10000);
        }

        if (outputBufferIndex >= 0) {
            mDecoder.releaseOutputBuffer(outputBufferIndex, false);
        } else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
            outputBuffers = mDecoder.getOutputBuffers();
            Log.d("Fuck", "outputBufferIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED");
        } else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
            // Subsequent data will conform to new format.
            Log.d("Fuck", "outputBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED");
        }
    }

    public void stopDecode() {
        if (mDecoder == null) {
            mDecoder.stop();
        }
    }
}
