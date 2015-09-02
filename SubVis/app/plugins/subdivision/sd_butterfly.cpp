#include "plugins/subdivision/sd_butterfly.h"


namespace subdivision {

void SubdivButterfly::subdivide_input_mesh_write_output_mesh() {
  this->init_mesh_members();


  this->deinit_mesh_members();
}

void SubdivButterfly::init_mesh_members() {
  SubdivAlgorithm::init_mesh_members();
}

void SubdivButterfly::deinit_mesh_members() {
  SubdivAlgorithm::deinit_mesh_members();
}

} // namespace subdivision
