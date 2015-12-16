#ifndef SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
#define SUBVIS_VIEW_VIEWER_MESH_WIDGET_H

#include "surface_mesh/Surface_mesh.h"

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
  void mousePressEvent(QMouseEvent* const event) override;

 private:
  const surface_mesh::Surface_mesh* mesh_ {nullptr};
  bool edit_{false};
  const int kPixelsBytes {3};
  const int kClickBoxLength {6};
  std::map<int, const surface_mesh::Surface_mesh::Vertex> id_to_vertex_;
  int click_x_ {0};
  int click_y_ {0};

  const surface_mesh::Surface_mesh::Vertex* get_vertex_at_click() const;
  /// RGBA color values will be stored in rgba array
  void index_to_rgba(const int index, unsigned char rgba[4]) const;
  /// Allows rgba values
  int rgba_to_index(const unsigned char rgba[4]) const;
  void draw_mesh();
};


} // namespace subvis
#endif // SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
