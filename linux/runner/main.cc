#include "sinosecu_reader_plugin.h"

int main(int argc, char **argv) {
    g_autoptr(FlCommandLineArgv) command_line = fl_command_line_argv_new(argc, argv);
    g_autoptr(FlDartProject) project = fl_dart_project_new();
    fl_dart_project_set_dart_entrypoint_arguments(project, command_line->argv);

    FlView *view = fl_view_new(project);
    gtk_widget_show(GTK_WIDGET(view));

    // Register plugin
    SinosecuReaderPlugin::RegisterWithRegistrar(fl_plugin_registry_get_registrar_for_plugin(
            FL_PLUGIN_REGISTRY(view), "SinosecuReaderPlugin"));

    gtk_main();
    return 0;
}
