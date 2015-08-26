#ifndef SUBVIS_PLUGIN_MANAGER_H
#define SUBVIS_PLUGIN_MANAGER_H

#include <memory>
#include <unordered_map>
#include <vector>
#include <QPluginLoader>
#include <QDir>
#include "subvis_plugin.h"
#include "controller/draw_controller.h"

namespace SubVis {

using std::unique_ptr;
using std::unordered_map;
using std::string;
using std::vector;

struct PluginInfo
{
    string name;
    unique_ptr<SubVisPlugin> plugin;
};

class PluginManager
{
public:
    PluginManager(const string &plugins_dir, DrawController &draw_ctrl);

    bool load_plugins();
    /**
     * @brief Lists all loaded plugins: id->(name, plugin)
     * @return
     */
    unordered_map<string, PluginInfo> &list_plugins();

private:
    unordered_map<string, PluginInfo> plugins;
    string key_active_plugin;
    string plugins_directory;
    DrawController &draw_controller;

    void switch_to_plugin_dir(QDir &dir);
    void check_and_add_plugin(QObject *plugin, QPluginLoader &plugin_loader);
};

} // namespace SubVis
#endif // SUBVIS_PLUGIN_MANAGER_H
