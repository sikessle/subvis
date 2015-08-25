#include "mesh_data.h"

namespace SubVis {

MeshData::MeshData()
{
}

Surface_mesh& MeshData::mesh()
{
    return *mesh_object.get();
}

bool MeshData::empty() const
{
    return mesh_object->empty();
}

void MeshData::set_updated()
{
    emit updated();
}

bool MeshData::load(string &filename)
{
    if (filename.empty()) {
        return false;
    }
    // only clear mesh to keep reference to it in other modules alive.
    mesh_object->clear();
    //mesh_data->load()

    emit updated();

    // if not successfull
    return false;
}

bool MeshData::persist(string &filename) const
{
    if (filename.empty()) {
        return false;
    }
    mesh_object->garbage_collection();
    return false;
}

} // namespace SubVis

