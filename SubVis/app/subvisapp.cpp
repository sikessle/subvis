#include <QPixmap>
#include "view/mainwindow.h"
#include "subvisapp.h"

using namespace std;

namespace SubVis {

SubVisApp::SubVisApp()
{

}

int SubVisApp::run(int argc, char *argv[])
{
    QApplication app(argc, argv);
    View::MainWindow mainwindow;

    auto splash = create_show_splash(app);

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
        std::cerr << "Failed to load splash screen image" << std::endl;
        return nullptr;
    }

    unique_ptr<QSplashScreen> splash(new QSplashScreen(splash_image));

    splash->show();
    app.processEvents();

    return splash;
}

} // namespace SubVis

