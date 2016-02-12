#ifndef SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
#define SUBVIS_VIEW_VIEWER_MESH_WIDGET_H

#include "surface_mesh/Surface_mesh.h"

#include "view/mesh_edit_handler.h"
#include "view/edit_constraint.h"
#include "view/viewer_widget.h"

namespace subvis {

///
/// \brief Displays a single mesh and allows for editing it.
///
class ViewerMeshWidget : public ViewerWidget {
  Q_OBJECT

 public:
  ///
  /// \brief Constructor.
  /// \param parent
  /// \param mesh_id The mesh idx which this viewer should use from the mesh data object.
  ///
  ViewerMeshWidget(QWidget* parent = 0, int mesh_id = 0);
  ///
  /// \brief Enable or disable edit mode.
  /// \param edit Edit mode
  ///
  void set_edit(bool edit);
  ///
  /// \brief Set drawing type (edges, vertices, faces)
  /// \param type Drawing type
  ///
  void set_drawing_type(GLenum type);
  ///
  /// \brief Set OpenGL shading type (flat, smooth)
  /// \param type Shading type
  ///
  void set_shading_type(GLenum type);
  ///
  /// \brief Set coloring active (example in code: coloring with coordinates as RGB colors)
  /// \param active Coloring active / not active
  ///
  void set_coloring(bool active);
  ///
  /// \brief Set lighting active / not active
  /// \param active Lighting active / not active
  ///
  void set_lighting(bool active);

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

  std::vector<unsigned int> vertex_indices_;
  std::vector<float> color_values_;
  surface_mesh::Surface_mesh::Vertex_property<surface_mesh::Point>  points_;
  surface_mesh::Surface_mesh::Vertex_property<surface_mesh::Point>  vnormals_;
  GLenum drawing_type_;
  GLenum shading_type_;
  bool coloring_active_;
  bool lighting_active_;

  void draw_mesh(void);
  void create_color_values(void);
};


} // namespace subvis
#endif // SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
