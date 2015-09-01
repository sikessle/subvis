#ifndef SUBVIS_PLUGINS_SUBDIVISION_SUBDIV_ALGORITHM_H
#define SUBVIS_PLUGINS_SUBDIVISION_SUBDIV_ALGORITHM_H

#include <memory>
#include <string>
#include <QString>
#include "surface_mesh/Surface_mesh.h"

namespace subdivision {

class SubdivAlgorithm {
 public:
  using Surface_mesh = surface_mesh::Surface_mesh;
  using Point = surface_mesh::Point;

  virtual ~SubdivAlgorithm();

  /**
   * @brief Subdivides the given mesh_data object in n steps.
   * @param mesh_data Will not be modified but copied.
   * @param steps The number of subdivision steps.
   * @return The resulting mesh including ownership.
   */
  virtual std::unique_ptr<Surface_mesh> subdivide(Surface_mesh& mesh_data,
      int steps = 1);

  /**
   * @brief Returns result mesh for testing.
   * @return A view on the mesh, but ownership stays in this class.
   */
  virtual Surface_mesh& result_mesh();

 protected:
  /**
   * @brief kPropVertexPoint key to access surface mesh property that stores the vertex coordinates
   */
  const ::std::string kPropVertexPoint = "v:point";

  /**
   * @brief kPropVertexPointUpdated key to access surface mesh property that stores the updated vertex coordinates
   */
  const ::std::string kPropVertexPointUpdated = "v:point_updated";

  /**
   * @brief kPropFacePoint key to access surface mesh property that stores the face point coordinates
   */
  const ::std::string kPropFacePoint = "f:point";

  /**
   * @brief kPropEdgePoint key to access surface mesh property that stores the edge point coordinates
   */
  const ::std::string kPropEdgePoint = "e:point";

  /**
   * @brief Put the resulting mesh in here.
   */
  std::unique_ptr<Surface_mesh> result_mesh_;
  /**
   * @brief Get the input mesh from here. The mesh may be modified freely,
   * as this is only a copy of the original mesh (i.e. if special properties
   * are needed).
   */
  std::unique_ptr<Surface_mesh> input_mesh_;

  Surface_mesh::Vertex_property<Point> v_points_;
  Surface_mesh::Edge_property<Point> e_points_;
  Surface_mesh::Face_property<Point> f_points_;
  Surface_mesh::Vertex_property<Point> v_points_updated_;


  /**
   * @brief Writes the result of modifiyng the input_mesh_ to result_mesh_
   *
   * The input_mesh_ and result_mesh_ smart pointers must be used to read
   * from the original mesh and write the result.
   * This function is the initial starting point of the algorithm and the only
   * point in time where sub-classes may interact with the input/result mesh
   * pointers. After and before calling this method there is no guarantee about
   * the state of the input/result mesh pointers.
   * There is also no need to destruct or clean up the two pointers as they are
   * smart pointers.
   */
  virtual void subdivide_specific_algorithm() = 0;

  /**
   * @brief init_mesh_members Allocate memory for surface mesh properties and init the class members.
   */
  virtual void init_mesh_members();
  /**
   * @brief deinit_mesh_members Release allocated memory of init_mesh_members().
   */
  virtual void deinit_mesh_members();

  /**
   * @brief compute_mid_edge Compute the mid point of an edge (average of the two vertices).
   *                 The edge has to be in the input_mesh_.
   * @param mid_edge The result (mid of edge).
   * @param edge Edge to compute the mid point.
   */
  void compute_mid_edge(Point& mid_edge, const Surface_mesh::Edge& edge);

  /**
   * @brief compute_face_point Compute the average of all the points of the face.
   */
  void compute_face_point(Point& face_point, const Surface_mesh::Face& face);

};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_SUBDIV_ALGORITHM_H

