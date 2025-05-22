//
// Created by Hakim Messi on 2025/05/22.
//

#ifndef SINOSECU_READER_PLUGIN_H
#define SINOSECU_READER_PLUGIN_H

#include <flutter_linux/flutter_linux.h>

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE(SinosecuReaderPlugin, sinosecu_reader_plugin, SINOSECU, READER_PLUGIN, GObject)

void sinosecu_reader_plugin_register_with_registrar(FlPluginRegistrar* registrar);

G_END_DECLS

#endif  // SINOSECU_READER_PLUGIN_H
