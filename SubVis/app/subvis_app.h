#ifndef SUBVIS_SUBVIS_APP_H
#define SUBVIS_SUBVIS_APP_H

/**
 * \mainpage SubVis - A Tool to visualize subdivision algorithms
 *
 * SubVis is an easy to use Software to load and to display arbitrary meshes.
 * Addicted to the topology the user can apply subdivision algorithms on the mesh.
 *
 * The following algorithms are supported:
 *  - Catmull-Clark
 *  - Doo-Sabin
 *  - Loop
 *  - Butterfly
 *  - Modified Butterfly
 *
 * Furthermore SubVis has an edit mode, in which the user can modify the mesh.
 *
 * SubVis was developed in the Team-Project (2015/16) of the HTWG Konstanz
 * by Tobias Keh, Simon Kessler and Felix Born.
 * The project was subervised by Prof. Dr. Georg Umlauf und Pascal Laube.
 *
 */

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
