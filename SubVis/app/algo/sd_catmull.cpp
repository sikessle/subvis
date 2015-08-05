
// ===============[ includes ]===============

#include <QDebug>

#include "surface_mesh/Surface_mesh.h"
#include "surface_mesh/IO.h"

#include "sd_catmull.h"
#include "utils.h"

namespace SubVis {
namespace Algo {

// ===============[ private prototypes ]===============


// ===============[ public implementation ]===============

void test_surface_mesh_read() {
    surface_mesh::Surface_mesh mesh;
    std::string path = kRootPathToObjFiles + kObjDemoFilesString[kCube];
    // read mesh and print basic info to stdout
    surface_mesh::read_mesh(mesh, path);
    utils_q_debug_mesh(mesh, QString("Cube Mesh"));
    // compute subdivision
    SubdivCatmull sd_catmull(mesh);
    sd_catmull.subdivide(1);
}


// ===============[ class implementation ]===============

void SubdivCatmull::subdivide(int steps) {
    // TODO implement steps
    // instantiate iterator
    surface_mesh::Surface_mesh::Face_iterator fit;
    // loop over all faces
    for (fit = mesh_.faces_begin(); fit != mesh_.faces_end(); ++fit)
    {
        // TODO subdivision
        surface_mesh::Point face_point;
        this->compute_face_point(face_point, *fit);

        // TODO edge points

        // TODO update
    }
}

void SubdivCatmull::compute_face_point(surface_mesh::Point& face_point, const surface_mesh::Surface_mesh::Face& face) {
    // init result with zero
    face_point = surface_mesh::Point(0);
    // get (pre-defined) property storing vertex positions
    surface_mesh::Surface_mesh::Vertex_property<surface_mesh::Point> v_points = mesh_.get_vertex_property<surface_mesh::Point>(kSurfMeshPropVertexPoint);
    // declare and initialize circulators
    surface_mesh::Surface_mesh::Vertex_around_face_circulator vc, vc_end;
    vc = mesh_.vertices(face);
    vc_end = vc;
    int i = 0;
    // loop over all incident vertices
    do {
        face_point += v_points[*vc];
        ++i;
    } while (++vc != vc_end);
    if (i != 0)
        face_point /= i;
    // add new face point to mesh
    surface_mesh::Surface_mesh::Face_property<surface_mesh::Point> f_points = mesh_.get_face_property<surface_mesh::Point>(kSurfMeshPropFacePoint);
    f_points[face] = face_point;
}

void SubdivCatmull::compute_edge_point(surface_mesh::Point& edge_point, const surface_mesh::Surface_mesh::Edge& edge) {
    // init result with zero
    edge_point = surface_mesh::Point(0);
    // TODO


}

} // namespace Algo
} // namespace SubVis
