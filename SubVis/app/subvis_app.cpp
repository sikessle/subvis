#include <QPixmap>
#include <exception>
#include <QtConcurrent/QtConcurrent>

#include "model/mesh_data.h"
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

  MainWindow mainwindow{mesh_data_, plugin_manager_};

  QEventLoop loop;

  QtConcurrent::run(this, &SubVisApp::sleep);
  // Start thread
  connect(this, &SubVisApp::sleep_finished, &loop, &QEventLoop::quit);

  // Blocks until thread is finished
  loop.exec();

  mainwindow.show();
  splash->finish(&mainwindow);

  return exec();
}

void SubVisApp::sleep() {
  QThread::msleep(1500);

  emit sleep_finished();
}

std::unique_ptr<QSplashScreen> SubVisApp::create_splash() {
  if (splash_image_.isNull()) {
    throw std::runtime_error{"Failed to load splash screen image."};
  }

  return std::unique_ptr<QSplashScreen> {new QSplashScreen{splash_image_}};
}

} // namespace subvis

