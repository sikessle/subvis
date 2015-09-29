/**
 * @class SdAlgorithm "plugins/subdivision/sd_algorithm.h"
 *
 * @brief Base class for all subdivision algorithms.
 *
 * This interface shoulb be implemented by every subdivision class.
 * It provides the basic interface that the plugin is using
 * for subdivision.
 */

#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_ALGORITHM_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_ALGORITHM_H

#include <memory>
#include <string>
#include <QString>
#include "surface_mesh/Surface_mesh.h"

namespace subdivision {

class SdAlgorithm {
 public:
  using Surface_mesh = surface_mesh::Surface_mesh;
  using Point = surface_mesh::Point;

  virtual ~SdAlgorithm();

  /**
   * @brief Subdivides the given mesh object in n steps.
   * @param steps The number of subdivision steps.
   * @return The subdivided mesh
   */
  virtual std::unique_ptr<Surface_mesh> subdivide(const Surface_mesh& mesh,
      int steps = 1);

  /**
   * @brief Returns result mesh for testing.
   * @return A view on the mesh, but ownership stays in this class.
   */
  virtual const Surface_mesh& get_result_mesh();

  /// Test if the @c mesh can be subdivided with the subdivision algorithm.
  virtual bool is_subdividable(const Surface_mesh& mesh) const = 0;

 protected:

  /// Property key to access the coordinate of the vertex points.
  const ::std::string kPropVertexPoint = "v:point";

  /// Put the resulting mesh in here.
  std::unique_ptr<Surface_mesh> output_mesh_;

  /// Get the input mesh from here. The mesh may be modified freely,
  /// as this is only a copy of the original mesh (i.e. if special properties
  /// are needed).
  std::unique_ptr<Surface_mesh> input_mesh_;

  /// Property with the key kPropVertexPoint.
  /// Use @c init_mesh_members() and @c deinit_mesh_members() to initialize and release.
  Surface_mesh::Vertex_property<Point> v_points_;

  /// Writes the result of modifiyng the input_mesh_ to result_mesh_
  /** The input_mesh_ and result_mesh_ smart pointers must be used to read
   * from the original mesh and write the result.
   * This function is the initial starting point of the algorithm and the only
   * point in time where sub-classes may interact with the input/output mesh
   * pointers. After and before calling this method there is no guarantee about
   * the state of the input/result mesh pointers.
   * There is also no need to destruct or clean up the two pointers as they are
   * smart pointers.
   */
  virtual void subdivide_input_mesh_write_output_mesh() = 0;

  /// Allocate memory for surface mesh properties and init the class members.
  virtual void init_mesh_members();

  /// Release allocated memory of init_mesh_members().
  virtual void deinit_mesh_members();

  /// Compute the mid point of the @c edge (average of the two vertices)
  /// and store the computed coordinates in @c mid_edge.
  void compute_mid_edge(Point& mid_edge, const Surface_mesh::Edge& edge) const;
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_ALGORITHM_H

