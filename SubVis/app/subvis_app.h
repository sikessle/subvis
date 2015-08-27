#ifndef SUBVISAPP_SUBVIS_H
#define SUBVISAPP_H

#include <memory>
#include <QSplashScreen>
#include <QApplication>

#include "plugins/plugin_manager.h"
#include "plugins/subvis_plugin.h"

namespace SubVis {

class SubVisApp : protected QApplication
{
    Q_OBJECT

public:
    SubVisApp(int& argc, char* argv[]);
    /**
     * @brief Takes ownership of the plugin
     * @param plugin
     */
    void register_plugin(SubVisPlugin* plugin);
    int run();

private:
    PluginManager plugin_manager_;
    QPixmap splash_image_{":/media/splash.png"};

    std::unique_ptr<QSplashScreen> create_show_splash();
};

} // namespace SubVis

#endif // SUBVIS_SUBVISAPP_H
