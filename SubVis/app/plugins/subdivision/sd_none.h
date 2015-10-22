#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_NONE_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_NONE_H

#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {

class SdNone : public SdAlgorithm {
 public:
  using Surface_mesh = surface_mesh::Surface_mesh;

  void subdivide_threaded(const Surface_mesh& mesh,
                          std::function<void(std::unique_ptr<Surface_mesh>)> callback,
                          int /*steps = 1*/) override {
    output_mesh_.reset(new Surface_mesh{mesh});
    callback(std::move(output_mesh_));
  }

  bool is_subdividable(const Surface_mesh& /*mesh*/) const override {
    return true;
  }

 protected:
  void subdivide_input_mesh_write_output_mesh() override {}
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_NONE_H
