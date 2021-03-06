#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_ALGORITHM_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_ALGORITHM_H

#include <memory>
#include <string>
#include <QObject>
#include <QString>
#include "surface_mesh/Surface_mesh.h"

namespace subdivision {

///
/// \brief Abstract base class for all subdivision algorithms.
///
class SdAlgorithm : public QObject {
  Q_OBJECT
 public:
  using Surface_mesh = surface_mesh::Surface_mesh;
  using Point = surface_mesh::Point;

  SdAlgorithm();
  virtual ~SdAlgorithm();

  /**
   * \brief Subdivides the given mesh object in n steps.
   * \param mesh The mesh which will be subdivided.
   * \param callback The function which will be called when the computation is done.
   * This function will be called in the single GUI thread, so it has not to be thread safe.
   * \param steps The number of subdivision steps.
   */
  virtual void subdivide_threaded(const Surface_mesh& mesh,
                                  std::function<void(std::unique_ptr<Surface_mesh>)> callback,
                                  const int steps = 1);

  /// Stops after the current subdivision step and executes the callback.
  virtual void stop_subdivide_threaded();

  /**
   * \brief Returns result mesh for testing.
   * \return A view on the mesh, but ownership stays in this class.
   */
  virtual const Surface_mesh& get_result_mesh() const;

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

  /// Property with the key @c kPropVertexPoint.
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

  /// Release allocated memory of @c init_mesh_members().
  virtual void deinit_mesh_members();

  /// Compute the mid point of the @c edge (average of the two vertices)
  /// and store the computed coordinates in @c mid_edge.
  void compute_mid_edge(Point& mid_edge, const Surface_mesh::Edge& edge) const;

  /// Compute the new vertex coordinate of the boundary @c vertex using the weights 1/8----3/4----1/8.
  void compute_new_boundary_vertex_coordinate(Point& new_vertex_point,
      const Surface_mesh::Vertex& vertex) const;

  /// Return the valid @c halfedge of a boundary edge.
  Surface_mesh::Halfedge get_valid_halfedge_of_boundary_edge(
    const Surface_mesh::Edge& edge) const;

  /// Find a halfedge that is connected with the @c vertex and belongs to a boundary edge
  /// (Doesn't have to be a boundary halfedge!).
  /// It is the same as @c find_halfedge_of_boundary_edge_ccw(input_mesh_->halfedge(vertex))
  Surface_mesh::Halfedge find_halfedge_of_boundary_edge_ccw(
    const Surface_mesh::Vertex& vertex) const;

  /// Find a halfedge that is connected with the @c vertex and belongs to a boundary edge
  /// (Doesn't have to be a boundary halfedge!).
  /// Start search with the @c start_halfedge and rotate counter clockwise.
  Surface_mesh::Halfedge find_next_halfedge_of_boundary_edge_ccw(
    const Surface_mesh::Halfedge start_halfedge) const;

 private:
  bool thread_running_{false};
  volatile bool stop_subdivide_{false};
  std::function<void(std::unique_ptr<Surface_mesh>)> callback_;

  void subdivide_worker(int steps);
  void subdivide_worker_cleanup();

 signals:
  void subdivide_worker_finished();
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_ALGORITHM_H

