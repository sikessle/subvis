/**
 * @class SdLoop Loop subdivision "plugins/subdivision/sd_loop.h"
 *
 * @brief Implementation of the Loop subdivision algorithm.
 *
 * The algorithm was devised by Charles Loop.
 * Loop subdivision is based on a quartic box spline and
 * operates on triangular meshes.
 * The subdivided output mesh contains always triangle faces.
 * The control points are approximated.
 *
 * The subdivision scheme:
 *  1. Compute edge points for each edge (these edge points are also called odd vertices).
 *           1/8
 *          /  \
 *         /    \
 *        /      \
 *       /        \
 *      /          \
 *     3/8---o----3/8   o: odd vertex
 *       \        /
 *        \      /
 *         \    /
 *          \  /
 *           \/
 *           1/8
 *     Boundary case: 1/2----o----1/2
 *  2. Update all vertices of the input mesh (these updated vertex points are also called even vertices)
 *      b----------b
 *       \        / \
 *        \      /   \
 *         \    /     \
 *          \  /       \
 *           \/         \
 *           e-----------b    e: /f$even vertex = vertex * (1 - n * beta) + (sum of surrounding vertices) * beta/f$
 *          / \          /
 *         /   \        /     n - valence
 *        /     \      /      beta (as proposed by Warren): for n > 3, beta = 3/(8n) and for n = 3, beta = 3/16
 *       /       \    /       beta (original choice of Loop): /f$1/n(5/8-(3/8+1/4cos(2pi/n))^2)/f$
 *      /         \  /
 *                 \/
 *      ------------b
 *      Boundary case: 1/8----e: 3/4----1/8
 *   3. Replace each triangle face by 4 new triangle faces. (e: even vertex, o: odd vertex)
 *      e----o-----e      e----o-----e
 *       \        /        \   /\   /
 *        \      /          \ /  \ /
 *         o    o  -->       o----o
 *          \  /              \  /
 *           \/                \/
 *           e                 e
 *
 * Sources that helped to implement the algorithm.
 * <a href="https://graphics.stanford.edu/~mdfisher/subdivision.html">Matt's Webcorner</a>
 * <a href="http://www.cs.cmu.edu/afs/cs/academic/class/15462-s14/www/lec_slides/Subdivision.pdf">Subdivision</a>
 *
 * @author Felix Born
 *
 */

#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_LOOP_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_LOOP_H

#include "plugins/subdivision/sd_triangle.h"

namespace subdivision {

class SdLoop : public SdTriangle {
 protected:
  void subdivide_input_mesh_write_output_mesh() override;

 private:

  /// Loop over all edges, compute odd vertices and add them to the output mesh.
  /// @sa compute_odd_vertex(Point& odd_vertex, const Surface_mesh::Edge& edge)
  void compute_all_odd_vertices();

  /// Loop over all vertices, compute even vertices and add them to the output mesh.
  /// @sa void compute_even_vertex(Point& even_vertex, const Surface_mesh::Vertex& vertex)
  void compute_all_even_vertices();

  /// Compute the odd vertex point of an @c edge.
  /// @sa compute_odd_vertex_regular(Point& odd_vertex, const Surface_mesh::Edge& edge),
  /// compute_odd_vertex_boundary(Point& odd_vertex, const Surface_mesh::Edge& edge)
  void compute_odd_vertex(Point& odd_vertex, const Surface_mesh::Edge& edge);

  /// Compute the odd vertex point of an @c edge using the regular subdivision scheme.
  void compute_odd_vertex_regular(Point& odd_vertex,
                                  const Surface_mesh::Edge& edge);

  /// Compute the odd vertex point of an @c edge using the scheme for boundary cases.
  void compute_odd_vertex_boundary(Point& odd_vertex,
                                   const Surface_mesh::Edge& edge);

  /// Compute the even vertex point of a @c vertex.
  /// @sa compute_even_vertex_regular(Point& even_vertex, const Surface_mesh::Vertex& vertex),
  /// compute_even_vertex_boundary(Point& even_vertex, const Surface_mesh::Vertex& vertex)
  void compute_even_vertex(Point& even_vertex,
                           const Surface_mesh::Vertex& vertex);

  /// Compute the even vertex point of a @c vertex using the regular subdivision scheme.
  void compute_even_vertex_regular(Point& even_vertex,
                                   const Surface_mesh::Vertex& vertex);

  /// Compute the even vertex point of a @c vertex using the scheme for boundary cases.
  void compute_even_vertex_boundary(Point& even_vertex,
                                    const Surface_mesh::Vertex& vertex);

  /// Compute the constante beta (proposed by Warren) for a vertex with valence @c n.
  double compute_beta(unsigned int n) const;

};

} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_LOOP_H
