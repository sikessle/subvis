#include <QPixmap>
#include <QDir>
#include <QPluginLoader>
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

    // View layer
    MainWindow mainwindow{draw_controller, io_controller};

    // Signals
    QObject::connect(&mesh_data, SIGNAL(updated()),
                     &mainwindow, SIGNAL(mesh_updated()));

    // Plugins
    if (!load_plugins()) {
        cerr << "Failed to load plugins." << endl;
    }

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

bool SubVisApp::load_plugins()
{
    QDir plugins_dir(qApp->applicationDirPath());

    #if defined(Q_OS_WIN)
    if (plugins_dir.dirName().toLower() == "debug" || plugins_dir.dirName().toLower() == "release")
        plugins_dir.cdUp();
    #elif defined(Q_OS_MAC)
    if (plugins_dir.dirName() == "MacOS") {
        plugins_dir.cdUp();
        plugins_dir.cdUp();
        plugins_dir.cdUp();
    }
    #endif

    plugins_dir.cd("plugins");

    foreach (QString filename, plugins_dir.entryList(QDir::Files)) {
        QPluginLoader plugin_loader(plugins_dir.absoluteFilePath(filename));
        QObject *plugin = plugin_loader.instance();
        if (plugin) {
            SubVisPlugin *subvis_plugin = qobject_cast<SubVisPlugin *>(plugin);
            if (!subvis_plugin) {
                return false;
            } else {
                const string key {plugin_loader.metaData().value("MetaData").toObject()
                            .value("id").toString().toStdString()};

                plugins[key] = unique_ptr<SubVisPlugin>{subvis_plugin};
            }
        }
    }
    return true;
}

} // namespace SubVis

