package com.xyt.sip.jni;

import android.content.Context;
import android.content.res.AssetManager;

import java.io.Closeable;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by zhangyangjing on 2018/5/3.
 */
class Util {
    private static final String SIP_ASSETS_DIR  = "baresip";
    private static final String SIP_CONFIG_NAME  = "baresip_config";

    public static String getSipConfigPath(Context ctx) {
        return new File(ctx.getFilesDir(), SIP_CONFIG_NAME).getAbsolutePath();
    }

    public static void ensureSipAssets(Context ctx, JniCallback mCallback) {
        // TODO: use version code
        String configPath = getSipConfigPath(ctx);
        copyAssets(ctx, SIP_ASSETS_DIR, configPath);
    }

    private static void copyAssets(Context ctx, String src, String dest) {
        AssetManager assetManager = ctx.getAssets();
        try {
            String assets[] = assetManager.list(src);
            if (assets.length == 0) {
                InputStream in = null;
                OutputStream out = null;
                try {
                    in = assetManager.open(src);
                    out = new FileOutputStream(dest);
                    byte[] buffer = new byte[1024];
                    int read;
                    while ((read = in.read(buffer)) != -1)
                        out.write(buffer, 0, read);
                } catch (Exception e) {
                    e.printStackTrace();
                } finally {
                    close(in);
                    close(out);
                }
            } else {
                File dir = new File(dest);
                if (!dir.exists())
                    dir.mkdirs();
                for (String name : assets)
                    copyAssets(ctx, src + File.separator +  name, dest  + File.separator + name);
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    public static void close(Closeable closeable) {
        try {
            if (null != closeable && closeable instanceof Closeable)
                closeable.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
