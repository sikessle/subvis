#ifndef SUBVIS_SUBVIS_APP_H
#define SUBVIS_SUBVIS_APP_H

#include <memory>
#include <QSplashScreen>
#include <QApplication>

#include "view/mainwindow.h"
#include "plugins/plugin_manager.h"
#include "plugins/subvis_plugin.h"

namespace subvis {

///
/// \brief The Subdivision Visualizer
///
class SubVisApp : public QApplication {
  Q_OBJECT

 public:
  SubVisApp(int& argc, char* argv[]);
  /// Registers a new plugin. Must be called before calling run()
  void register_plugin(std::unique_ptr<SubVisPlugin> plugin);
  ///
  /// \brief Starts this application and runs the main loop.
  /// @return The exit code
  ///
  int run();

 private:
  // Model layer
  MeshData mesh_data_;
  PluginManager plugin_manager_{mesh_data_};
  QPixmap splash_image_{":/media/splash.png"};

  std::unique_ptr<QSplashScreen> create_splash();
  void sleep();

 signals:
  /// Internal use
  void sleep_finished();
};

} // namespace subvis

#endif // SUBVIS_SUBVIS_APP_H
