package com.example.rebootxposed;

import android.content.Context;
import android.os.Environment;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class SoReleaseUtil {

    public static void release(Context context) throws IOException {
        File soFile = new File("/data/data/"+context.getPackageName()+"/lib/libCustomHook.so");
        File targetFile = new File(Environment.getExternalStorageDirectory().getPath() + File.separator + "libCustomHook.so");
        if (!targetFile.exists()) {
            targetFile.createNewFile();
        }
        int len = 0;
        byte[] buffer = new byte[1024];
        FileOutputStream fos = new FileOutputStream(targetFile);
        FileInputStream fis = new FileInputStream(soFile);
        while ((len = fis.read(buffer)) != -1) {
            fos.write(buffer, 0, len);
        }
        fis.close();
        fos.close();
    }
}
