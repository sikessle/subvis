#include <QApplication>
#include "plugin_manager.h"

namespace SubVis {

PluginManager::PluginManager(const QString &plugins_dir)
    : plugins_directory{plugins_dir}
{
}

const map<QString, PluginInfo> &PluginManager::list_plugins() const
{
    return plugins;
}

bool PluginManager::load_plugins(DrawController &draw_controller)
{
    QDir plugins_dir(qApp->applicationDirPath());
    switch_to_plugin_dir(plugins_dir);

    foreach (QString filename, plugins_dir.entryList(QDir::Files)) {
        QPluginLoader plugin_loader(plugins_dir.absoluteFilePath(filename));
        check_and_add_plugin(plugin_loader.instance(), plugin_loader, draw_controller);
    }
    return true;
}

void PluginManager::switch_to_plugin_dir(QDir &dir)
{
#if defined(Q_OS_WIN)
    if (dir.dirName().toLower() == "debug" || dir.dirName().toLower() == "release")
        dir.cdUp();
#elif defined(Q_OS_MAC)
    if (dir.dirName() == "MacOS") {
        dir.cdUp();
        dir.cdUp();
        dir.cdUp();
    }
#endif

    dir.cd(plugins_directory);
}

void PluginManager::check_and_add_plugin(QObject *plugin,
                                         QPluginLoader &plugin_loader,
                                         DrawController &draw_controller)
{
    if (!plugin) {
        return;
    }

    SubVisPlugin *subvis_plugin = qobject_cast<SubVisPlugin *>(plugin);

    if (!subvis_plugin) {
        return;
    }

    subvis_plugin->set_draw_controller(&draw_controller);

    const QString id {plugin_loader.metaData().value("MetaData").toObject()
                .value("id").toString()};
    const QString name {plugin_loader.metaData().value("MetaData").toObject()
                .value("name").toString()};

    plugins[id] = {name, unique_ptr<SubVisPlugin>{subvis_plugin}};
}

} // namespace SubVis

