
// ===============[ includes ]===============

#include <QDebug>

#include "surface_mesh/Surface_mesh.h"
#include "surface_mesh/IO.h"

#include "sd_catmull.h"
#include "utils.h"

namespace SubVis {
namespace Algo {

// ===============[ public implementation ]===============

void test_surface_mesh() {
    surface_mesh::Surface_mesh mesh;

    surface_mesh::Surface_mesh::Vertex u = mesh.add_vertex(surface_mesh::Point(0,1,0));
    surface_mesh::Surface_mesh::Vertex v = mesh.add_vertex(surface_mesh::Point(0,0,0));
    surface_mesh::Surface_mesh::Vertex w  = mesh.add_vertex(surface_mesh::Point(1,0,0));
    surface_mesh::Surface_mesh::Vertex x  = mesh.add_vertex(surface_mesh::Point(1,1,0));

    surface_mesh::Surface_mesh::Face f = mesh.add_quad(u, v, w, x);
}


void test_surface_mesh_read() {
    surface_mesh::Surface_mesh mesh;
    std::string path = kRootPathToObjFiles + kObjDemoFilesString[kCube];
    surface_mesh::read_mesh(mesh, path);
    std::vector<surface_mesh::Point> vec = mesh.points();
    utils_q_debug_mesh(mesh, QString("Cube Mesh"));

}

// ===============[ private implementation ]===============


} // namespace Algo
} // namespace SubVis
