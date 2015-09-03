#ifndef SUBVIS_VIEWER_MESH_WIDGET_H
#define SUBVIS_VIEWER_MESH_WIDGET_H

#include "surface_mesh/Surface_mesh.h"

#include "view/viewer_widget.h"

namespace subvis {

class ViewerMeshWidget : public ViewerWidget {
  Q_OBJECT

 public:
  ViewerMeshWidget(QWidget* parent = 0);

  virtual void set_draw_controller(DrawController& draw_controller) override;

 protected:
  virtual void draw() override;
  virtual void init() override;

 private:
  // TODO placeholder, here we should use a custom data structure to hold vertices
  const surface_mesh::Surface_mesh* mesh_{nullptr};

  void draw_mesh();

 public slots:
  void mesh_updated(const surface_mesh::Surface_mesh& mesh);
};


} // namespace subvis
#endif // SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
