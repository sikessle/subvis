#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_LOOP_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_LOOP_H

#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {

class SubdivLoop : public SubdivAlgorithm {
 public:
  using Surface_mesh = surface_mesh::Surface_mesh;
  using Point = surface_mesh::Point;

 protected:
  virtual void subdivide_specific_algorithm() override;

};

} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_LOOP_H
