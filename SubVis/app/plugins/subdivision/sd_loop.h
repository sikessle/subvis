/**
 *
 * @brief Implementation of the Loop subdivision algorithm.
 *
 * 1. create edge point for each edge
 * 2. update all vertices of the old mesh
 * 2. create new faces (4 new triangle faces replace the old triangle face)
 * (https://graphics.stanford.edu/~mdfisher/subdivision.html
 *  http://www.cs.cmu.edu/afs/cs/academic/class/15462-s14/www/lec_slides/Subdivision.pdf)
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

  void compute_all_odd_vertices();
  void compute_all_even_vertices();
  void compute_all_faces();

  void compute_odd_vertex(Point& odd_vertex, const Surface_mesh::Edge& edge);
  void compute_even_vertex(Point& even_vertex,
                           const Surface_mesh::Vertex& vertex);
  void compute_new_faces(const Surface_mesh::Face& face);

  /**
   * @brief compute_beta Compute the constant beta as proposed by Warren.
   * for n > 3, beta = 3/(8n) and for n = 3, beta = 3/16
   *
   * [original choice of Loop: 1/n(5/8-(3/8+1/4cos(2pi/n))^2)]
   * @param n
   * @return
   */
  double compute_beta(unsigned int n) const;
};

} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_LOOP_H
