#ifndef SUBDIVISIONPLUGIN_ALGORITHM_H
#define SUBDIVISIONPLUGIN_ALGORITHM_H

#include <QString>
#include "surface_mesh/Surface_mesh.h"

#include "model/mesh_data.h"

namespace SubdivisionPlugin {

class Algorithm
{
public:
    using Surface_mesh = surface_mesh::Surface_mesh;
    using Point = surface_mesh::Point;

    virtual ~Algorithm() {}

    virtual std::unique_ptr<Surface_mesh> subdivide(SubVis::MeshData& mesh_data, int steps = 1);
    /**
     * @brief unique id which identifies the algorithm.
     * i.e. de.htwg.ios.subvis.plugin.subdivision.catmull
     * @return
     */
    virtual const QString id() = 0;

    virtual const QString name() = 0;

    /**
     * @brief Returns result mesh for testing.
     * @return A view on the mesh, but ownership stays in this class.
     */
    virtual Surface_mesh& result_mesh();

protected:
    /**
     * @brief put the resulting mesh in here
     */
    std::unique_ptr<Surface_mesh> result_mesh_;

    std::unique_ptr<Surface_mesh> input_mesh_;

    Surface_mesh::Vertex_property<Point> v_points_;

    /**
     * @brief Writes the result of modifiyng the input_mesh_ to result_mesh_
     */
    virtual void subdivide_specific_algorithm() = 0;

    void init_mesh_members();

    /**
     * @brief mid_edge Compute the mid point of an edge (average of the two vertices).
     *                 The edge has to be in the input_mesh_.
     * @param mid_edge The result (mid of edge).
     * @param edge Edge to compute the mid point.
     */
    virtual void mid_edge(Point& mid_edge, const Surface_mesh::Edge& edge);

    /**
     * @brief compute_face_point Compute the average of all the points of the face.
     */
    virtual void compute_face_point(Point& face_point, const Surface_mesh::Face& face);

};

} // namespace SubdivisionPlugin
#endif // SUBDIVISIONPLUGIN_ALGORITHM_H

