package com.ktk.sim_dispenser_demo_app;

import android.content.Context;
import android.content.res.AssetManager;

import androidx.annotation.NonNull;

import com.sun.jna.WString;
import com.sun.jna.ptr.IntByReference;

import java.io.*;

public class DocumentReader {

    private final CIDCardRecog idCardLib = CIDCardRecog.INSTANCE;

    public void prepareConfigFiles(Context context) throws IOException {
        AssetManager assetManager = context.getAssets();
        String assetFolder = "readerConfig";
        File destDir = new File(context.getFilesDir(), assetFolder);

        //if (!destDir.exists()) destDir.mkdirs();

        String[] files = assetManager.list(assetFolder);
        if (files == null) return;

        for (String file : files) {
            File outFile = new File(destDir, file);
            if (!outFile.exists()) {
                InputStream in = assetManager.open(assetFolder + "/" + file);
                OutputStream out = new FileOutputStream(outFile);

                byte[] buffer = new byte[1024];
                int read;
                while ((read = in.read(buffer)) != -1) {
                    out.write(buffer, 0, read);
                }

                in.close();
                out.flush();
                out.close();
            }
        }

        System.out.println("[Android] Assets copied to: " + destDir.getAbsolutePath());
    }

    public int init(@NonNull Context context) {
        File configDir = new File(context.getFilesDir(), "readerConfig");
        WString dirPath = new WString(configDir.getAbsolutePath());

        int result = idCardLib.InitIDCard(new WString("426911010110763248"), 0, dirPath);
        System.out.println("[Android] InitIDCard result: " + result);

        return result;
    }

    public int detectDocument() {
        int res = idCardLib.DetectDocument();
        System.out.println("[Android] DetectDocument: " + res);
        return res;
    }

    public String autoProcessIDCard() {
        IntByReference cardType = new IntByReference();
        int res = idCardLib.AutoProcessIDCard(cardType);
        System.out.println("[Android] AutoProcessIDCard result: " + res + ", cardType: " + cardType.getValue());

        return "{ \"result\": " + res + ", \"cardType\": " + cardType.getValue() + " }";
    }

    public void free() {
        idCardLib.FreeIDCard();
        System.out.println("[Android] Scanner released.");
    }

    public int initialize(Context context) {
        try {
            prepareConfigFiles(context);
            return init(context);
        } catch (IOException e) {
            e.printStackTrace();
            return 1; // indicate failure
        }
    }
}
