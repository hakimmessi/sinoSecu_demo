package com.ktk.sim_dispenser_demo_app;

import android.content.Context;
import io.flutter.embedding.android.FlutterActivity;
import io.flutter.embedding.engine.FlutterEngine;
import io.flutter.plugin.common.MethodChannel;

public class MainActivity extends FlutterActivity {
    private static final String CHANNEL = "sinosecu_reader";
    private final DocumentReader reader = new DocumentReader();

    @Override
    public void configureFlutterEngine(FlutterEngine flutterEngine) {
        super.configureFlutterEngine(flutterEngine);

        new MethodChannel(flutterEngine.getDartExecutor().getBinaryMessenger(), CHANNEL)
                .setMethodCallHandler((call, result) -> {
                    Context context = getApplicationContext();
                    switch (call.method) {
                        case "initialize":
                            int initResult = reader.initialize(context);
                            result.success(initResult);
                            break;

                        case "detectDocument":
                            result.success(reader.detectDocument());
                            break;

                        case "autoProcess":
                            result.success(reader.autoProcessIDCard());
                            break;

                        case "free":
                            reader.free();
                            result.success(null);
                            break;

                        default:
                            result.notImplemented();
                    }
                });
    }
}
