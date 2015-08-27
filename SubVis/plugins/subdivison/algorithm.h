#ifndef SUBDIVISIONPLUGIN_ALGORITHM_H
#define SUBDIVISIONPLUGIN_ALGORITHM_H

#include "surface_mesh/Surface_mesh.h"

#include "model/mesh_data.h"

namespace SubdivisionPlugin {

using surface_mesh::Surface_mesh;
using SubVis::MeshData;

class Algorithm
{
public:
    virtual ~Algorithm();

    virtual void subdivide(MeshData& mesh_data, int steps = 1);
    virtual void foo();

protected:
    //virtual void subdivide_specific_algorithm(Surface_mesh& mesh) = 0;
};

} // namespace SubdivisionPlugin
#endif // SUBDIVISIONPLUGIN_ALGORITHM_H

