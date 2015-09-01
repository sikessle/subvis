/**
 *
 * @brief Implementation of the Loop subdivision algorithm.
 *
 * 1. create edge point for each edge
 * 2. update all vertices of the old mesh
 * 2. create new faces (4 new triangle faces replace the old triangle face)
 * (https://graphics.stanford.edu/~mdfisher/subdivision.html)
 *
 */


#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_LOOP_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_LOOP_H

#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {

class SubdivLoop : public SubdivAlgorithm {
 protected:
  virtual void subdivide_specific_algorithm() override;
  virtual void init_mesh_members() override;
  virtual void deinit_mesh_members() override;

};

} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_LOOP_H
