#ifndef SUBVIS_VIEW_VIEWER_PLUGIN_WIDGET_H
#define SUBVIS_VIEW_VIEWER_PLUGIN_WIDGET_H

#include "plugins/subvis_plugin.h"
#include "view/viewer_widget.h"

namespace subvis {

class ViewerPluginWidget : public ViewerWidget {
  Q_OBJECT

 public:
  ViewerPluginWidget(QWidget* parent = 0, int mesh_id = 0);

  void set_drawing_plugin(SubVisPlugin* plugin);

 protected:
  void draw() override;
  void init() override;
  void mesh_updated(const surface_mesh::Surface_mesh& mesh) override;

 private:
  SubVisPlugin* drawing_plugin_{nullptr};
};

} // namespace subvis
#endif // SUBVIS_VIEW_VIEWER_PLUGIN_WIDGET_H
