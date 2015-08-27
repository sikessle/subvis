#include "plugins/subdivision/algorithm.h"

namespace SubdivisionPlugin {

void Algorithm::subdivide(SubVis::MeshData& mesh_data, int steps)
{
    result_mesh_.reset(new Surface_mesh);
    input_mesh_.reset(new Surface_mesh{mesh_data.mesh()});

    for (int i = 0; i < steps; i++) {
        result_mesh_->clear();
        subdivide_specific_algorithm();
        // input mesh is now the previous result mesh
        input_mesh_.reset(new Surface_mesh{*(result_mesh_.get())});
    }

    mesh_data.load(std::move(result_mesh_));

    // free memory
    result_mesh_.reset(nullptr);
    input_mesh_.reset(nullptr);
}

Surface_mesh& Algorithm::result_mesh()
{
    return *(result_mesh_.get());
}

} // namespace SubdivisionPlugin
