#ifndef SUBVIS_VIEW_VIEWER_PLUGIN_WIDGET_H
#define SUBVIS_VIEW_VIEWER_PLUGIN_WIDGET_H

#include "plugins/subvis_plugin.h"
#include "view/viewer_widget.h"

namespace subvis {

///
/// \brief A viewer which delegates most operations to the active plugin.
///
class ViewerPluginWidget : public ViewerWidget {
  Q_OBJECT

 public:
  ///
  /// \brief Constructor.
  /// \param parent
  /// \param mesh_id The mesh idx which this viewer should use from the mesh data object.
  ///
  ViewerPluginWidget(QWidget* parent = 0, int mesh_id = 0);
  ///
  /// \brief Sets the active plugin which will be used to draw.
  /// \param plugin The plugin to use. If nullptr is given, no draw operations will happen.
  ///
  void set_drawing_plugin(SubVisPlugin* plugin);

 protected:
  void draw_gl() override;
  void init_gl() override;
  void mesh_updated(const surface_mesh::Surface_mesh& mesh) override;

 private:
  SubVisPlugin* drawing_plugin_{nullptr};
};

} // namespace subvis
#endif // SUBVIS_VIEW_VIEWER_PLUGIN_WIDGET_H
