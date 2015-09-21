/**
 *
 * @brief Base class for all triangle subdivision algorithms
 *
 *
 */


#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_TRIANGLE_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_TRIANGLE_H

#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {


class SdTriangle : public SdAlgorithm {
 protected:
  void subdivide_input_mesh_write_output_mesh() override = 0;
  void init_mesh_members() override;
  void deinit_mesh_members() override;

  const ::std::string kPropVertexIndexResultV =
    "v:result_mesh_vertex_index";
  const ::std::string kPropVertexIndexResultE =
    "e:result_mesh_vertex_index";
  Surface_mesh::Vertex_property<Surface_mesh::Vertex> v_index_result_v_prop_;
  Surface_mesh::Edge_property<Surface_mesh::Vertex> v_index_result_e_prop_;

  void compute_all_faces();

  surface_mesh::Surface_mesh::Vertex get_outer_vertex_triangle(
    const Surface_mesh::Halfedge& halfedge) const;

  void add_splitted_face_to_result_mesh(const Surface_mesh::Face& face);

};


} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_TRIANGLE_H


