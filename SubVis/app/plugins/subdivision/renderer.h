#ifndef SUBVIS_PLUGINS_SUBDIVISION_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_RENDERER_H

#include "surface_mesh/Surface_mesh.h"

namespace subdivision {

class Renderer
{
public:
    virtual ~Renderer();

    virtual void render_limit_surface_opengl(const surface_mesh::Surface_mesh& mesh);
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_RENDERER_H
