#include "plugins/subdivision/algorithm.h"

namespace SubdivisionPlugin {

void Algorithm::subdivide(SubVis::MeshData& mesh_data, int steps)
{
    std::unique_ptr<Surface_mesh> input_mesh;
    result_mesh_.reset(new Surface_mesh{mesh_data.mesh()});

    for (int i = 0; i < steps; i++) {
        input_mesh.reset(new Surface_mesh{*(result_mesh_.get())});
        result_mesh_->clear();
        // deep copy
        subdivide_specific_algorithm(*(input_mesh.get()));
    }

    mesh_data.load(std::move(result_mesh_));
}

} // namespace SubdivisionPlugin
