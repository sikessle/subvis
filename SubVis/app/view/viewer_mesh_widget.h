#ifndef SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
#define SUBVIS_VIEW_VIEWER_MESH_WIDGET_H

#include "surface_mesh/Surface_mesh.h"

#include "view/edit_mesh_mouse_grabber.h"
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

 private:
  // TODO placeholder, here we should use a custom data structure to hold vertices
  const surface_mesh::Surface_mesh* mesh_ {nullptr};
  bool edit_{false};
  EditMeshMouseGrabber mouse_grabber_;

  void draw_mesh();
  /// RGBA color values will be stored in rgba array
  void index_to_rgba(const int index, int rgba[4]) const;
  /// Allows rgba values (4)
  int rgba_to_index(const int rgba[4]) const;
};


} // namespace subvis
#endif // SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
