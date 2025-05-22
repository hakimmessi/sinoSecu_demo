//
// Created by Hakim Messi on 2025/05/22.
//

#ifndef SIM_DISPENSER_DEMO_APP_SINOSECU_READER_PLUGIN_H
#define SIM_DISPENSER_DEMO_APP_SINOSECU_READER_PLUGIN_H
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_linux.h>

#include <memory>

class SinosecuReaderPlugin : public flutter::Plugin {
public:
    static void RegisterWithRegistrar(FlPluginRegistrar* registrar);


    SinosecuReaderPlugin();
    virtual ~SinosecuReaderPlugin();

    // Disallow copy and assign.
    SinosecuReaderPlugin(const SinosecuReaderPlugin&) = delete;
    SinosecuReaderPlugin& operator=(const SinosecuReaderPlugin&) = delete;

private:
    // Handle method calls from Dart.
    void HandleMethodCall(
            const flutter::MethodCall<std::string> &method_call,
            std::unique_ptr<flutter::MethodResult<std::string>> result);
};
#endif //SIM_DISPENSER_DEMO_APP_SINOSECU_READER_PLUGIN_H
