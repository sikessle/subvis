
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
        // TODO
        surface_mesh::Point face_point;
        this->compute_face_point(face_point, *fit);
    }

    // TODO edge points
    //this->compute_edge_point();
    // TODO update
}

void SubdivCatmull::compute_face_point(surface_mesh::Point& face_point, const surface_mesh::Surface_mesh::Face& face) {
    // init result with zero
    face_point = surface_mesh::Point(0);
    // get (pre-defined) property storing vertex positions
    surface_mesh::Surface_mesh::Vertex_property<surface_mesh::Point> v_points = mesh_.get_vertex_property<surface_mesh::Point>(kSurfMeshPropVertexPoint);
    surface_mesh::Surface_mesh::Face_property<surface_mesh::Point> f_points = mesh_.get_face_property<surface_mesh::Point>(kSurfMeshPropFacePoint);
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
    f_points[face] = face_point;
}

void SubdivCatmull::compute_edge_point(surface_mesh::Point& edge_point, const surface_mesh::Surface_mesh::Edge& edge) {
    // init result with zero
    edge_point = surface_mesh::Point(0);
    // get properties
    surface_mesh::Surface_mesh::Vertex_property<surface_mesh::Point> v_points = mesh_.get_vertex_property<surface_mesh::Point>(kSurfMeshPropVertexPoint);
    surface_mesh::Surface_mesh::Face_property<surface_mesh::Point> f_points = mesh_.get_face_property<surface_mesh::Point>(kSurfMeshPropFacePoint);
    surface_mesh::Surface_mesh::Edge_property<surface_mesh::Point> e_points = mesh_.get_edge_property<surface_mesh::Point>(kSurfMeshPropEdgePoint);
    // get all coordinates
    surface_mesh::Surface_mesh::Vertex edge_vertex0, edge_vertex1;
    surface_mesh::Surface_mesh::Face edge_face0, edge_face1;
    edge_vertex0 = mesh_.vertex(edge, 0);
    edge_vertex1 = mesh_.vertex(edge, 1);
    edge_face0 = mesh_.face(edge, 0);
    edge_face1 = mesh_.face(edge, 1);
    edge_point += v_points[edge_vertex0] + v_points[edge_vertex1] + f_points[edge_face0] + f_points[edge_face1];
    edge_point /= 4;
    // store edge_point as property
    e_points[edge] = edge_point;
}

} // namespace Algo
} // namespace SubVis
