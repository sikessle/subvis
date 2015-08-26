#ifndef SUBVIS_PLUGIN_MANAGER_H
#define SUBVIS_PLUGIN_MANAGER_H

#include <memory>
#include <unordered_map>
#include <QPluginLoader>
#include <QDir>
#include "subvis_plugin.h"

namespace SubVis {

using std::unique_ptr;
using std::unordered_map;
using std::string;

class PluginManager
{
public:
    PluginManager(const string &plugins_dir);

    bool load_plugins();

private:
    unordered_map<string, unique_ptr<SubVisPlugin>> plugins;
    string plugins_directory;

    void switch_to_plugin_dir(QDir &dir);
    void check_and_add_plugin(QObject *plugin, QPluginLoader &plugin_loader);
};

} // namespace SubVis
#endif // SUBVIS_PLUGIN_MANAGER_H
