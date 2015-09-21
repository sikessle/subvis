#include "plugins/subdivision/sd_butterfly.h"


namespace subdivision {

void SdButterfly::subdivide_input_mesh_write_output_mesh() {
  this->init_mesh_members();


  this->deinit_mesh_members();
}

void SdButterfly::init_mesh_members() {
  SdAlgorithm::init_mesh_members();
}

void SdButterfly::deinit_mesh_members() {
  SdAlgorithm::deinit_mesh_members();
}

} // namespace subdivision
