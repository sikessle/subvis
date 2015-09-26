/**
 * @class SdButterfly Butterfly subdivision "plugins/subdivision/sd_butterfly.h"
 *
 * @brief Implementation of the Butterfly subdivision algorithm.
 *
 * The algorithm was devised by Nira Dyn, David Levine und John A. Gregory.
 * Butterfly subdivision operates on triangular meshes.
 * The subdivided output mesh contains always triangle faces.
 * The control points are interpolated.
 *
 * The subdivision scheme:
 *  1. Compute edge points for each edge. These points are the new interpolated vertices.
 *     The edge point is computed with the following 8 point stencil.
 *     (This is the same as the 10 point stencil with w = 0).
 *     c------b------c
 *      \    / \    /
 *       \  /   \  /
 *        \/     \/
 *         a--x--a       a: 1/2, b: 1/8, c: -1/16
 *        /\     /\
 *       /  \   /  \
 *      /    \ /    \
 *     c------b------c
 *      Boundary case: use 1-dimensional 4 point scheme (-1/16)-----(9/16)--x--(9/16)-----(-1/16)
 *   2. Replace each triangle face by 4 new triangle faces.
 *      +----x-----+      +----x-----+
 *       \        /        \   /\   /
 *        \      /          \ /  \ /
 *         x    x  -->       x----x
 *          \  /              \  /
 *           \/                \/
 *           +                 +
 *
 * Sources that helped to implement the algorithm.
 * <a href="http://www.gamasutra.com/view/feature/131584/implementing_subdivision_surface_.php?print=1">Implementing Subdivision Surface Theory</a>
 * <a href="http://mrl.nyu.edu/~dzorin/papers/zorin1996ism.pdf">Interpolating Subdivision for Meshes with Arbitrary Topology</a>
 * <a href="http://www.multires.caltech.edu/pubs/interpolationTR.pdf">Interpolating Subdivision for Meshes with Arbitrary Topology</a>
 * <a href="http://lgdv.cs.fau.de/get/942">Subdivision Surfaces - Lehrstuhl für Graphische Datenverarbeitung</a>
 *
 * @author Felix Born
 *
 */

#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_BUTTERFLY_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_BUTTERFLY_H

#include "plugins/subdivision/sd_triangle.h"

namespace subdivision {

class SdButterfly : public SdTriangle {
 protected:
  void subdivide_input_mesh_write_output_mesh() override;

  /// Compute the new interpolated vertex point of the @c edge with the 8 point stencil.
  /** This function calls only @c compute_edge_point_ordinary or @c compute_edge_point_boundary.
   *  To implement the Modified Butterfly algorithm,
   *  simply inherit from this class and overrite this method.
   */
  virtual void compute_edge_point(Point& edge_point,
                                  const Surface_mesh::Edge& edge) const;

  /// Compute the new interpolated vertex point of the @c edge with the 8 point stencil.
  void compute_edge_point_ordinary(Point& edge_point,
                                   const Surface_mesh::Edge& edge) const;

  /// Compute the new interpolated vertex point of the @c edge with the 1-dimensional 4 point stencil for boundaries.
  void compute_edge_point_boundary(Point& edge_point,
                                   const Surface_mesh::Edge& edge) const;

  /// Returns the next boundary halfedge. If there is no next halfedge the @c halfedge passed as argument is returned.
  surface_mesh::Surface_mesh::Halfedge get_next_boundary_halfedge(
    const Surface_mesh::Halfedge halfedge) const;

 private:
  /// Copy all vertices from the input mesh to the output mesh and store the vertex index in @c v_index_output_v_prop_.
  void add_all_vertices_output_mesh();

  /// Loop over all edges and compute the new inteprolated vertices.
  /// @sa compute_edge_point_ordinary(Point& edge_point,const Surface_mesh::Edge& edge)
  void add_all_edge_points_output_mesh();
};

} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_BUTTERFLY_H
