#ifndef SUBVIS_VIEWER_PLUGIN_WIDGET_H
#define SUBVIS_VIEWER_PLUGIN_WIDGET_H

#include "plugins/subvis_plugin.h"
#include "view/viewer_widget.h"

namespace subvis {

class ViewerPluginWidget : public ViewerWidget {
  Q_OBJECT

 public:
  ViewerPluginWidget(QWidget* parent = 0);

  void set_drawing_plugin(SubVisPlugin* plugin);
  virtual void set_draw_controller(DrawController& draw_controller) override;

 protected:
  virtual void draw() override;
  virtual void init() override;

 private:
  SubVisPlugin* drawing_plugin_{nullptr};

 public slots:
  void mesh_updated(const surface_mesh::Surface_mesh& mesh);
};

} // namespace subvis
#endif // SUBVIS_VIEWER_PLUGIN_WIDGET_H
