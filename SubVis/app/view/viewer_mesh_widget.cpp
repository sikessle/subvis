#include <GL/glut.h>
#include <QtDebug>
#include <QMouseEvent>
#include "QGLViewer/manipulatedFrame.h"

#include "view/viewer_mesh_widget.h"

namespace subvis {

using Point = surface_mesh::Point;
using Vertex = surface_mesh::Surface_mesh::Vertex;

ViewerMeshWidget::ViewerMeshWidget(QWidget* parent,
                                   int mesh_id) :
  ViewerWidget{parent, mesh_id} {

  setMouseBinding(Qt::ControlModifier, Qt::LeftButton, QGLViewer::FRAME, QGLViewer::TRANSLATE);
}

void ViewerMeshWidget::set_edit(bool edit) {
  edit_ = edit;
  extract_vertices();
  qDebug() << "Edit set to" << edit;
}

void ViewerMeshWidget::mesh_updated(const surface_mesh::Surface_mesh& mesh) {
  mesh_ = &mesh;

  if (edit_) {
    extract_vertices();
  }

  // force redraw
  updateGL();
}

bool ViewerMeshWidget::is_edit_event(QMouseEvent* const event) const {
  return edit_
         && event->button() == Qt::LeftButton
         && mesh_;
}

void ViewerMeshWidget::extract_vertices() {
  id_to_vertex_.clear();

  if (!mesh_) {
    return;
  }

  qDebug() << "Copying mesh to allow for non-destructive editing";
  editable_mesh_ = std::unique_ptr<surface_mesh::Surface_mesh>
                   (new surface_mesh::Surface_mesh(*mesh_));

  qDebug() << "Extracting vertices and mapping to ids";

  for (Vertex vertex : editable_mesh_->vertices()) {
    id_to_vertex_.insert(std::pair<int, Vertex>(vertex.idx(), vertex));
  }
}

void ViewerMeshWidget::mouseDoubleClickEvent(QMouseEvent* const event) {
  // Delegate roations etc. to default behaviour, if we are not in edit mode.
  if (!is_edit_event(event)) {
    qDebug() << "Using default mouse behavior.";
    ViewerWidget::mouseDoubleClickEvent(event);
    return;
  }
  qDebug() << "Using edit mode mouse behavior.";

  // Save previous modification
  if (manipulatedFrame()) {
    qDebug() << "Saving previous modifications to a mesh point";
    qreal x, y, z;
    manipulatedFrame()->getPosition(x, y, z);
    Point& p = *editing_point_;
    p[0] = x;
    p[1] = y;
    p[2] = z;
    qDebug("New Position: %f %f %f", p[0], p[1], p[2]);
    qDebug() << "Saving modified mesh";

    mesh_data_->load_and_duplicate(std::move(editable_mesh_), mesh_id_);
    setManipulatedFrame(nullptr);
  } else {
    qDebug() << "Handling a new click on a mesh point";
    click_x_ = event->x();
    click_y_ = event->y();
    unhandled_click_ = true;
    // This will now trigger a redraw where we handle the click
  }
}


const surface_mesh::Surface_mesh::Vertex*
ViewerMeshWidget::get_vertex_at_click() const {
  glClearColor(1.f, 1.f, 1.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  unsigned char rgba[4];
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  const int viewport_height = viewport[3];

  // Limit drawing area to the clicked area around the mouse click coordinates.
  glScissor(click_x_ - kClickBoxLength / 2,
            viewport_height - click_y_ - kClickBoxLength / 2,
            kClickBoxLength,
            kClickBoxLength);

  // Start rendering the mesh with unique colors per vertex
  glEnable(GL_SCISSOR_TEST);

  glPointSize(kClickBoxLength);

  glBegin(GL_POINTS);
  for (const auto& id_vertex : id_to_vertex_) {
    // Set color based on idx
    index_to_rgba(id_vertex.first, rgba);
    const Point& p =
      editable_mesh_->get_vertex_property<Point>("v:point")[id_vertex.second];
    glColor4f(rgba[0] / 255.0f, rgba[1] / 255.0f, rgba[2] / 255.0f, 1.0f);
    glVertex3f(p[0], p[1], p[2]);
  }
  glEnd();

  glDisable(GL_SCISSOR_TEST);

  // Get pixel color of mouse click coordinates (1 pixel)
  unsigned char pixels[4];
  glReadPixels(click_x_, viewport_height - click_y_, 1, 1, GL_RGBA,
               GL_UNSIGNED_BYTE, pixels);

  // Convert color to vertex idx
  const unsigned int vertex_idx = rgba_to_index(pixels);

  // Get vertex by id
  if (id_to_vertex_.count(vertex_idx) > 0) {
    return &id_to_vertex_.at(vertex_idx);
  }

  return nullptr;
}


void ViewerMeshWidget::index_to_rgba(const int index,
                                     unsigned char rgba[4]) const {
  const unsigned char* index_ptr = (const unsigned char*) &index;

  for (int i = 0; i < kPixelsBytes; i++) {
    rgba[i] = index_ptr[i];
  }
  qDebug("Index %d to rgba: %d %d %d", index, rgba[0], rgba[1], rgba[2]);
}

int ViewerMeshWidget::rgba_to_index(const unsigned char rgba[4])
const {
  // rebuild index from r, g and b values.
  // inverse function of index_to_rgb.
  int index = 0;
  unsigned char* index_ptr = (unsigned char*) &index;

  for (int i = 0; i < kPixelsBytes; i++) {
    index_ptr[i] = rgba[i];
  }

  qDebug("Rgba %d %d %d to index: %d", rgba[0], rgba[1], rgba[2], index);

  return index;
}

void ViewerMeshWidget::draw_edit_handle() {

  // Save the current model view matrix
  glPushMatrix();
  // Multiply matrix to get in the frame coordinate system.
  glMultMatrixd(manipulatedFrame()->matrix());

  glColor3f(102 / 255.f, 0.f, 0.f);
  drawGrid();

  // Draw the vertex edit handle
  glPointSize(kEditHandleSize);
  glBegin(GL_POINTS);
  glColor4f(1.f, .0f, .0f, 1.0f);
  glVertex3f(0.f, 0.f, 0.f);
  glEnd();

  // TODO: Instead of both Mouse buttons use only left button + control!! (setMousebinding..)
  // Depth BUffer
  // Grids
  // computeNormals?

  // Restore the original (world) coordinate system
  glPopMatrix();
}


void ViewerMeshWidget::handle_click_during_draw() {
  qDebug() << "Handling last click";

  const Vertex* vertex = get_vertex_at_click();

  if (vertex != nullptr) {
    Point& handle = editable_mesh_->get_vertex_property<Point>("v:point")[*vertex];
    qDebug("Found vertex @ click (%d, %d): v%d with coordinates: %f %f %f",
           click_x_, click_y_, vertex->idx(), handle[0], handle[1], handle[2]);

    editing_point_ = &handle;
    setManipulatedFrame(new qglviewer::ManipulatedFrame());
    // Set to correct position
    manipulatedFrame()->setPosition(handle[0], handle[1], handle[2]);
    qDebug() << "Manipulated Frame created";

  } else {
    qDebug("No vertex found @ click (%d, %d)", click_x_, click_y_);
  }

  unhandled_click_ = false;
}




void ViewerMeshWidget::init_gl() {
  qDebug() << "Initializing open gl.";
  // TODO compute the correct scene radius and configure camera.
  setSceneRadius(1.0);
  camera()->setZNearCoefficient(0.0001);
  camera()->setZClippingCoefficient(10.0);
  //glEnable(GL_DEPTH_TEST);
  qDebug() << "Finished initializing open gl.";
}

void ViewerMeshWidget::draw_gl() {
  qDebug() << "Drawing mesh.";

  if (unhandled_click_) {
    handle_click_during_draw();
    // Trigger redraw to hide the color picking leftovers..
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
  if (manipulatedFrame()) {
    draw_edit_handle();
  }
}

// this is just a temporary prototype.
// This implementation is EXTREMLY SLOW!
void ViewerMeshWidget::draw_mesh() {
  using Point = surface_mesh::Point;

  // data structure
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

