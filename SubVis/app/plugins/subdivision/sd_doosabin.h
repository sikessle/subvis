#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_DOO_SABIN_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_DOO_SABIN_H

#include <map>
#include "plugins/subdivision/sd_quad.h"

namespace subdivision {

/**
 * @brief Implementation of the Doo-Sabin subdivision algorithm.
 *
 * The algorithm was devised by Daniel Doo and Malcolm Sabin.
 * Doo-Sabin subdivision is a generalization of bi-quadratic uniform B-spline surfaces and
 * operates on meshes with arbitrary topology.
 * The subdivided output mesh contains mainly quad faces and polygons in special cases.
 * The control points are approximated.
 *
 * The subdivision scheme:
 *  1. Compute face points for each face (average of all vertices of a face).
 *  2. Compute edge points for each edge (midpoint of the edge).
 *  3. Compute the new vertex point (average of face point, two edge points and vertex).
 *     <pre>(f: face point, e: edge point, v: vertex point, V: new vertex point, F: new Face)
 *      v-------v      +-------+
 *      |       |      | V   V |
 *      |   f   e -->  |       |
 *      |     V |      | V   V |
 *      v---e---v      +-------+</pre>
 *     <pre>Boundary case: add 2 vertices for every edge 3/4--V------1/4 and 1/4------V--3/4</pre>
 *  4. Create the faces with the new vertex points
 *    - for each face:<pre>
 *      v-------v      +-------+
 *      |       |      | V---V |
 *      |   f   e -->  | | F | |   new face can be arbitrary polygon face
 *      |     V |      | V---V |
 *      v---e---v      +-------+</pre>
 *    - for each vertex:<pre>
 *      v-------v-------v     v-------v-------v
 *      |       |       |     |       |       |
 *      |       |       |     |       |       |
 *      |    V  |  V    |     |    V-----V    |
 *      v-------v-------v --> v----|  F  |----v    new face can be arbitrary polygon face
 *      |    V  |  V    |     |    V-----V    |
 *      |       |       |     |       |       |
 *      |       |       |     |       |       |
 *      v-------v-------v     v-------v-------v</pre>
 *    - for each edge (if edge is not a boundary edge):<pre>
 *      +-------v-------+     +-------+-------+
 *      |    V  | V     |     |     V---V     |
 *      |       |       | --> |     | F |     |  new face is always quad face
 *      |    V  | V     |     |     V---V     |
 *      +-------v-------+     +-------+-------+</pre>
 *
 *
 * Sources that helped to implement the algorithm.
 * - <a href="http://yoshihitoyagi.com/projects/mesh/subdiv/doo/index.html">Doo-Sabin Subdivision Surface</a>
 * - <a href="http://www.idav.ucdavis.edu/education/CAGDNotes/Doo-Sabin/Doo-Sabin.html">DOO-SABIN SURFACES</a>
 *
 * @author Felix Born
 *
 */
class SdDooSabin : public SdQuad {
 protected:
  void subdivide_input_mesh_write_output_mesh() override;
  void init_mesh_members() override;
  void deinit_mesh_members() override;

 private:
  using VertexToVertexMap =
    std::map<Surface_mesh::Vertex, Surface_mesh::Vertex>;

  /// Property key to store the index of the corresponding vertex to vertex map of every face.
  const ::std::string kPropVertexIndexOutputMapF =
    "f:vertex_index_map_output_mesh";

  /// Property key to store the index of the corresponding output mesh boundary vertex points.
  const ::std::string kPropVertexIndexOutputBoundaryE =
    "e:vertex_index_output_mesh";

  /// The face property with the key @c kPropVertexIndexOutputMapF stores the new vertices in a map with the old vertices as key.
  Surface_mesh::Face_property<VertexToVertexMap> v_index_map_output_f_prop_;

  /// Property with the key @c kPropVertexIndexOutputBoundaryE.
  Surface_mesh::Edge_property<VertexToVertexMap> v_index_map_output_e_prop_;

  /// Loop over all faces of the input mesh and compute all face points.
  /// The face points are stored as property in the input mesh.
  /// @sa compute_face_point(Point& face_point, const Surface_mesh::Face& face);
  void compute_all_face_points();

  /// Loop over all edges of the input mesh and compute all edge points
  /// The edge points are stored as property in the input mesh.
  /// @sa compute_mid_edge(Point& mid_edge, const Surface_mesh::Edge& edge)
  void compute_all_edge_points();

  /// Loop over every vertex of all faces and compute the new vertex points.
  /// The coordinates are stored as property in the @c VertexToVertexMap (@c f_vertex_index_map_).
  /// @sa compute_new_vertex_point(Point& new_vertex_point, const Surface_mesh::Vertex& vertex, const Surface_mesh::Face& face)
  void add_all_new_vertex_points_output_mesh();

  /// Loop over every boundary edges, compute the new vertex point and add it as vertex to the output mesh.
  /// @sa compute_boundary_vertex_point(Point& new_vertex_point, const Surface_mesh::Halfedge& halfedge)
  void add_all_new_boundary_vertex_points_output_mesh();

  /**
   * @brief Compute the new vertex point of a @c vertex that belongs to the @c face.
   *
   * The new vertex point is the average of the face point, two edge points and the vertex.
   * @param[out] new_vertex_point The computed vertex point coordinate.
   * @param[in]  vertex           Compute the new vertex point of the @c vertex.
   * @param[in]  face             The corresponding face to compute the new vertex point. (A vertex can belong to many faces)
   * @attention The @c vertex has to be a vertex of the @c face.
   */
  void compute_new_vertex_point(Point& new_vertex_point,
                                const Surface_mesh::Vertex& vertex, const Surface_mesh::Face& face) const;

  /**
   * @brief Compute the new boundary vertex point of the edge (from vertex of the @c halfedge).
   *
   * @param[out] new_vertex_point The computed vertex point coordinate.
   * @param[in]  halfedge         Compute the new vertex of the from vertex of the @c halfedge.
   */
  void compute_boundary_vertex_point(Point& new_vertex_point,
                                     const Surface_mesh::Halfedge& halfedge) const;

  /// Loop over all faces, edges, and vertieces to create the new faces and add them to the output mesh.
  /// @sa add_all_faces_output_mesh_face(), add_all_faces_output_mesh_edge(), add_all_faces_output_mesh_vertex()
  void add_all_faces_output_mesh();

  /// Loop over all faces, create new faces and add them to the output mesh. Arbitrary polygon faces can be created.
  void add_all_faces_output_mesh_face();

  /// Loop over all edges, create new faces and add them to the output mesh. Always quad faces created.
  void add_all_faces_output_mesh_edge();

  /// Loop over all vertices, create new faces and add them to the output mesh. Arbitrary polygon faces can be created.
  void add_all_faces_output_mesh_vertex();
};


} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_DOO_SABIN_H
