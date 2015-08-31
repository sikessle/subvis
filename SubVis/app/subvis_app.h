#ifndef SUBVIS_SUBVIS_APP_H
#define SUBVIS_SUBVIS_APP_H

#include <memory>
#include <QSplashScreen>
#include <QApplication>

#include "plugins/plugin_manager.h"
#include "plugins/subvis_plugin.h"

namespace subvis {

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

} // namespace subvis

#endif // SUBVIS_SUBVIS_APP_H
