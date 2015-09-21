#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_BUTTERFLY_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_BUTTERFLY_H

#include "plugins/subdivision/sd_triangle.h"

namespace subdivision {

class SdButterfly : public SdTriangle {
 protected:
  void subdivide_input_mesh_write_output_mesh() override;
  void init_mesh_members() override;
  void deinit_mesh_members() override;
};

} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_BUTTERFLY_H
