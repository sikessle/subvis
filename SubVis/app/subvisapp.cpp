#include <QPixmap>
#include "view/mainwindow.h"
#include "subvisapp.h"
#include "model/mesh_data.h"
#include "controller/io_controller.h"


namespace SubVis {

SubVisApp::SubVisApp()
{

}

int SubVisApp::run(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainwindow;

    auto splash = create_show_splash(app);

    mainwindow.show();

    // wire together model, view and controller layers
    MeshData mesh_data;
    IOController io_controller(mesh_data);
    QObject::connect(&mesh_data, SIGNAL(updated()),
                     &io_controller, SLOT(mesh_updated()));
    // TODO add view component

    if (splash) {
        splash->finish(&mainwindow);
    }

    return app.exec();
}

std::unique_ptr<QSplashScreen> SubVisApp::create_show_splash(const QApplication &app)
{
    QPixmap splash_image(":/media/splash.png");

    if (splash_image.isNull()) {
        std::cerr << "Failed to load splash screen image" << std::endl;
        return nullptr;
    }

    std::unique_ptr<QSplashScreen> splash(new QSplashScreen(splash_image));

    splash->show();
    app.processEvents();

    return splash;
}

} // namespace SubVis

