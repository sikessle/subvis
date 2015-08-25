#include <QPixmap>
#include "subvisapp.h"
#include "view/mainwindow.h"
#include "model/mesh_data.h"
#include "controller/io_controller.h"


namespace SubVis {

using std::string;
using std::cerr;
using std::endl;

SubVisApp::SubVisApp()
{

}

int SubVisApp::run(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainwindow;

    auto splash = create_show_splash(app);

    // wire together model, view and controller layers
    MeshData mesh_data;
    IOController io_controller(mesh_data);

    QObject::connect(&mesh_data, SIGNAL(updated()),
                     &io_controller, SLOT(mesh_updated()));

    // TODO add view component

    mainwindow.show();

    if (splash) {
        splash->finish(&mainwindow);
    }

    return app.exec();
}

unique_ptr<QSplashScreen> SubVisApp::create_show_splash(const QApplication &app)
{
    QPixmap splash_image(":/media/splash.png");

    if (splash_image.isNull()) {
        cerr << "Failed to load splash screen image" << endl;
        return nullptr;
    }

    unique_ptr<QSplashScreen> splash(new QSplashScreen(splash_image));

    splash->show();
    app.processEvents();

    return splash;
}

} // namespace SubVis

