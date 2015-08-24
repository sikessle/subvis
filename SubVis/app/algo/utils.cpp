
// ===============[ includes ]===============

#include <QDebug>

#include "surface_mesh/Surface_mesh.h"
#include "surface_mesh/IO.h"

#include "algo/utils.h"


namespace SubVis {

// ===============[ public implementation ]===============

void utils_debug_mesh(const surface_mesh::Surface_mesh& mesh, const QString& title) {
    qDebug() << "---------- " << title << " ----------" << endl;
    qDebug() << "vertices: " << mesh.n_vertices() << endl;
    qDebug() << "edges: " << mesh.n_edges() << endl;
    qDebug() << "faces: " << mesh.n_faces() << endl;
}

void utils_debug_point(const surface_mesh::Point point, const QString& title) {
    qDebug() << title  << ": " << point[0] << " " << point[1] << " " << point[2] << endl;
}

// ===============[ private implementation ]===============


} // namespace SubVis
