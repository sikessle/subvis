#ifndef SUBVIS_VIEWER_MESH_WIDGET_H
#define SUBVIS_VIEWER_MESH_WIDGET_H

#include "view/viewer_widget.h"

namespace subvis {

class ViewerMeshWidget : public ViewerWidget {
 public:
  ViewerMeshWidget(QWidget* parent = 0);

 protected:
  virtual void draw() override;
  virtual void init() override;

 private:
  void draw_mesh(const surface_mesh::Surface_mesh& mesh);
};


} // namespace subvis
#endif // SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
