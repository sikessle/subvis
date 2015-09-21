/**
*
* http://www.gamasutra.com/view/feature/131584/implementing_subdivision_surface_.php?print=1
* http://mrl.nyu.edu/~dzorin/papers/zorin1996ism.pdf
* http://www.multires.caltech.edu/pubs/interpolationTR.pdf
*
*/


#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_MODBUTTERFLY_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_MODBUTTERFLY_H

#include "plugins/subdivision/sd_butterfly.h"

namespace subdivision {

class SdModButterfly : public SdButterfly {
 protected:
  void compute_edge_point(Point& edge_point,
                          const Surface_mesh::Edge& edge) override;

 private:
  void compute_edge_point_one_extraordinary(Point& edge_point,
      const Surface_mesh::Halfedge& halfedge);
  void compute_edge_point_two_extraordinary(Point& edge_point,
      const Surface_mesh::Edge& edge);
};

} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_MODBUTTERFLY_H
