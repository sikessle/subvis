#include <QPixmap>
#include "subvisapp.h"
#include "view/mainwindow.h"
#include "model/mesh_data.h"
#include "controller/io_controller.h"
#include "controller/draw_controller.h"


namespace SubVis {

using std::string;
using std::cerr;
using std::endl;

SubVisApp::SubVisApp()
{

}

int SubVisApp::run(int argc, char *argv[])
{
    QApplication app{argc, argv};

    auto splash = create_show_splash(app);

    // Model layer
    MeshData mesh_data;

    // Controller layer
    IOController io_controller{mesh_data};
    DrawController draw_controller{mesh_data};

    // View layer
    MainWindow mainwindow{draw_controller};
    // TODO: add io_controller and connect with dialogs

    // Signals
    QObject::connect(&mesh_data, SIGNAL(updated()),
                     &mainwindow, SIGNAL(mesh_updated()));

    mainwindow.show();

    if (splash) {
        splash->finish(&mainwindow);
    }

    return app.exec();
}

unique_ptr<QSplashScreen> SubVisApp::create_show_splash(const QApplication &app)
{
    QPixmap splash_image{":/media/splash.png"};

    if (splash_image.isNull()) {
        cerr << "Failed to load splash screen image" << endl;
        return nullptr;
    }

    unique_ptr<QSplashScreen> splash{new QSplashScreen{splash_image}};

    splash->show();
    app.processEvents();

    return splash;
}

} // namespace SubVis

