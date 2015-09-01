#include "plugins/subdivision/sd_butterfly.h"


namespace subdivision {

void SubdivButterfly::subdivide_specific_algorithm() {
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
