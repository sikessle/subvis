#ifndef SUBDIVISIONPLUGIN_ALGORITHM_H
#define SUBDIVISIONPLUGIN_ALGORITHM_H

#include <QString>
#include "surface_mesh/Surface_mesh.h"

#include "model/mesh_data.h"

namespace SubdivisionPlugin {

using surface_mesh::Surface_mesh;

class Algorithm
{
public:
    virtual ~Algorithm() {}

    virtual void subdivide(SubVis::MeshData& mesh_data, int steps = 1);
    /**
     * @brief unique id which identifies the algorithm.
     * i.e. de.htwg.ios.subvis.plugin.subdivision.catmull
     * @return
     */
    virtual const QString id() = 0;

    virtual const QString name() = 0;

    /**
     * @brief Returns result mesh for testing etc.
     * @return
     */
    virtual Surface_mesh& result_mesh();

protected:
    /**
     * @brief put the resulting mesh in here
     */
    std::unique_ptr<Surface_mesh> result_mesh_;

    std::unique_ptr<Surface_mesh> input_mesh_;

    /**
     * @brief Writes the result of modifiyng the input_mesh_ to result_mesh_
     */
    virtual void subdivide_specific_algorithm() = 0;
};

} // namespace SubdivisionPlugin
#endif // SUBDIVISIONPLUGIN_ALGORITHM_H

