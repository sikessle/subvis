/**
*
* http://www.gamasutra.com/view/feature/131584/implementing_subdivision_surface_.php?print=1
* http://mrl.nyu.edu/~dzorin/papers/zorin1996ism.pdf
* http://www.multires.caltech.edu/pubs/interpolationTR.pdf
*
*/

#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_BUTTERFLY_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_BUTTERFLY_H

#include "plugins/subdivision/sd_triangle.h"

namespace subdivision {

class SdButterfly : public SdTriangle {
 protected:
  void subdivide_input_mesh_write_output_mesh() override;

  /**
   * @brief compute_edge_point Compute the interpolated point. Override for modified butterfly!
   */
  virtual void compute_edge_point(Point& edge_point,
                                  const Surface_mesh::Edge& edge);

  void compute_edge_point_ordinary(Point& edge_point,
                                   const Surface_mesh::Edge& edge);

 private:
  void compute_vertices_result();
  void compute_all_edge_points();
};

} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_BUTTERFLY_H
