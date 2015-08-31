#include <QApplication>

#include "plugins/plugin_manager.h"

namespace subvis {

void PluginManager::set_draw_controller(DrawController& draw_controller)
{    
   for (const auto& it : plugins_) {
       it.second.plugin->set_draw_controller(&draw_controller);
    }
}

const std::map<QString, PluginWrapper>& PluginManager::list_plugins() const
{
    return plugins_;
}

void PluginManager::register_plugin(SubVisPlugin* plugin)
{
    const QString id = plugin->id();
    const QString name = plugin->name();
    plugins_[id] = {name, std::unique_ptr<SubVisPlugin>{plugin}};
}

} // namespace subvis

