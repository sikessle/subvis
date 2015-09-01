#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_BUTTERFLY_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_BUTTERFLY_H

#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {

class SubdivButterfly : public SubdivAlgorithm {
 protected:
  virtual void subdivide_specific_algorithm() override;
  virtual void init_mesh_members() override;
  virtual void deinit_mesh_members() override;
};

} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_BUTTERFLY_H
