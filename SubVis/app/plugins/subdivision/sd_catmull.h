/**
 * @class SdCatmull Catmull–Clark subdivision "plugins/subdivision/sd_catmull.h"
 *
 * @brief Implementation of the Catmull–Clark subdivision algorithm.
 *
 * The algorithm was devised by Edwin Catmull and Jim Clark.
 * Catmull–Clark subdivision is a generalization of bi-cubic uniform B-spline surfaces and
 * operates on meshes with arbitrary topology (with quad and triangle faces).
 * The subdivided output mesh contains always quad faces.
 * The control points are approximated.
 *
 * The subdivision scheme:
 *  1. Compute face points for each face (average of all vertices of a face).
 *  2. Compute edge points for each edge (average of the center of the edge and
 *     the center of face points of the two adjacent faces).
 *     Boundary case: edge point is the middle of the edge
 *  3. Update the coordinates of all vertices: /f$(Q/n) + (2R/n) + (S(n-3)/n)/f$
 *    - n: valence
 *    - Q: average of the surrounding face points
 *    - R: average of all surround edge midpoints
 *    - S: old control point
 *      Boundary case: 1/8----3/4----1/8
 *  4. Replace each face by new faces. Ever face is splitted from the face point
 *    - triangle:
 *      +----------+      +----------+
 *       \        /        \   |    /
 *        \      /          \  +   /
 *         \    /  -->       \/ \ /
 *          \  /              +  +
 *           \/                \/
 *           +                  +
 *    - quad:
 *      +-------+      +---+---+
 *      |       |      |   |   |
 *      |       | -->  +---+---+
 *      |       |      |   |   |
 *      +-------+      +---+---+
 *
 * Sources that helped to implement the algorithm.
 * <a href="http://rosettacode.org/wiki/Catmull%E2%80%93Clark_subdivision_surface">Catmull-Clark Subdivision Surface</a>
 * <a href="http://yoshihitoyagi.com/projects/mesh/subdiv/catmull/">Catmull–Clark subdivision surface</a>
 * <a href="http://www.rorydriscoll.com/2008/08/01/catmull-clark-subdivision-the-basics/">Catmull-Clark Subdivision:
 * The Basics </a>
 *
 * @author Felix Born
 *
 */

#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_CATMULL_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_CATMULL_H

#include "plugins/subdivision/sd_quad.h"

namespace subdivision {

class SdCatmull : public SdQuad {
 public:
  bool is_subdividable(const Surface_mesh& mesh) const override;

 protected:
  void subdivide_input_mesh_write_output_mesh() override;
  void init_mesh_members() override;
  void deinit_mesh_members() override;

 private:
  /// Property key to store the index of the corresponding output mesh vertex points.
  const ::std::string kPropVertexIndexOutputV =
    "v:vertex_index_output_mesh";

  /// Property key to store the index of the corresponding output mesh edge points.
  const ::std::string kPropVertexIndexOutputE =
    "e:vertex_index_output_mesh";

  /// Property key to store the index of the corresponding output mesh face points.
  const ::std::string kPropVertexIndexOutputF =
    "f:vertex_index_output_mesh";


  /// Property with the key @c kPropVertexIndexOutputV.
  /// Use @c init_mesh_members() and @c deinit_mesh_members() to initialize and release.
  Surface_mesh::Vertex_property<Surface_mesh::Vertex> v_index_output_v_prop_;

  /// Property with the key @c kPropVertexIndexOutputE.
  /// Use @c init_mesh_members() and @c deinit_mesh_members() to initialize and release.
  Surface_mesh::Edge_property<Surface_mesh::Vertex> v_index_output_e_prop_;

  /// Property with the key @c kPropVertexIndexOutputF.
  /// Use @c init_mesh_members() and @c deinit_mesh_members() to initialize and release.
  Surface_mesh::Face_property<Surface_mesh::Vertex> v_index_output_f_prop_;


  /// Loop over all faces of the input mesh and compute all face points.
  /// The face points are stored as property in the input mesh and added as vertex to the output mesh.
  /// @sa compute_face_point(Point& face_point, const Surface_mesh::Face& face);
  void add_all_face_points_output_mesh();

  /// Loop over all edges of the input mesh and compute all edge points
  /// The edge points are stored as property in the input mesh and added as vertex to the output mesh.
  /// @sa compute_edge_point(Point& edge_point, const Surface_mesh::Edge& edge)
  void add_all_edge_points_output_mesh();

  /// Loop over all vertices and update the coordinates.
  /// The updated vertex points are stored as property in the input mesh and added as vertex to the output mesh.
  /// @sa compute_updated_vertex_point(Point& new_vertex_point, const Surface_mesh::Vertex& vertex)
  void add_all_updated_vertex_points_output_mesh();

  /// Loop over all faces, split them and add the splitted faces to the output mesh.
  /// @sa add_splitted_face_to_output_mesh(const Surface_mesh::Face& face)
  void add_all_faces_output_mesh();

  /**
   * @brief Compute the @c edge_point of the @c edge.
   *
   * An edge point is the the average of the two control points on either side of the edge,
   * and the face-points of the touching faces
   * @param[out] edge_point The computed edge point coordinate.
   * @param[in]  edge       The edge for which the edge point has to be computed.
   * @attention All face points have to be computed and stored as property before usage! (call @c compute_all_face_points())
   */
  void compute_edge_point(Point& edge_point,
                          const Surface_mesh::Edge& edge) const;


  /// Compute the updated vertex point. Choose the appropriate subdivision rule whether it is a boundary vertex or not.
  /// @attention All face points have to be computed and stored as property before usage! (call @c compute_all_face_points())
  /// @sa compute_updated_vertex_point_regular(Point& new_vertex_point, const Surface_mesh::Vertex& vertex),
  /// void compute_updated_vertex_point_boundary(Point& new_vertex_point, const Surface_mesh::Vertex& vertex)
  void compute_updated_vertex_point(Point& new_vertex_point,
                                    const Surface_mesh::Vertex& vertex) const;

  /**
   * @brief Compute the updated coordinate @c new_vertex_point of the @c vertex.
   *
   * The computation of the coordinate: /f$(Q/n) + (2R/n) + (S(n-3)/n)/f$
   *    - n: valence
   *    - Q: average of the surrounding face points
   *    - R: average of all surround edge midpoints
   *    - S: old control point
   * @param[out] new_vertex_point The updated vertex coordinate.
   * @param[in]  vertex           The vertex for which the updated vertex point has to be computed.
   */
  void compute_updated_vertex_point_regular(Point& new_vertex_point,
      const Surface_mesh::Vertex& vertex) const;

  /// Compute the updated coordinate @c new_vertex_point of the @c vertex for boundary cases.
  void compute_updated_vertex_point_boundary(Point& new_vertex_point,
      const Surface_mesh::Vertex& vertex) const;

  /// Split the @c face (quad or triangle face) and add the splitted faces to the output mesh.
  void add_splitted_face_output_mesh(const Surface_mesh::Face& face);

  /// Compute the average of all surrounding face points of the @c vertex and store the output in @c avg_face_points.
  void avg_face_points(Point& avg_face_points,
                       const Surface_mesh::Vertex& vertex) const;

  /// Compute the average of all surrounding edge midpoints of the @c vertex and store the output in @c avg_mid_edges.
  void avg_mid_edges(Point& avg_mid_edges,
                     const Surface_mesh::Vertex& vertex) const;
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_CATMULL_H

