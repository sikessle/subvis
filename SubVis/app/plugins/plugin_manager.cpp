#include <QtDebug>

#include "plugins/plugin_manager.h"

namespace subvis {

PluginManager::PluginManager(MeshData& mesh_data) : mesh_data_(mesh_data) {

}

const std::map<const QString, PluginWrapper>& PluginManager::get_plugins()
const {
  return plugins_;
}

void PluginManager::register_plugin(std::unique_ptr<SubVisPlugin> plugin) {
  const QString id = plugin->id();
  const QString name = plugin->name();
  plugins_[id] = {name, std::move(plugin)};
  plugins_[id].plugin->set_model(mesh_data_);
  qDebug() << "Registered plugin:" << name;
}

} // namespace subvis

