/**
 * @class SdQuad "plugins/subdivision/sd_quad.h"
 *
 * @brief Base class for all quad subdivision algorithms.
 *
 * This class bunches methods that are used by subdivision algorithms that operate
 * mainly on quad meshes like Doo Sabin and Catmull-Clark.
 * It provides methods to initializes all necessary properties.
 *
 * @author Felix Born
 *
 */

#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_QUAD_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_QUAD_H

#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {


class SdQuad : public SdAlgorithm {
 public:
  bool is_subdividable(const Surface_mesh& mesh) const override;

 protected:
  void subdivide_input_mesh_write_output_mesh() override = 0;
  void init_mesh_members() override;
  void deinit_mesh_members() override;


  /// Property key to store the coordinate of the edge points.
  const ::std::string kPropEdgePoint = "e:point";

  /// Property key to store the coordinate of the face points.
  const ::std::string kPropFacePoint = "f:point";

  /// Property key to store the coordinate of the updated vertex points.
  const ::std::string kPropVertexPointUpdated = "v:point_updated";


  /// Property with the key @c kPropEdgePoint.
  /// Use @c init_mesh_members() and @c deinit_mesh_members() to initialize and release.
  Surface_mesh::Edge_property<Point> e_points_;

  /// Property with the key @c kPropFacePoint.
  /// Use @c init_mesh_members() and @c deinit_mesh_members() to initialize and release.
  Surface_mesh::Face_property<Point> f_points_;

  /// Property with the key @c kPropVertexPointUpdated.
  /// Use @c init_mesh_members() and @c deinit_mesh_members() to initialize and release.
  Surface_mesh::Vertex_property<Point> v_points_updated_;


  /// Compute the average of all the points of the @c face and store the result in @c face_point.
  void compute_face_point(Point& face_point,
                          const Surface_mesh::Face& face) const;
};


} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_QUAD_H


