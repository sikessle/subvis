#ifndef SUBVIS_PLUGIN_MANAGER_H
#define SUBVIS_PLUGIN_MANAGER_H

#include <memory>
#include <map>
#include <vector>
#include <QPluginLoader>
#include <QDir>

#include "plugins/subvis_plugin.h"
#include "controller/draw_controller.h"

namespace subvis {

struct PluginWrapper {
  QString name;
  std::unique_ptr<SubVisPlugin> plugin;
};

class PluginManager {
 public:
  void register_plugin(std::unique_ptr<SubVisPlugin> plugin);

  /**
   * @brief Lists all loaded plugins.
   * @return id->(name, plugin)
   */
  const std::map<const QString, PluginWrapper>& get_plugins() const;
  void set_draw_controller_on_plugins(DrawController& draw_controller);

 private:
  std::map<const QString, PluginWrapper> plugins_;
};

} // namespace subvis
#endif // SUBVIS_PLUGIN_MANAGER_H
