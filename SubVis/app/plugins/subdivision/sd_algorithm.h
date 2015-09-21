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

 protected:
  /**
   * @brief kPropVertexPoint key to access surface mesh property that stores the vertex coordinates
   */
  const ::std::string kPropVertexPoint = "v:point";

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
  virtual void subdivide_input_mesh_write_output_mesh() = 0;

  /**
   * @brief init_mesh_members Allocate memory for surface mesh properties and init the class members.
   */
  virtual void init_mesh_members();
  /**
   * @brief deinit_mesh_members Release allocated memory of init_mesh_members().
   */
  virtual void deinit_mesh_members();
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_ALGORITHM_H

