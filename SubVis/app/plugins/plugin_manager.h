#ifndef SUBVIS_PLUGINS_PLUGIN_MANAGER_H
#define SUBVIS_PLUGINS_PLUGIN_MANAGER_H

#include <memory>
#include <map>
#include <vector>
#include <QPluginLoader>
#include <QDir>

#include "plugins/subvis_plugin.h"
#include "model/mesh_data.h"

namespace subvis {

///
/// \brief Wraps the name of a plugin and the plugin itself.
///
struct PluginWrapper {
  QString name;
  std::unique_ptr<SubVisPlugin> plugin;
};

///
/// \brief Manages the plugins of the application.
///
class PluginManager {
 public:
  PluginManager(MeshData& mesh_data);
  ///
  /// \brief Registers a new plugin. Ownership is transfered.
  /// \param plugin The plugin to register
  ///
  void register_plugin(std::unique_ptr<SubVisPlugin> plugin);

  /**
   * @brief Lists all loaded plugins.
   * @return id->(name, plugin)
   */
  const std::map<const QString, PluginWrapper>& get_plugins() const;

 private:
  MeshData& mesh_data_;
  std::map<const QString, PluginWrapper> plugins_;
};

} // namespace subvis
#endif // SUBVIS_PLUGINS_PLUGIN_MANAGER_H
