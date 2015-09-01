

#include <QDebug>
#include "surface_mesh/IO.h"
#include "plugins/subdivision/debug.h"

namespace subdivision {

using surface_mesh::Surface_mesh;
using surface_mesh::Point;

void debug_mesh(const Surface_mesh& mesh, const QString& title) {
  qDebug() << "---------- " << title << " ----------" << endl;
  qDebug() << "vertices: " << mesh.n_vertices() << endl;
  qDebug() << "edges: " << mesh.n_edges() << endl;
  qDebug() << "faces: " << mesh.n_faces() << endl;
}

void debug_point(const Point point, const QString& title) {
  qDebug() << title  << ": " << point[0] << " " << point[1] << " " << point[2] <<
           endl;
}

} // namespace subdivision
