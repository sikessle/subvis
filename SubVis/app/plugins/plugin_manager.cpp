#include <QApplication>

#include "plugins/plugin_manager.h"

namespace SubVis {

PluginManager::PluginManager()
{
}

const map<QString, PluginWrapper>& PluginManager::list_plugins() const
{
    return plugins_;
}

void PluginManager::load_plugins(DrawController& draw_controller)
{

}

} // namespace SubVis

