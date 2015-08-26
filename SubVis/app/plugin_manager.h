#ifndef SUBVIS_PLUGIN_MANAGER_H
#define SUBVIS_PLUGIN_MANAGER_H

#include <memory>
#include <map>
#include <vector>
#include <QPluginLoader>
#include <QDir>
#include "subvis_plugin.h"
#include "controller/draw_controller.h"

namespace SubVis {

using std::unique_ptr;
using std::map;
using std::vector;

struct PluginInfo
{
    QString name;
    unique_ptr<SubVisPlugin> plugin;
};

class PluginManager
{
public:
    PluginManager(const QString &plugins_dir);

    bool load_plugins(DrawController &draw_controller);
    /**
     * @brief Lists all loaded plugins: id->(name, plugin)
     * @return
     */
    const map<QString, PluginInfo> &list_plugins() const;

private:
    map<QString, PluginInfo> plugins;
    QString key_active_plugin;
    QString plugins_directory;

    void switch_to_plugin_dir(QDir &dir);
    void check_and_add_plugin(QObject *plugin,
                              QPluginLoader &plugin_loader,
                              DrawController &draw_controller);
};

} // namespace SubVis
#endif // SUBVIS_PLUGIN_MANAGER_H
