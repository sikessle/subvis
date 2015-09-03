#include "GL/gl.h"

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

GLRenderer::~GLRenderer() {
}

void GLRenderer::mesh_updated(const surface_mesh::Surface_mesh& mesh) {
  // placeholder for custom data structure
  mesh_ = &mesh;
  // TODO extract vertices in a custom data structure (member variable) to speed up drawing!
}

void GLRenderer::render_mesh_opengl() {
  if (mesh_) {
    render();
  }
}

void GLRenderer::init_opengl() {
  // basic init
}

} // namespace subdivision
