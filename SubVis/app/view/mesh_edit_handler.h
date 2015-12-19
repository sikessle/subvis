#ifndef SUBVIS_VIEW_MESH_EDIT_HANDLER_H
#define SUBVIS_VIEW_MESH_EDIT_HANDLER_H

#include <QMouseEvent>
#include <QKeyEvent>
#include "QGLViewer/vec.h"
#include "QGLViewer/qglviewer.h"
#include "QGLViewer/manipulatedFrame.h"
#include "surface_mesh/Surface_mesh.h"

#include "model/mesh_data.h"
#include "view/edit_constraint.h"

namespace subvis {

enum EditTranslationType {VERTEX_NORMAL_PLANE, VERTEX_NORMAL_ORTHOGONAL_PLANE};

class MeshEditHandler {
 public:
  MeshEditHandler(int mesh_id);
  bool mouseDoubleClickEvent(QMouseEvent* const event, MeshData& mesh_data,
                             QGLViewer* viewer);
  bool keyPressEvent(QKeyEvent* e);
  bool callback_before_main_draw(QGLViewer* viewer);
  void draw_edit_handle(QGLViewer* viewer);
  void set_enabled(bool enabled);
  void mesh_updated(const surface_mesh::Surface_mesh& mesh);
  void set_mouse_binding(QGLViewer* viewer);

 private:
  const int kPixelsBytes {3};
  const int kClickBoxLength {10};
  const int kEditHandleSize {10};
  MeshData* mesh_data_;
  int mesh_id_;
  std::unique_ptr<surface_mesh::Surface_mesh> editable_mesh_ {nullptr};
  const surface_mesh::Surface_mesh* mesh_ {nullptr};
  std::map<int, const surface_mesh::Surface_mesh::Vertex> id_to_vertex_;
  int click_x_ {0};
  int click_y_ {0};
  bool unhandled_click_ {false};
  surface_mesh::Point* editing_point_ {nullptr};
  EditConstraint edit_constraint_;
  qglviewer::Vec vertex_normal_;
  EditTranslationType translation_type_ = VERTEX_NORMAL_PLANE;
  bool enabled_ {false};
  qglviewer::ManipulatedFrame manipulated_frame_;
  bool edit_in_progress_ {false};

  void extract_vertices();
  void callback_before_main_draw();
  bool is_edit_event(QMouseEvent* const event) const;
  const surface_mesh::Surface_mesh::Vertex* get_vertex_at_click() const;
  /// RGBA color values will be stored in rgba array
  void index_to_rgba(const int index, unsigned char rgba[4]) const;
  /// Allows rgba values
  int rgba_to_index(const unsigned char rgba[4]) const;
};

} // namespace subvis
#endif // SUBVIS_VIEW_MESH_EDIT_HANDLER_H
