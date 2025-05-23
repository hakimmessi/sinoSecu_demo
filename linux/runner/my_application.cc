#include "sinosecu_reader_plugin.h"
#include <flutter_linux/flutter_linux.h>
#include <glib-object.h>
#include <string.h>
#include <iostream>
#include <dlfcn.h>

#define SINOSECU_READER_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), sinosecu_reader_plugin_get_type(), SinosecuReaderPlugin))

struct _SinosecuReaderPlugin {
    GObject parent_instance;
};

G_DEFINE_TYPE(SinosecuReaderPlugin, sinosecu_reader_plugin, g_object_get_type())

typedef int (*InitIDCardFunc)(const wchar_t*, int, const wchar_t*);

static void* libHandle = nullptr;
static InitIDCardFunc InitIDCard = nullptr;

static void method_call_cb(FlMethodChannel* channel,
                           FlMethodCall* method_call,
                           gpointer user_data) {
    const gchar* method = fl_method_call_get_name(method_call);

    if (strcmp(method, "initialize") == 0) {
        if (!libHandle) {
            libHandle = dlopen("libIDCard.so", RTLD_LAZY);
            if (!libHandle) {
                g_autoptr(FlMethodResponse) response = FL_METHOD_RESPONSE(
                        fl_method_error_response_new("LOAD_ERROR", dlerror(), nullptr));
                fl_method_channel_invoke_response(channel, method_call, response, nullptr);
                return;
            }

            InitIDCard = (InitIDCardFunc)dlsym(libHandle, "InitIDCard");
            if (!InitIDCard) {
                g_autoptr(FlMethodResponse) response = FL_METHOD_RESPONSE(
                        fl_method_error_response_new("SYM_ERROR", dlerror(), nullptr));
                fl_method_channel_invoke_response(channel, method_call, response, nullptr);
                return;
            }
        }

        int ret = InitIDCard(L"426911010110763248", 0, L".");
        char ret_str[16];
        snprintf(ret_str, sizeof(ret_str), "%d", ret);
        g_autoptr(FlValue) result = fl_value_new_string(ret_str);
        g_autoptr(FlMethodResponse) response = FL_METHOD_RESPONSE(
                fl_method_success_response_new(result));
        fl_method_channel_invoke_response(channel, method_call, response, nullptr);
    } else {
        g_autoptr(FlMethodResponse) response = FL_METHOD_RESPONSE(
                fl_method_not_implemented_response_new());
        fl_method_channel_invoke_response(channel, method_call, response, nullptr);
    }
}

void sinosecu_reader_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
    g_autoptr(SinosecuReaderPlugin) plugin = SINOSECU_READER_PLUGIN(
            g_object_new(sinosecu_reader_plugin_get_type(), nullptr));

    g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
    g_autoptr(FlMethodChannel) channel = fl_method_channel_new(
            fl_plugin_registrar_get_messenger(registrar),
            "sinosecu_reader",
            FL_METHOD_CODEC(codec));

    fl_method_channel_set_method_call_handler(channel, method_call_cb, plugin, nullptr);
}
