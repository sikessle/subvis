#include <QPixmap>
#include <exception>

#include "view/mainwindow.h"
#include "model/mesh_data.h"
#include "controller/io_controller.h"
#include "controller/draw_controller.h"
#include "plugins/plugin_manager.h"
#include "plugins/subvis_plugin.h"
#include "plugins/subdivision/subdivision_algorithms_plugin.h"

#include "subvis_app.h"


namespace subvis {

SubVisApp::SubVisApp(int& argc, char* argv[]) : QApplication(argc, argv) {
}

void SubVisApp::register_plugin(std::unique_ptr<SubVisPlugin> plugin) {
  plugin_manager_.register_plugin(std::move(plugin));
}

int SubVisApp::run() {
  const auto splash = create_splash();
  splash->show();
  processEvents();

  // Model layer
  MeshData mesh_data;

  // Controller layer
  IOController io_controller{mesh_data};
  DrawController draw_controller{mesh_data};

  // Plugins
  plugin_manager_.set_draw_controller(draw_controller);

  // View layer
  MainWindow mainwindow{draw_controller, io_controller, plugin_manager_.get_plugins()};

  // Signals
  QObject::connect(&mesh_data, SIGNAL(updated()),
                   &mainwindow, SIGNAL(mesh_updated()));

  mainwindow.show();
  splash->finish(&mainwindow);

  return exec();
}

std::unique_ptr<QSplashScreen> SubVisApp::create_splash() {
  if (splash_image_.isNull()) {
    throw std::runtime_error("Failed to load splash screen image.");
  }
  std::unique_ptr<QSplashScreen> splash{new QSplashScreen{splash_image_}};

  return splash;
}

} // namespace subvis

