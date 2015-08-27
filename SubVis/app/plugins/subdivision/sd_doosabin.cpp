#include "plugins/subdivision/sd_doosabin.h"
#include "plugins/subdivision/utils.h"

namespace SubdivisionPlugin {

const QString SubdivDooSabin::id()
{
    return "de.htwg.ios.subvis.plugin.subdivision.doosabin";
}

const QString SubdivDooSabin::name()
{
    return "Doo-Sabin";
}

void SubdivDooSabin::subdivide_specific_algorithm()
{
    // read from input_mesh_
    // write to result_mesh_
    this->add_mesh_properties();
    this->init_mesh_members();

    // loop over all faces and compute face points (same as catmull clark face points)
    this->compute_all_face_points();
    // loop over all edges and compute edge points (different to catmull clark edge points!)
    this->compute_all_edge_points();
    // compute new vertex point


    this->remove_mesh_properties();
}

void SubdivDooSabin::compute_all_face_points()
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

void SubdivDooSabin::compute_all_edge_points()
{
    Surface_mesh::Edge_iterator eit;
    Point edge_point;
    for (eit = input_mesh_->edges_begin(); eit != input_mesh_->edges_end(); ++eit) {
        this->mid_edge(edge_point, *eit);
        // store edge_point as property
        e_points_[*eit] = edge_point;
        utils_debug_point(edge_point, "Edge Point");
        v_index_sub_mesh_e_prop_[*eit] = result_mesh_->add_vertex(e_points_[*eit]);
    }
}

void SubdivDooSabin::compute_new_vertex_point(Point& new_vertex_point, const Surface_mesh::Vertex& vertex, const Surface_mesh::Face& face)
{
    // compute new vertex point: average of four points (face point, two edge points and vertex)
    Surface_mesh::Halfedge_around_face_circulator hc, hc_end;
    hc = input_mesh_->halfedges(face);
    hc_end = hc;
    do {
        if (input_mesh_->from_vertex(*hc) == vertex)
            break;
    } while (++hc != hc_end);
    Surface_mesh::Edge e1, e2;
    e1 = input_mesh_->edge(*hc);
    e2 = input_mesh_->edge(input_mesh_->prev_halfedge(*hc));
    new_vertex_point = (v_points_[vertex] + f_points_[face] + e_points_[e1] + e_points_[e2] ) / 4;
}

} // namespace SubVis
