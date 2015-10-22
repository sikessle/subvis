#include "plugins/subdivision/sd_none.h"

namespace subdivision {

using Surface_mesh = surface_mesh::Surface_mesh;

void SdNone::subdivide_threaded(const Surface_mesh& mesh,
                                std::function<void(std::unique_ptr<Surface_mesh>)> callback,
                                const int /*steps*/) {
  output_mesh_.reset(new Surface_mesh{mesh});
  callback(std::move(output_mesh_));
}

void SdNone::subdivide_input_mesh_write_output_mesh() {

}

bool SdNone::is_subdividable(const Surface_mesh& /*mesh*/) const {
  return true;
}

} // namespace subdivision

