
// ===============[ includes ]===============

#include <QDebug>

#include "surface_mesh/Surface_mesh.h"
#include "surface_mesh/IO.h"

#include "plugins/subdivision/sd_catmull.h"
#include "plugins/subdivision/utils.h"

namespace SubdivisionPlugin {

using ::std::string;
using surface_mesh::Surface_mesh;
using surface_mesh::Point;

// ===============[ private prototypes ]===============

// ===============[ public implementation ]===============

void test_catmull() {
    Surface_mesh mesh;
    string path = kRootPathToObjFiles + kObjDemoFilesString[kCube];
    // read mesh and print basic info to stdout
    read_mesh(mesh, path);
    utils_debug_mesh(mesh, QString("Cube Mesh"));
    // compute subdivision
    SubdivCatmull sd_catmull(mesh);
    sd_catmull.subdivide();
    Surface_mesh subdivision_mesh = sd_catmull.get_subdivision_mesh();
    // Do something with the subdivision_mesh
}

// ===============[ class implementation ]===============

void SubdivCatmull::subdivide() {
    Surface_mesh subdivision_mesh;
    // instantiate iterator
    Surface_mesh::Face_iterator fit;
    Surface_mesh::Edge_iterator eit;
    Surface_mesh::Vertex_iterator vit;
    // loop over all faces and compute face points
    for (fit = mesh_.faces_begin(); fit != mesh_.faces_end(); ++fit) {
        this->compute_face_point(*fit);
        v_index_sub_mesh_f_prop_[*fit] = subdivision_mesh.add_vertex(f_points_[*fit]);
    }
    // loop over all edges and compute edge points
    for (eit = mesh_.edges_begin(); eit != mesh_.edges_end(); ++eit) {
        this->compute_edge_point(*eit);
        v_index_sub_mesh_e_prop_[*eit] = subdivision_mesh.add_vertex(e_points_[*eit]);
    }
    // update all vertices
    for (vit = mesh_.vertices_begin(); vit != mesh_.vertices_end(); ++vit) {
        this->compute_new_vertex_point(*vit);
        v_index_sub_mesh_v_prop_[*vit] = subdivision_mesh.add_vertex(v_points_updated_[*vit]);
    }
    // create faces in new mesh
    for (fit = mesh_.faces_begin(); fit != mesh_.faces_end(); ++fit) {
        this->compute_new_faces(subdivision_mesh, *fit);
    }
    // copy result to mesh attribute
    mesh_ = subdivision_mesh;
    utils_debug_mesh(subdivision_mesh, "Subdivision Mesh");
}

void SubdivCatmull::subdivide(unsigned char steps) {
    // TODO implement steps
    for (unsigned char i = 0; i < steps; ++i) {
        this->subdivide();
        // update properties
        this->add_mesh_properties(mesh_);
        this->init_mesh_members();
    }
}

void SubdivCatmull::compute_face_point(const Surface_mesh::Face& face) {
    // init result with zero
    Point face_point = Point(0);
    // declare and initialize circulators
    Surface_mesh::Vertex_around_face_circulator vc, vc_end;
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
    utils_debug_point(face_point, "Face Point");
}

void SubdivCatmull::compute_edge_point(const Surface_mesh::Edge& edge) {
    // init result with zero
    Point edge_point = Point(0);
    // get properties
     // get all coordinates
    Surface_mesh::Vertex edge_vertex0, edge_vertex1;
    Surface_mesh::Face edge_face0, edge_face1;
    edge_vertex0 = mesh_.vertex(edge, 0);
    edge_vertex1 = mesh_.vertex(edge, 1);
    edge_face0 = mesh_.face(edge, 0);
    edge_face1 = mesh_.face(edge, 1);
    // compute edge point
    edge_point = v_points_[edge_vertex0] + v_points_[edge_vertex1] + f_points_[edge_face0] + f_points_[edge_face1];
    edge_point /= 4;
    // store edge_point as property
    e_points_[edge] = edge_point;
    utils_debug_point(edge_point, "Edge Point");
}

void SubdivCatmull::compute_new_vertex_point(const Surface_mesh::Vertex& vertex) {
    // compute new vertex point: (Q/n) + (2R/n) + (S(n-3)/n)
    Point new_vertex_point, q, r, s;
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
    utils_debug_point(new_vertex_point, "New Vertex Point");
}

void SubdivCatmull::compute_new_faces(Surface_mesh& result_mesh, const Surface_mesh::Face& face) {
    // init halfedge circulator
    Surface_mesh::Halfedge_around_face_circulator hc, hc_end;
    hc = mesh_.halfedges(face);
    hc_end = hc;
    // add vertices to new mesh
    const int kArraySize = 4;
    Surface_mesh::Vertex v_index_list[kArraySize];
    Surface_mesh::Vertex e_index_list[kArraySize];
    int i = 0;
    do {
        if (i < kArraySize) { // check if in array bounds
            v_index_list[i] = v_index_sub_mesh_v_prop_[mesh_.from_vertex(*hc)];
            e_index_list[i] = v_index_sub_mesh_e_prop_[mesh_.edge(*hc)];
        }
        ++i; // increment to compute face valence
    } while (++hc != hc_end);
    const Surface_mesh::Vertex f_index = v_index_sub_mesh_f_prop_[face];
    if (i == 3) { // triangle face
        result_mesh.add_quad(v_index_list[0], e_index_list[0], f_index, e_index_list[2]);
        result_mesh.add_quad(v_index_list[1], e_index_list[1], f_index, e_index_list[0]);
        result_mesh.add_quad(v_index_list[2], e_index_list[2], f_index, e_index_list[1]);
    } else if (i == 4) { // quad face
        result_mesh.add_quad(v_index_list[0], e_index_list[0], f_index, e_index_list[3]);
        result_mesh.add_quad(v_index_list[1], e_index_list[1], f_index, e_index_list[0]);
        result_mesh.add_quad(v_index_list[2], e_index_list[2], f_index, e_index_list[1]);
        result_mesh.add_quad(v_index_list[3], e_index_list[3], f_index, e_index_list[2]);
    } else { // error
        throw new string("Invalid mesh topology: " + i);
    }
    // TODO
}

void SubdivCatmull::avg_face_points(Point& avg_face_points, const Surface_mesh::Vertex& vertex) {
    avg_face_points = Point(0);
    Surface_mesh::Face_around_vertex_circulator fc, fc_end;
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

void SubdivCatmull::avg_mid_edges(Point& avg_mid_edges, const Surface_mesh::Vertex& vertex) {
    avg_mid_edges = Point(0);
    Surface_mesh::Halfedge_around_vertex_circulator hc, hc_end;
    hc = mesh_.halfedges(vertex);
    hc_end = hc;
    int i = 0;
    Point mid_edge;
    do {
        this->mid_edge(mid_edge, mesh_.edge(*hc));
        avg_mid_edges += mid_edge;
        ++i;
    } while(++hc != hc_end);
    if (i != 0)
        avg_mid_edges /= i;
}

void SubdivCatmull::mid_edge(Point& mid_edge, const Surface_mesh::Edge& edge) {
    Surface_mesh::Vertex edge_vertex0, edge_vertex1;
    edge_vertex0 = mesh_.vertex(edge, 0);
    edge_vertex1 = mesh_.vertex(edge, 1);
    mid_edge = v_points_[edge_vertex0] + v_points_[edge_vertex1];
    mid_edge /= 2;
}

} // namespace SubdivisionPlugin
