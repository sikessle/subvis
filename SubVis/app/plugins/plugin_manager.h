#ifndef SUBVIS_PLUGIN_MANAGER_H
#define SUBVIS_PLUGIN_MANAGER_H

#include <memory>
#include <map>
#include <vector>
#include <QPluginLoader>
#include <QDir>

#include "plugins/subvis_plugin.h"
#include "controller/draw_controller.h"

namespace SubVis {

struct PluginWrapper
{
    QString name;
    std::unique_ptr<SubVisPlugin> plugin;
};

class PluginManager
{
public:
    /**
     * @brief Takes ownership of the plugin
     * @param plugin
     */
    void register_plugin(SubVisPlugin *plugin);

    /**
     * @brief Lists all loaded plugins: id->(name, plugin)
     * @return
     */
    const std::map<QString, PluginWrapper>& list_plugins() const;

    /**
     * @brief Sets the draw controller on every registered plugin
     * @param draw_controller
     */
    void set_draw_controller(DrawController& draw_controller);

private:
    std::map<QString, PluginWrapper> plugins_;
};

} // namespace SubVis
#endif // SUBVIS_PLUGIN_MANAGER_H
