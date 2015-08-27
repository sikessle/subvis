
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
    //SubdivCatmull sd_catmull(mesh);
    //sd_catmull.subdivide();
    //Surface_mesh subdivision_mesh = sd_catmull.get_subdivision_mesh();
    // Do something with the subdivision_mesh
}

// ===============[ class implementation ]===============

const QString SubdivCatmull::id()
{
    return "de.htwg.ios.subvis.plugin.subdivision.catmullclark";
}

const QString SubdivCatmull::name()
{
    return "Catmull-Clark";
}

void SubdivCatmull::subdivide_specific_algorithm()
{
    this->add_mesh_properties();
    this->init_mesh_members();

    // loop over all faces and compute face points
    this->compute_all_face_points();
    // loop over all edges and compute edge points
    this->compute_all_edge_points();
    // update all vertices
    this->compute_all_new_vertex_points();
    // create faces in new mesh
    Surface_mesh::Face_iterator fit;
    for (fit = input_mesh_->faces_begin(); fit != input_mesh_->faces_end(); ++fit) {
        this->compute_new_faces(*fit);
    }
    utils_debug_mesh(*(result_mesh_.get()), "Subdivision Mesh");
    remove_mesh_properties();
}

void SubdivCatmull::compute_all_face_points()
{
    Surface_mesh::Face_iterator fit;
    Point face_point;
    for (fit = input_mesh_->faces_begin(); fit != input_mesh_->faces_end(); ++fit) {
        this->compute_face_point(face_point, *fit);
        // add new face point to mesh
        f_points_[*fit] = face_point;
        utils_debug_point(face_point, "Face Point");
        v_index_sub_mesh_f_prop_[*fit] = result_mesh_->add_vertex(f_points_[*fit]);
    }
}

void SubdivCatmull::compute_all_edge_points()
{
    Surface_mesh::Edge_iterator eit;
    Point edge_point;
    for (eit = input_mesh_->edges_begin(); eit != input_mesh_->edges_end(); ++eit) {
        this->compute_edge_point(edge_point, *eit);
        // store edge_point as property
        e_points_[*eit] = edge_point;
        utils_debug_point(edge_point, "Edge Point");
        v_index_sub_mesh_e_prop_[*eit] = result_mesh_->add_vertex(e_points_[*eit]);
    }
}

void SubdivCatmull::compute_all_new_vertex_points()
{
    Surface_mesh::Vertex_iterator vit;
    Point new_vertex_point;
    for (vit = input_mesh_->vertices_begin(); vit != input_mesh_->vertices_end(); ++vit) {
        this->compute_new_vertex_point(new_vertex_point, *vit);
        // store result in kSurfMeshPropVertexPointUpdated
        v_points_updated_[*vit] = new_vertex_point;
        utils_debug_point(new_vertex_point, "New Vertex Point");
        v_index_sub_mesh_v_prop_[*vit] = result_mesh_->add_vertex(v_points_updated_[*vit]);
    }
}

void SubdivCatmull::compute_edge_point(Point& edge_point, const Surface_mesh::Edge& edge)
{
    // init result with zero
    edge_point = Point(0);
    // get properties
     // get all coordinates
    Surface_mesh::Vertex edge_vertex0, edge_vertex1;
    Surface_mesh::Face edge_face0, edge_face1;
    edge_vertex0 = input_mesh_->vertex(edge, 0);
    edge_vertex1 = input_mesh_->vertex(edge, 1);
    edge_face0 = input_mesh_->face(edge, 0);
    edge_face1 = input_mesh_->face(edge, 1);
    // compute edge point
    edge_point = v_points_[edge_vertex0] + v_points_[edge_vertex1] + f_points_[edge_face0] + f_points_[edge_face1];
    edge_point /= 4;
}

void SubdivCatmull::compute_new_vertex_point(Point& new_vertex_point, const Surface_mesh::Vertex& vertex)
{
    // compute new vertex point: (Q/n) + (2R/n) + (S(n-3)/n)
    Point q, r, s;
    unsigned int vertex_valence = input_mesh_->valence(vertex);
    // average of the surrounding face points
    this->avg_face_points(q, vertex);
    // average of all surround edge midpoints
    this->avg_mid_edges(r, vertex);
    // old control point
    s = v_points_[vertex];
    // compute new vertex point
    new_vertex_point = (q/vertex_valence) + (2*r/vertex_valence) + (s*(vertex_valence-3)/vertex_valence);
}

void SubdivCatmull::compute_new_faces(const Surface_mesh::Face& face)
{
    // init halfedge circulator
    Surface_mesh::Halfedge_around_face_circulator hc, hc_end;
    hc = input_mesh_->halfedges(face);
    hc_end = hc;
    // add vertices to new mesh
    const int kArraySize = 4;
    Surface_mesh::Vertex v_index_list[kArraySize];
    Surface_mesh::Vertex e_index_list[kArraySize];
    int i = 0;
    do {
        if (i < kArraySize) { // check if in array bounds
            v_index_list[i] = v_index_sub_mesh_v_prop_[input_mesh_->from_vertex(*hc)];
            e_index_list[i] = v_index_sub_mesh_e_prop_[input_mesh_->edge(*hc)];
        }
        ++i; // increment to compute face valence
    } while (++hc != hc_end);
    const Surface_mesh::Vertex f_index = v_index_sub_mesh_f_prop_[face];
    if (i == 3) { // triangle face
        result_mesh_->add_quad(v_index_list[0], e_index_list[0], f_index, e_index_list[2]);
        result_mesh_->add_quad(v_index_list[1], e_index_list[1], f_index, e_index_list[0]);
        result_mesh_->add_quad(v_index_list[2], e_index_list[2], f_index, e_index_list[1]);
    } else if (i == 4) { // quad face
        result_mesh_->add_quad(v_index_list[0], e_index_list[0], f_index, e_index_list[3]);
        result_mesh_->add_quad(v_index_list[1], e_index_list[1], f_index, e_index_list[0]);
        result_mesh_->add_quad(v_index_list[2], e_index_list[2], f_index, e_index_list[1]);
        result_mesh_->add_quad(v_index_list[3], e_index_list[3], f_index, e_index_list[2]);
    } else { // error
        throw new string("Invalid mesh topology: " + i);
    }
    // TODO
}

void SubdivCatmull::avg_face_points(Point& avg_face_points, const Surface_mesh::Vertex& vertex)
{
    avg_face_points = Point(0);
    Surface_mesh::Face_around_vertex_circulator fc, fc_end;
    fc = input_mesh_->faces(vertex);
    fc_end = fc;
    int i = 0;
    do {
        avg_face_points += f_points_[*fc];
        ++i;
    } while (++fc != fc_end);
    if (i != 0)
        avg_face_points /= i;
}

void SubdivCatmull::avg_mid_edges(Point& avg_mid_edges, const Surface_mesh::Vertex& vertex)
{
    avg_mid_edges = Point(0);
    Surface_mesh::Halfedge_around_vertex_circulator hc, hc_end;
    hc = input_mesh_->halfedges(vertex);
    hc_end = hc;
    int i = 0;
    Point mid_edge;
    do {
        this->mid_edge(mid_edge, input_mesh_->edge(*hc));
        avg_mid_edges += mid_edge;
        ++i;
    } while(++hc != hc_end);
    if (i != 0)
        avg_mid_edges /= i;
}

} // namespace SubdivisionPlugin
