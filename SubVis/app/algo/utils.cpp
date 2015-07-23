
// ===============[ includes ]===============

#include <QDebug>

#include "surface_mesh/Surface_mesh.h"
#include "surface_mesh/IO.h"

#include "utils.h"


namespace SubVis {
namespace Algo {

// ===============[ public implementation ]===============

void utils_q_debug_mesh(const surface_mesh::Surface_mesh& mesh, const QString& title) {
    qDebug() << "---------- " << title << " ----------" << endl;
    qDebug() << "vertices: " << mesh.n_vertices() << endl;
    qDebug() << "edges: " << mesh.n_edges() << endl;
    qDebug() << "faces: " << mesh.n_faces() << endl;
}

// ===============[ private implementation ]===============


} // namespace Algo
} // namespace SubVis
