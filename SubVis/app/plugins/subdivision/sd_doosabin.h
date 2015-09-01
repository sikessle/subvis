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

#include <map>
#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {


class SubdivDooSabin : public SubdivAlgorithm {
 public:
  using Surface_mesh = surface_mesh::Surface_mesh;
  using Point = surface_mesh::Point;

 protected:
  virtual void subdivide_specific_algorithm() override;
  void init_mesh_members() override;

 private:
  using VertexToVertexMap =
    std::map<Surface_mesh::Vertex, Surface_mesh::Vertex>;

  const ::std::string kPropVertexIndexResultMapF =
    "f:subdivision_mesh_vertex_index_map";

  Surface_mesh::Face_property<Point> f_points_;
  Surface_mesh::Edge_property<Point> e_points_;

  /**
   * @brief v_index_vertex_map_ every face has this property to store the new vertices
   * of the subdivision mesh in a map.
   * Key: index of the old Vertex in the input_mesh_
   * Value: index of the new Vertex in the result_mesh_
   */
  Surface_mesh::Face_property<VertexToVertexMap> f_vertex_index_map_;

  void compute_all_face_points();
  void compute_all_edge_points();
  void compute_all_new_vertex_points();

  void compute_new_vertex_point(Point& new_vertex_point,
                                const Surface_mesh::Vertex& vertex, const Surface_mesh::Face& face);

  void add_mesh_properties();

  void remove_mesh_properties();
};


} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_DOO_SABIN_H
