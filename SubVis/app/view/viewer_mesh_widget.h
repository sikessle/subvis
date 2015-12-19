#ifndef SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
#define SUBVIS_VIEW_VIEWER_MESH_WIDGET_H

#include "surface_mesh/Surface_mesh.h"

#include "view/mesh_edit_handler.h"
#include "view/edit_constraint.h"
#include "view/viewer_widget.h"

namespace subvis {

class ViewerMeshWidget : public ViewerWidget {
  Q_OBJECT

 public:
  ViewerMeshWidget(QWidget* parent = 0, int mesh_id = 0);
  void set_edit(bool edit);

 protected:
  void draw_gl() override;
  void init_gl() override;
  void mesh_updated(const surface_mesh::Surface_mesh& mesh) override;
  void mouseDoubleClickEvent(QMouseEvent* const event) override;
  void keyPressEvent(QKeyEvent* e) override;


 private:
  const surface_mesh::Surface_mesh* mesh_ {nullptr};
  std::unique_ptr<surface_mesh::Surface_mesh> editable_mesh_ {nullptr};
  MeshEditHandler mesh_edit_handler_{mesh_id_};

  void draw_mesh();
};


} // namespace subvis
#endif // SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
