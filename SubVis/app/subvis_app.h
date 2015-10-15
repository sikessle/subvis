#ifndef SUBVIS_SUBVIS_APP_H
#define SUBVIS_SUBVIS_APP_H

#include <memory>
#include <QSplashScreen>
#include <QApplication>

#include "plugins/plugin_manager.h"
#include "plugins/subvis_plugin.h"

namespace subvis {

class SubVisApp : protected QApplication {
  Q_OBJECT

 public:
  SubVisApp(int& argc, char* argv[]);
  void register_plugin(std::unique_ptr<SubVisPlugin> plugin);
  int run();

 private:
  MeshData mesh_data_;
  PluginManager plugin_manager_{mesh_data_};
  QPixmap splash_image_{":/media/splash.png"};

  std::unique_ptr<QSplashScreen> create_splash();
};

} // namespace subvis

#endif // SUBVIS_SUBVIS_APP_H
