
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_triangle.h"

namespace subdivision {

void SdTriangle::init_mesh_members() {
  SdAlgorithm::init_mesh_members();
  input_mesh_->add_vertex_property<Surface_mesh::Vertex>
  (kPropVertexIndexOutputV);
  input_mesh_->add_edge_property<Surface_mesh::Vertex>
  (kPropVertexIndexOutputE);
  v_index_output_v_prop_ =
    input_mesh_->get_vertex_property<Surface_mesh::Vertex>(kPropVertexIndexOutputV);
  v_index_output_e_prop_ = input_mesh_->get_edge_property<Surface_mesh::Vertex>
                           (kPropVertexIndexOutputE);
}

void SdTriangle::deinit_mesh_members() {
  SdAlgorithm::deinit_mesh_members();
  input_mesh_->remove_edge_property(v_index_output_e_prop_);
  input_mesh_->remove_vertex_property(v_index_output_v_prop_);
}

void SdTriangle::add_all_faces_output_mesh() {
  for (const auto& face : input_mesh_->faces()) {
    this->add_splitted_face_output_mesh(face);
  }
}

void SdTriangle::add_splitted_face_output_mesh(const Surface_mesh::Face&
    face) {
  const int kArraySize = 3;
  Surface_mesh::Vertex v_index_list[kArraySize];
  Surface_mesh::Vertex e_index_list[kArraySize];
  unsigned int i = 0;
  for (const auto& halfedge : input_mesh_->halfedges(face)) {
    if (i < kArraySize) { // check if in array bounds
      v_index_list[i] = v_index_output_v_prop_[input_mesh_->from_vertex(halfedge)];
      e_index_list[i] = v_index_output_e_prop_[input_mesh_->edge(halfedge)];
    }
    ++i;
  }
  result_mesh_->add_triangle(e_index_list[0], e_index_list[1], e_index_list[2]);
  result_mesh_->add_triangle(v_index_list[0], e_index_list[0], e_index_list[2]);
  result_mesh_->add_triangle(v_index_list[1], e_index_list[1], e_index_list[0]);
  result_mesh_->add_triangle(v_index_list[2], e_index_list[2], e_index_list[1]);
}

surface_mesh::Surface_mesh::Vertex SdTriangle::get_outer_vertex_triangle(
  const Surface_mesh::Halfedge& halfedge) const {
  return input_mesh_->to_vertex(input_mesh_->next_halfedge(halfedge));
}

} // namespace subdivision

