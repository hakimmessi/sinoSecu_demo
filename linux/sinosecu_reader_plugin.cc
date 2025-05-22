//
// Created by Hakim Messi on 2025/05/22.
//

#include "sinosecu_reader_plugin.h"
#include <flutter_linux/flutter_linux.h>
#include <iostream>
#include <dlfcn.h>

typedef int (*InitIDCardFunc)(const wchar_t*, int, const wchar_t*);

void* libHandle = nullptr;
InitIDCardFunc InitIDCard = nullptr;

SinosecuReaderPlugin::SinosecuReaderPlugin() {}

SinosecuReaderPlugin::~SinosecuReaderPlugin() {
    if (libHandle) {
        dlclose(libHandle);
    }
}

void SinosecuReaderPlugin::HandleMethodCall(
        const flutter::MethodCall<std::string> &call,
        std::unique_ptr<flutter::MethodResult<std::string>> result) {
    if (call.method_name().compare("initialize") == 0) {
        if (!libHandle) {
            libHandle = dlopen("libIDCard.so", RTLD_LAZY);
            if (!libHandle) {
                result->Error("LOAD_ERROR", dlerror());
                return;
            }

            InitIDCard = (InitIDCardFunc)dlsym(libHandle, "InitIDCard");
            if (!InitIDCard) {
                result->Error("SYM_ERROR", dlerror());
                return;
            }
        }

        int ret = InitIDCard(L"426911010110763248", 0, L".");
        result->Success(std::to_string(ret));
    } else {
        result->NotImplemented();
    }
}

void SinosecuReaderPlugin::RegisterWithRegistrar(FlPluginRegistrar* registrar)
 {
     auto channel = std::make_unique<flutter::MethodChannel<std::string>>(
             flutter_plugin_registrar_get_messenger(registrar),
                     "sinosecu_reader",
                     &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<SinosecuReaderPlugin>();

    channel->SetMethodCallHandler(
            [plugin_pointer = plugin.get()](const auto &call, auto result) {
                plugin_pointer->HandleMethodCall(call, std::move(result));
            });

    registrar->AddPlugin(std::move(plugin));
}
