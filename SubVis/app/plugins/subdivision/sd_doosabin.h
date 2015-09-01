/**
 *
 * @brief Implementation of the Doo Sabin subdivision algorithm.
 *
 * 1. create face point for each face
 * 2. create edge point for each edge
 * 3. compute new vertex: average of the four points (face point, two edge points, vertex)
 * (http://yoshihitoyagi.com/projects/mesh/subdiv/doo/index.html)
 *
 *
 */

#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_DOO_SABIN_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_DOO_SABIN_H


#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {


class SubdivDooSabin : public SubdivAlgorithm {
 public:
  using Surface_mesh = surface_mesh::Surface_mesh;
  using Point = surface_mesh::Point;

 protected:
  virtual void subdivide_specific_algorithm() override;

 private:
  Surface_mesh::Face_property<Point> f_points_;
  Surface_mesh::Edge_property<Point> e_points_;

  // vertex index properties to map from origin mesh to subdivision mesh
  Surface_mesh::Face_property<Surface_mesh::Vertex> v_index_sub_mesh_f_prop_;
  Surface_mesh::Edge_property<Surface_mesh::Vertex> v_index_sub_mesh_e_prop_;

  void compute_all_face_points();
  void compute_all_edge_points();
  void compute_all_new_vertex_points();

  void compute_new_vertex_point(Point& new_vertex_point,
                                const Surface_mesh::Vertex& vertex, const Surface_mesh::Face& face);

  void add_mesh_properties();

  void init_mesh_members();

  void remove_mesh_properties();
};


} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_DOO_SABIN_H
