#include <GL/glut.h>
#include <QtDebug>
#include <QMouseEvent>

#include "view/viewer_mesh_widget.h"

namespace subvis {

using Point = surface_mesh::Point;

ViewerMeshWidget::ViewerMeshWidget(QWidget* parent,
                                   int mesh_id) :
  ViewerWidget{parent, mesh_id} {
  // Initialize mouse bindings for the edit handling
  mesh_edit_handler_.set_mouse_binding(this);
}

void ViewerMeshWidget::set_edit(bool edit) {
  mesh_edit_handler_.set_enabled(edit);
  qDebug() << "Edit set to" << edit;
}

void ViewerMeshWidget::mesh_updated(const surface_mesh::Surface_mesh& mesh) {
  mesh_ = &mesh;
  mesh_edit_handler_.mesh_updated(mesh);
}

void ViewerMeshWidget::mouseDoubleClickEvent(QMouseEvent* const event) {
  // Event was not handled by the edit handler.
  if (!mesh_edit_handler_.mouseDoubleClickEvent(event, *mesh_data_, this)) {;
    ViewerWidget::mouseDoubleClickEvent(event);
  }
}

void ViewerMeshWidget::keyPressEvent(QKeyEvent* e) {
  // Edit handler changed and display might need to update
  if (mesh_edit_handler_.keyPressEvent(e)) {
    // Trigger redraw.
    updateGL();
  } else {
    // Propagate to super class.
    ViewerWidget::keyPressEvent(e);
  }
}

void ViewerMeshWidget::init_gl() {
  qDebug() << "Initializing open gl.";
  // TODO compute the correct scene radius and configure camera.
  setSceneRadius(10.0);
  camera()->setZNearCoefficient(0.0001);
  camera()->setZClippingCoefficient(10.0);
  qDebug() << "Finished initializing open gl.";
}

void ViewerMeshWidget::draw_gl() {
  qDebug() << "Drawing mesh.";

  // Edit handler had an unhandled click, redraw to clear any leftovers
  // from the color picking algorithm.
  if (mesh_edit_handler_.callback_handle_previous_click(this)) {
    updateGL();
    return;
  }

  //-------------------- SLOW MESH DRAWING
  // black background
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  // no lighting TODO: shadows etc. would be good..
  glDisable(GL_LIGHTING);
  // WIREFRAME
  glLineWidth(1.5);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // dark blue color
  glColor3f(0, 0.294, 0.419);
  glEnable(GL_POLYGON_OFFSET_LINE);
  glPolygonOffset(-1.0, -1.0);
  draw_mesh();

  // SOLID
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // blue (SubVis) color
  glColor3f(0.003, 0.615, 0.878);
  draw_mesh();
  //-------------------- END SLOW MESH DRAWING

  // Draw edit handle
  mesh_edit_handler_.draw_edit_handle(this);
}

// this is just a temporary prototype.
// This implementation is EXTREMLY SLOW!
void ViewerMeshWidget::draw_mesh() {
  if (!mesh_) {
    return;
  }

  for (const auto& face : mesh_->faces()) {
    glBegin(GL_POLYGON);
    for (const auto& vertex : mesh_->vertices(face)) {
      const Point& p = mesh_->get_vertex_property<Point>("v:point")[vertex];
      glVertex3d(p[0], p[1], p[2]);
    }
    glEnd();
  }
}

} // namespace subvis

