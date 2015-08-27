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

using std::unique_ptr;
using std::map;
using std::vector;

struct PluginWrapper
{
    QString name;
    unique_ptr<SubVisPlugin> plugin;
};

class PluginManager
{
public:
    PluginManager();

    void load_plugins(DrawController& draw_controller);
    /**
     * @brief Lists all loaded plugins: id->(name, plugin)
     * @return
     */
    const map<QString, PluginWrapper>& list_plugins() const;

private:
    map<QString, PluginWrapper> plugins_;
};

} // namespace SubVis
#endif // SUBVIS_PLUGIN_MANAGER_H
