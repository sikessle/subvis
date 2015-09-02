/**
 *
 * @brief Implementation of the catmull-clark subdivision algorithm.
 *
 * 1. create face point for each face
 * 2. create edge point for each edge
 * 3. update vertex point coordinates
 * (http://rosettacode.org/wiki/Catmull%E2%80%93Clark_subdivision_surface)
 *
 * @author Felix Born
 *
 */

#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_CATMULL_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_CATMULL_H

#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {


class SubdivCatmull : public SubdivAlgorithm {
 protected:
  virtual void subdivide_input_mesh_write_output_mesh() override;
  virtual void init_mesh_members() override;
  virtual void deinit_mesh_members() override;

 private:

  /**
   * @brief kSurfMeshPropVertexIndexSubMesh index of the corresponding vertex in the new result mesh
   * This is necessary to map vertices from origin mesh to result mesh.
   */
  const ::std::string kPropVertexIndexResultV =
    "v:subdivision_mesh_vertex_index";
  const ::std::string kPropVertexIndexResultE =
    "e:subdivision_mesh_vertex_index";
  const ::std::string kPropVertexIndexResultF =
    "f:subdivision_mesh_vertex_index";

  // vertex index properties to map from origin mesh to subdivision mesh
  Surface_mesh::Vertex_property<Surface_mesh::Vertex> v_index_result_v_prop_;
  Surface_mesh::Edge_property<Surface_mesh::Vertex> v_index_result_e_prop_;
  Surface_mesh::Face_property<Surface_mesh::Vertex> v_index_result_f_prop_;

  void compute_all_face_points();
  void compute_all_edge_points();
  void compute_all_new_vertex_points();

  /**
   * @brief compute_edge_point An edge point is the the average of the two control points on either side of the edge,
   *                           and the face-points of the touching faces
   *                           - all face points have to be computed before usage!
   */
  void compute_edge_point(Point& edge_point, const Surface_mesh::Edge& edge);

  /**
   * @brief compute_new_vertex_point Compute the new vertex coordinates: (Q/n) + (2R/n) + (S(n-3)/n)
   *  n - valence
   *  Q - average of the surrounding face points
   *  R - average of all surround edge midpoints
   *  S - old control point
   * @param vertex old vertex
   */
  void compute_new_vertex_point(Point& new_vertex_point,
                                const Surface_mesh::Vertex& vertex);

  void compute_new_faces(const Surface_mesh::Face& face);

  void avg_face_points(Point& avg_face_points,
                       const Surface_mesh::Vertex& vertex);

  void avg_mid_edges(Point& avg_mid_edges, const Surface_mesh::Vertex& vertex);
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_CATMULL_H

