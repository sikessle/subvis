#include "algorithm.h"

namespace SubdivisionPlugin {

void Algorithm::subdivide(MeshData& mesh_data, int steps)
{
    for (int i = 0; i < steps; i++) {
        //subdivide_specific_algorithm(mesh_data.mesh());
        foo();
    }
    mesh_data.set_updated();
}

} // namespace SubdivisionPlugin
