#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H

#include <memory>
#include <QObject>
#include "surface_mesh/Surface_mesh.h"

namespace subdivision {

class GLRenderer {
 public:
  using Surface_mesh = surface_mesh::Surface_mesh;
  using Point = surface_mesh::Point;

  virtual ~GLRenderer();

  virtual void render_mesh_opengl();
  /// Is called once before render_mesh_opengl().
  /// Override for specific settings (background color etc.)
  virtual void init_opengl();
  /// Extracts the data required to render to a custom data structure.
  virtual void mesh_updated(const Surface_mesh& mesh);

 protected:
  // TODO placeholder, here we should use a custom data structure to hold vertices
  std::unique_ptr<Surface_mesh> mesh_ {nullptr};
//const surface_mesh::Surface_mesh* mesh_ {nullptr};

  /// Called with the custom data structure TODO!
  virtual void render() = 0;
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
