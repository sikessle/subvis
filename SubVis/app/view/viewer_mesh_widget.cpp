#include <GL/glut.h>
#include <QtDebug>
#include <QMouseEvent>

#include "view/viewer_mesh_widget.h"

namespace subvis {

using Point = surface_mesh::Point;
using Vertex = surface_mesh::Surface_mesh::Vertex;

ViewerMeshWidget::ViewerMeshWidget(QWidget* parent,
                                   int mesh_id) :
  ViewerWidget{parent, mesh_id} {
}

void ViewerMeshWidget::set_edit(bool edit) {
  edit_ = edit;
  qDebug() << "Edit set to" << edit;
}

void ViewerMeshWidget::mesh_updated(const surface_mesh::Surface_mesh& mesh) {
  mesh_ = &mesh;

  if (edit_) {
    id_to_vertex_.clear();

    for (Vertex vertex : mesh_->vertices()) {
      id_to_vertex_.insert(std::pair<int, Vertex>(vertex.idx(), vertex));
    }
  }

  // force redraw
  updateGL();
}


void ViewerMeshWidget::mousePressEvent(QMouseEvent* const event) {
  // Delegate roations etc. to default behaviour, if we are not in
  // edit mode and not the CTRL+Left_Mouseclick is used.
  if (!edit_ || event->button() != Qt::LeftButton || !event->modifiers().testFlag(Qt::ControlModifier)) {
    qDebug() << "Using default mouse behavior.";
    ViewerWidget::mousePressEvent(event);
    return;
  }
  qDebug() << "Using edit mode mouse behavior.";
  if (!mesh_) {
    qDebug() << "No mesh via mesh_updated received. Aborting mouse event handling.";
    return;
  }

  click_x_ = event->x();
  click_y_ = event->y();

  // Handle click

  const Vertex* vertex = get_vertex_at_click();

  if (vertex != nullptr) {
    Point& point = mesh_->get_vertex_property<Point>("v:point")[*vertex];

    // This works! So we can just use a reference.
    // BUT: On Edit start: COPY the mesh, then on end STORE BOTH (copy again to 2. instance)
    // BUT if we are done, we must sync the second mesh as well

    point[0] = 0.0;
    point[1] = 0.0;
    point[2] = 0.0;

    qDebug("Found vertex @ click (%d, %d): v%d with coordinates: %f %f %f",
           click_x_, click_y_, vertex->idx(), point[0], point[1], point[2]);
  } else {
    qDebug("No vertex found @ click (%d, %d)", click_x_, click_y_);
  }
}


const surface_mesh::Surface_mesh::Vertex*
ViewerMeshWidget::get_vertex_at_click() const {
  glClearColor(255, 255, 255, 255);
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
      mesh_->get_vertex_property<Point>("v:point")[id_vertex.second];
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
    const Vertex& vertex = id_to_vertex_.at(vertex_idx);
    return &vertex;
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






void ViewerMeshWidget::init_gl() {
  qDebug() << "Initializing open gl.";
  // TODO compute the correct scene radius and configure camera.
  setSceneRadius(10.0);
  camera()->setZNearCoefficient(0.0001);
  camera()->setZClippingCoefficient(10.0);
  //glEnable(GL_DEPTH_TEST);
  qDebug() << "Finished initializing open gl.";
}

void ViewerMeshWidget::draw_gl() {
  qDebug() << "Drawing mesh.";

  // black background
  glClearColor(0, 0, 0, 0);
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
}

// TODO: We should rather pass a VBO to the graphics card to speed things up.
// this is just a temporary prototype.
// This implementation is EXTREMLY SLOW!
void ViewerMeshWidget::draw_mesh() {
  using Point = surface_mesh::Point;

  // mesh not yet loaded. TODO this can be removed if we use an appropriate container for our custom
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

