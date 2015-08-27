#include <QPixmap>

#include "view/mainwindow.h"
#include "model/mesh_data.h"
#include "controller/io_controller.h"
#include "controller/draw_controller.h"
#include "plugins/plugin_manager.h"
#include "plugins/subvis_plugin.h"
#include "plugins/subdivision/subdivision_algorithms.h"

#include "subvis_app.h"


namespace SubVis {

SubVisApp::SubVisApp(int& argc, char* argv[]) : QApplication(argc, argv)
{
}

void SubVisApp::register_plugin(SubVisPlugin* plugin)
{
    plugin_manager_.register_plugin(plugin);
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
    plugin_manager_.set_draw_controller(draw_controller);

    // View layer
    MainWindow mainwindow{draw_controller, io_controller, plugin_manager_.list_plugins()};

    // Signals
    QObject::connect(&mesh_data, SIGNAL(updated()),
                     &mainwindow, SIGNAL(mesh_updated()));

    mainwindow.show();

    if (splash) {
        splash->finish(&mainwindow);
    }

    return exec();
}

std::unique_ptr<QSplashScreen> SubVisApp::create_show_splash()
{
    if (splash_image_.isNull()) {
        std::cerr << "Failed to load splash screen image" << std::endl;
        return nullptr;
    }

    std::unique_ptr<QSplashScreen> splash{new QSplashScreen{splash_image_}};

    splash->show();
    processEvents();

    return splash;
}

} // namespace SubVis

