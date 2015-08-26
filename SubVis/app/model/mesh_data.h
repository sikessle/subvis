#ifndef SUBVIS_MESH_DATA_H
#define SUBVIS_MESH_DATA_H

#include <string>
#include <memory>
#include <QObject>
#include "surface_mesh/Surface_mesh.h"

namespace SubVis {

using std::string;
using std::unique_ptr;
using surface_mesh::Surface_mesh;

/**
 * @brief Must NEVER swap its encapsulated mesh object, as other
 * modules may rely on it. If it is swapped out, some modules may hold
 * an invalid reference to an old object.
 */
class MeshData : public QObject
{
    Q_OBJECT

public:
    MeshData();

    Surface_mesh& mesh();
    /**
     * @brief Should be called if the mesh was modified.
     */
    void set_updated();
    bool empty() const;
    bool load(const string &filename);
    bool persist(const string &filename) const;

private:
    unique_ptr<surface_mesh::Surface_mesh> mesh_object{new Surface_mesh};

signals:
    void updated();

public slots:

};

} // namespace SubVis
#endif // SUBVIS_MESH_DATA_H
