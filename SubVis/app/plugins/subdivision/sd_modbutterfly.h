#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_MODBUTTERFLY_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_MODBUTTERFLY_H

#include "plugins/subdivision/sd_butterfly.h"

namespace subdivision {

/**
 * @brief Implementation of the Modified Butterfly subdivision algorithm.
 *
 * The algorithm was devised by Denis Zorin and Peter Schröder.
 * It is related to the Butterfly subdivision.
 * If there is a vertex with valence == 6 the Modified Butterfly behaves equal to the Butterfly.
 * The difference is if there are vertices with valence != 6 (extraordinary).
 * For such cases there are special rules to make the subdivided surfaces smoother compared to Butterfly.
 *
 *
 * The subdivision scheme:
 *  1. Compute edge points for each edge.
 *    - both vertices of the edge with valence == 6: use the 8 point stencil of the Butterfly
 *    - one vertex of the edge is extraordinary: use the following stencil <pre>
 *       e3---------e2
 *        \        / \
 *         \      /   \
 *          \    /     \
 *           \  /       \
 *            \/         \
 *            v-----------e1
 *           / \          /
 *          /   \        /
 *         /     x      /
 *        /       \    /
 *       /         \  /
 *                  \/
 *       ------------e0</pre>
 *      - valence == 3: v: 3/4, e0: 5/12, e1: -1/12, e2: -1/12
 *      - valence == 4: v: 3/4, e0: 3/8, e1: 0, e2: -1/8, e3: 0
 *      - valence >= 5: v: 3/4, ej: \f$(1/4 + cos(2 * \pi * j/N) + 1/2 * cos(4 * \pi * j/N)/N)\f$
 *    - both vertices of the edge are extraordinary: average the results of using the above extraordinary stencil
 *  2. Replace each triangle face (equal to Butterfly).
 *
 * Sources that helped to implement the algorithm.
 * - <a href="http://www.gamasutra.com/view/feature/131584/implementing_subdivision_surface_.php?print=1">Implementing Subdivision Surface Theory</a>
 * - <a href="http://mrl.nyu.edu/~dzorin/papers/zorin1996ism.pdf">Interpolating Subdivision for Meshes with Arbitrary Topology</a>
 * - <a href="http://www.multires.caltech.edu/pubs/interpolationTR.pdf">Interpolating Subdivision for Meshes with Arbitrary Topology</a>
 *
 * @author Felix Born
 *
 */
class SdModButterfly : public SdButterfly {
 protected:

  /// Compute the new interpolated vertex point of the @c edge with appropriate stencil.
  void compute_edge_point(Point& edge_point,
                          const Surface_mesh::Edge& edge) const override;

 private:
  /// Compute the new interpolated vertex point of the edge if there is one extraordinary vertex (from vertex of the @c halfedge).
  void compute_edge_point_one_extraordinary(Point& edge_point,
      const Surface_mesh::Halfedge& halfedge) const;

  /// Compute the new interpolated vertex point of the @c edge with two extraordinary vertices.
  void compute_edge_point_two_extraordinary(Point& edge_point,
      const Surface_mesh::Edge& edge) const;
};

} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_MODBUTTERFLY_H
