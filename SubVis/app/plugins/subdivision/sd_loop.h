/**
 *
 * @brief Implementation of the Loop subdivision algorithm.
 *
 * 1. create edge point for each edge
 * 2. update all vertices of the old mesh
 * 2. create new faces (4 new triangle faces replace the old triangle face)
 * (https://graphics.stanford.edu/~mdfisher/subdivision.html)
 *
 */


#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_LOOP_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_LOOP_H

#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {

class SubdivLoop : public SubdivAlgorithm {
 protected:
  virtual void subdivide_input_mesh_write_output_mesh() override;
  virtual void init_mesh_members() override;
  virtual void deinit_mesh_members() override;

 private:
  const ::std::string kPropVertexIndexResultV =
    "v:result_mesh_vertex_index";
  const ::std::string kPropVertexIndexResultE =
    "e:result_mesh_vertex_index";
  Surface_mesh::Vertex_property<Surface_mesh::Vertex> v_index_result_v_prop_;
  Surface_mesh::Edge_property<Surface_mesh::Vertex> v_index_result_e_prop_;

  void compute_all_edge_points();

  void compute_edge_point(Point& edge_point, const Surface_mesh::Edge& edge);
};

} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_LOOP_H
