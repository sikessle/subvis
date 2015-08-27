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

    // loop over all faces and compute face points
    this->compute_all_face_points();
    // loop over all edges and compute edge points (different to catmull clark edge points!)
    this->compute_all_edge_points();
    // compute new vertex point

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

} // namespace SubVis
