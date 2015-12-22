#ifndef SUBVIS_SUBVIS_PLUGIN_H
#define SUBVIS_SUBVIS_PLUGIN_H

#include <string>
#include <QWidget>

#include "model/mesh_data.h"

namespace subvis {

///
/// \brief The interface which a plugin must implement.
///
class SubVisPlugin : public QObject {
  Q_OBJECT
 public:
  virtual ~SubVisPlugin() {}

  ///
  /// \brief A unique identifier of this plugin. May be follow a package-style naming
  /// convention.
  /// \return A unique identifier of this plugin.
  ///
  virtual const QString id() const = 0;
  ///
  /// \brief A human readable name which cleary states the purpose of this plugin.
  /// \return A human readable name
  ///
  virtual const QString name() const = 0;
  ///
  /// \brief The text to display in the large viewer for the second plugin tab.
  /// \return A short text (i.e. limit surface etc.)
  ///
  virtual const QString viewer_tab_text() const = 0;
  ///
  /// \brief Will be called to set the model
  /// \param mesh_data The model
  ///
  virtual void init(MeshData& mesh_data) = 0;
  ///
  /// \brief Will be called every time the mesh was updated.
  /// \param mesh The updated mesh
  /// \param mesh_id The id of the updated mesh
  ///
  virtual void mesh_updated(const surface_mesh::Surface_mesh& mesh,
                            int mesh_id) = 0;
  ///
  /// \brief Called to initialize the opengl context
  /// \param mesh_id The mesh id for which the init is made
  ///
  virtual void init_opengl(int mesh_id) = 0;
  ///
  /// \brief Called to during drawing
  /// \param mesh_id The mesh id for which the draw is made
  ///
  virtual void draw_opengl(int mesh_id) = 0;
  ///
  /// \brief Called to create plugin specific gui. Implementations can here
  /// create their own widgets etc. and attach them to the parent.
  /// \param parent The parent of the plugin gui.
  ///
  virtual void create_gui(QWidget* parent) = 0;

 signals:
  ///
  /// \brief Plugins can emit this signal to indicate that they need a redraw.
  /// It is guaranteed that a viewer will react to this signal and trigger a redraw
  /// and consequently a call to draw_opengl().
  ///
  void needs_redraw();
};

} // namespace subvis
#endif // SUBVIS_SUBVIS_PLUGIN_H

