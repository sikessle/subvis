#include <QPixmap>
#include "subvis_app.h"
#include "view/mainwindow.h"
#include "model/mesh_data.h"
#include "controller/io_controller.h"
#include "controller/draw_controller.h"


namespace SubVis {

using std::cerr;
using std::endl;

SubVisApp::SubVisApp(int argc, char *argv[]) : QApplication(argc, argv)
{
}

int SubVisApp::run()
{
    auto splash = create_show_splash();

    // Model layer
    MeshData mesh_data;

    // Controller layer
    IOController io_controller{mesh_data};
    DrawController draw_controller{mesh_data};

    // Plugins
    PluginManager plugin_manager {"plugins"};
    if (!plugin_manager.load_plugins(draw_controller)) {
        cerr << "Failed to load plugins." << endl;
    }

    // View layer
    MainWindow mainwindow{draw_controller, io_controller, plugin_manager};

    // Signals
    QObject::connect(&mesh_data, SIGNAL(updated()),
                     &mainwindow, SIGNAL(mesh_updated()));

    // Plugins
    mainwindow.create_plugin_guis();

    mainwindow.show();

    if (splash) {
        splash->finish(&mainwindow);
    }

    return exec();
}

unique_ptr<QSplashScreen> SubVisApp::create_show_splash()
{
    QPixmap splash_image{":/media/splash.png"};

    if (splash_image.isNull()) {
        cerr << "Failed to load splash screen image" << endl;
        return nullptr;
    }

    unique_ptr<QSplashScreen> splash{new QSplashScreen{splash_image}};

    splash->show();
    processEvents();

    return splash;
}

} // namespace SubVis

