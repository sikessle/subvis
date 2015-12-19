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

///
/// \brief Takes care of the edit operations on a mesh in a viewer.
///
class MeshEditHandler {
 public:
  ///
  /// \brief MeshEditHandler
  /// \param mesh_id The mesh idx which is managed by this class
  ///
  MeshEditHandler(int mesh_id);
  ///
  /// \brief Event handler to react on double clicks which may start or end
  /// edit operations.
  /// \param event The event
  /// \param mesh_data The mesh_data
  /// \param viewer The viewer which is using this handler
  /// \return True if the event was handled by this class. False if the event
  /// was not handled and should be delegated to other handlers.
  ///
  bool mouseDoubleClickEvent(QMouseEvent* const event, MeshData& mesh_data,
                             QGLViewer* viewer);
  ///
  /// \brief Event handler to react on key presses which may trigger some functionality
  /// regarding the edit operations.
  /// \param e The event
  /// \return True if the event was handled and a redraw might be necessary, false if not.
  ///
  bool keyPressEvent(QKeyEvent* e);
  ///
  /// \brief A callback which should be called every time before drawing the mesh.
  /// This allows the handler to handle any unhandled clicks and apply the color picking
  /// algorithm.
  /// \param viewer The viewer which uses this handler
  /// \return True if an unhandled click was handled and a redraw may be necessary.
  /// False otherwise.
  ///
  bool callback_handle_previous_click(QGLViewer* viewer);
  ///
  /// \brief Draws the edit handle of a vertex if necessary. Should be called every
  /// time after the main mesh drawing.
  /// \param viewer The viewer which uses this handler.
  ///
  void draw_edit_handle(QGLViewer* viewer);
  ///
  /// \brief Enables/Disables the handling and reaction of edit events.
  /// \param enabled
  ///
  void set_enabled(bool enabled);
  ///
  /// \brief Should be called every time the mesh is updated so that the edit operations
  /// refelct the latest state of the mesh.
  /// \param mesh The current mesh.
  ///
  void mesh_updated(const surface_mesh::Surface_mesh& mesh);
  ///
  /// \brief Allows the edit handler to setup some mouse bindings which are required
  /// for the handler to work. Should be called once, i.e. in a constructor of a viewer.
  /// \param viewer The viewer which uses this handler.
  ///
  void set_mouse_binding(QGLViewer* viewer);

 private:
  const int kPixelsBytes {3};
  const int kClickBoxLength {10};
  const int kEditHandleSize {10};
  MeshData* mesh_data_;
  int mesh_id_;
  // A copy of the mesh which is editable
  std::unique_ptr<surface_mesh::Surface_mesh> editable_mesh_ {nullptr};
  // The point which is currently edited. Is a pointer of a point in the editable_mesh_
  surface_mesh::Point* edited_point_ {nullptr};
  const surface_mesh::Surface_mesh* mesh_ {nullptr};
  std::map<int, const surface_mesh::Surface_mesh::Vertex> id_to_vertex_;
  // x coordinate of the last click
  int click_x_ {0};
  // y coordinate of the last click
  int click_y_ {0};
  // Indicates if there was a click which must be handled (applying color picking
  // on next redraw)
  bool unhandled_click_ {false};
  EditConstraint constraint_;
  // The normal of the edited vertex/point
  qglviewer::Vec edited_point_normal_;
  // The state of this handler
  bool enabled_ {false};
  // A frame which is used to draw the edit handle and move around with the mouse
  qglviewer::ManipulatedFrame manipulated_frame_;
  // Indicates whether there is an edit operation in progress (a manipulated frame
  // is set on the viewer and is moved around) or not.
  bool edit_in_progress_ {false};
  // Wheteher to use the orthogonal of the normal or not to constrain the translations.
  bool use_orthogonal_normal_ {false};

  // Extracts the vertices from the mesh and fills the id_to_vertex_ map
  void extract_vertices();
  // Checks if the event is an edit event which we must react to
  bool doubleclick_is_edit_event(QMouseEvent* const event) const;
  // Returns the vertex at the last click position by color picking
  const surface_mesh::Surface_mesh::Vertex* get_vertex_at_click() const;
  // RGBA color values will be stored in rgba array
  void index_to_rgba(const int index, unsigned char rgba[4]) const;
  // Allows rgba values
  int rgba_to_index(const unsigned char rgba[4]) const;
  // Draws the vertex normal
  void draw_vertex_normal();
  // Draws a line from the origin point of the edited vertex to the current new position.
  void draw_origin_line();
  // Draws the little knob which shows that the vertex is movable.
  void draw_edit_vertex();
  // Draws the current constraint mode.
  void draw_constraint_text(QGLViewer* viewer);
};

} // namespace subvis
#endif // SUBVIS_VIEW_MESH_EDIT_HANDLER_H
