
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
        this->compute_face_point(*fit);
        // loop over all edges of the face *fit
        surface_mesh::Surface_mesh::Halfedge_around_face_circulator hc, hc_end;
        hc = mesh_.halfedges(*fit);
        hc_end = hc;
        do {
            this->compute_edge_point(mesh_.edge(*hc));
        } while(++hc != hc_end);

        // TODO update vertices
    }
}

void SubdivCatmull::compute_face_point(const surface_mesh::Surface_mesh::Face& face) {
    // init result with zero
    surface_mesh::Point face_point = surface_mesh::Point(0);
    // get (pre-defined) property storing vertex positions
    surface_mesh::Surface_mesh::Vertex_property<surface_mesh::Point> v_points = mesh_.get_vertex_property<surface_mesh::Point>(kSurfMeshPropVertexPoint);
    surface_mesh::Surface_mesh::Face_property<surface_mesh::Point> f_points = mesh_.get_face_property<surface_mesh::Point>(kSurfMeshPropFacePoint);
    surface_mesh::Surface_mesh::Face_property<bool> f_points_is_set = mesh_.get_face_property<bool>(kSurfMeshPropIsFacePointSet);
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
    f_points_is_set[face] = true;
}

void SubdivCatmull::compute_edge_point(const surface_mesh::Surface_mesh::Edge& edge) {
    // init result with zero
    surface_mesh::Point edge_point = surface_mesh::Point(0);
    // get properties
    surface_mesh::Surface_mesh::Vertex_property<surface_mesh::Point> v_points = mesh_.get_vertex_property<surface_mesh::Point>(kSurfMeshPropVertexPoint);
    surface_mesh::Surface_mesh::Face_property<surface_mesh::Point> f_points = mesh_.get_face_property<surface_mesh::Point>(kSurfMeshPropFacePoint);
    surface_mesh::Surface_mesh::Edge_property<surface_mesh::Point> e_points = mesh_.get_edge_property<surface_mesh::Point>(kSurfMeshPropEdgePoint);
    surface_mesh::Surface_mesh::Face_property<bool> f_points_is_set = mesh_.get_face_property<bool>(kSurfMeshPropIsFacePointSet);
    surface_mesh::Surface_mesh::Edge_property<bool> e_points_is_set = mesh_.get_edge_property<bool>(kSurfMeshPropIsEdgePointSet);
    // get all coordinates
    surface_mesh::Surface_mesh::Vertex edge_vertex0, edge_vertex1;
    surface_mesh::Surface_mesh::Face edge_face0, edge_face1;
    edge_vertex0 = mesh_.vertex(edge, 0);
    edge_vertex1 = mesh_.vertex(edge, 1);
    edge_face0 = mesh_.face(edge, 0);
    edge_face1 = mesh_.face(edge, 1);
    // check if face point is not computed yet
    if (!f_points_is_set[edge_face0])
        this->compute_face_point(edge_face0);
    if (!f_points_is_set[edge_face1])
        this->compute_face_point(edge_face1);
    // compute edge point
    edge_point += v_points[edge_vertex0] + v_points[edge_vertex1] + f_points[edge_face0] + f_points[edge_face1];
    edge_point /= 4;
    // store edge_point as property
    e_points[edge] = edge_point;
    qDebug() << e_points[edge][0] << e_points[edge][1]<<e_points[edge][2] << endl;
    // TODO why runtime error ???
    //e_points_is_set[edge] = true;
}

} // namespace Algo
} // namespace SubVis
