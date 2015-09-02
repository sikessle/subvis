
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_loop.h"

namespace subdivision {

void SubdivLoop::subdivide_input_mesh_write_output_mesh() {
  this->init_mesh_members();


  this->deinit_mesh_members();
}

void SubdivLoop::init_mesh_members() {
  SubdivAlgorithm::init_mesh_members();
}

void SubdivLoop::deinit_mesh_members() {
  SubdivAlgorithm::deinit_mesh_members();
}


} // namespace subdivision
