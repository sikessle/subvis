
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
    for (fit = mesh_.faces_begin(); fit != mesh_.faces_end(); ++fit) {
        this->compute_face_point(*fit);
        // loop over all edges of the face *fit
        surface_mesh::Surface_mesh::Halfedge_around_face_circulator hc, hc_end;
        hc = mesh_.halfedges(*fit);
        hc_end = hc;
        do {
            // TODO check if edge point already computed
            this->compute_edge_point(mesh_.edge(*hc));
        } while(++hc != hc_end);
    }
    // update all vertices
    surface_mesh::Surface_mesh::Vertex_iterator vit;
    for (vit = mesh_.vertices_begin(); vit != mesh_.vertices_end(); ++vit) {
        this->compute_new_vertex_point(*vit);
    }

    // TODO replace faces with new faces (quad and triangle)
    for (fit = mesh_.faces_begin(); fit != mesh_.faces_end(); ++fit) {

    }
}

void SubdivCatmull::compute_face_point(const surface_mesh::Surface_mesh::Face& face) {
    // init result with zero
    surface_mesh::Point face_point = surface_mesh::Point(0);
    // declare and initialize circulators
    surface_mesh::Surface_mesh::Vertex_around_face_circulator vc, vc_end;
    vc = mesh_.vertices(face);
    vc_end = vc;
    int i = 0;
    // loop over all incident vertices
    do {
        face_point += v_points_[*vc];
        ++i;
    } while (++vc != vc_end);
    if (i != 0)
        face_point /= i;
    // add new face point to mesh
    f_points_[face] = face_point;
    f_points_is_set_[face] = true;
}

void SubdivCatmull::compute_edge_point(const surface_mesh::Surface_mesh::Edge& edge) {
    // init result with zero
    surface_mesh::Point edge_point = surface_mesh::Point(0);
    // get properties
     // get all coordinates
    surface_mesh::Surface_mesh::Vertex edge_vertex0, edge_vertex1;
    surface_mesh::Surface_mesh::Face edge_face0, edge_face1;
    edge_vertex0 = mesh_.vertex(edge, 0);
    edge_vertex1 = mesh_.vertex(edge, 1);
    edge_face0 = mesh_.face(edge, 0);
    edge_face1 = mesh_.face(edge, 1);
    // check if face point is not computed yet
    if (!f_points_is_set_[edge_face0])
        this->compute_face_point(edge_face0);
    if (!f_points_is_set_[edge_face1])
        this->compute_face_point(edge_face1);
    // compute edge point
    edge_point = v_points_[edge_vertex0] + v_points_[edge_vertex1] + f_points_[edge_face0] + f_points_[edge_face1];
    edge_point /= 4;
    // store edge_point as property
    e_points_[edge] = edge_point;
    e_points_is_set_[edge] = true;
}

void SubdivCatmull::compute_new_vertex_point(const surface_mesh::Surface_mesh::Vertex& vertex) {
    // compute new vertex point: (Q/n) + (2R/n) + (S(n-3)/n)
    surface_mesh::Point new_vertex_point, q, r, s;
    unsigned int vertex_valence = mesh_.valence(vertex);
    // average of the surrounding face points
    this->avg_face_points(q, vertex);
    // average of all surround edge midpoints
    this->avg_mid_edges(r, vertex);
    // old control point
    s = v_points_[vertex];
    // compute new vertex point
    new_vertex_point = (q/vertex_valence) + (2*r/vertex_valence) + (s*(vertex_valence-3)/vertex_valence);
    // store result in kSurfMeshPropVertexPointUpdated
    v_points_updated_[vertex] = new_vertex_point;
}

void SubdivCatmull::compute_new_faces(surface_mesh::Surface_mesh& result_mesh, const surface_mesh::Surface_mesh::Face& face) {
    surface_mesh::Surface_mesh::Vertex_around_face_circulator vc, vc_end;
    vc = mesh_.vertices(face);
    vc_end = vc;

    int i = 0;
    do {
        // TODO create new faces
        ++i;
    } while (++vc != vc_end);
    if (i == 3) { // triangle face
        ;
    } else if (i == 4) { // quad face
        ;
    } else { // error
        return;
    }
    // TODO
}

void SubdivCatmull::avg_face_points(surface_mesh::Point& avg_face_points, const surface_mesh::Surface_mesh::Vertex& vertex) {
    avg_face_points = surface_mesh::Point(0);
    surface_mesh::Surface_mesh::Face_around_vertex_circulator fc, fc_end;
    fc = mesh_.faces(vertex);
    fc_end = fc;
    int i = 0;
    do {
        avg_face_points += f_points_[*fc];
        ++i;
    } while (++fc != fc_end);
    if (i != 0)
        avg_face_points /= i;
}

void SubdivCatmull::avg_mid_edges(surface_mesh::Point& avg_mid_edges, const surface_mesh::Surface_mesh::Vertex& vertex) {
    avg_mid_edges = surface_mesh::Point(0);
    surface_mesh::Surface_mesh::Halfedge_around_vertex_circulator hc, hc_end;
    hc = mesh_.halfedges(vertex);
    hc_end = hc;
    int i = 0;
    surface_mesh::Point mid_edge;
    do {
        this->mid_edge(mid_edge, mesh_.edge(*hc));
        avg_mid_edges += mid_edge;
        ++i;
    } while(++hc != hc_end);
    if (i != 0)
        avg_mid_edges /= i;
}

void SubdivCatmull::mid_edge(surface_mesh::Point& mid_edge, const surface_mesh::Surface_mesh::Edge& edge) {
    surface_mesh::Surface_mesh::Vertex edge_vertex0, edge_vertex1;
    edge_vertex0 = mesh_.vertex(edge, 0);
    edge_vertex1 = mesh_.vertex(edge, 1);
    mid_edge = v_points_[edge_vertex0] + v_points_[edge_vertex1];
    mid_edge /= 2;
}

} // namespace Algo
} // namespace SubVis
