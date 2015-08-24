#ifndef SUBVIS_MODEL_MESHMODEL_H
#define SUBVIS_MODEL_MESHMODEL_H

#include <string>
#include <memory>
#include <QObject>
#include "surface_mesh/Surface_mesh.h"

using namespace surface_mesh;
using namespace std;

namespace SubVis {
namespace Model {

/**
 * @brief Must NEVER swap its encapsulated mesh_data object, as other
 * modules may rely on it. If it is swapped out, some modules may hold
 * an invalid reference to an old object.
 */
class MeshData : public QObject
{
    Q_OBJECT

public:
    MeshData();

    Surface_mesh& mesh();
    bool empty() const;
    bool load(string &filename);
    bool persist(string &filename) const;

private:
    unique_ptr<Surface_mesh> mesh_data;
};

} // namespace Model
} // namespace SubVis


#endif // SUBVIS_MODEL_MESHMODEL_H
