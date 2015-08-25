#include "mesh_data.h"

namespace SubVis {

MeshData::MeshData() : mesh_object(new Surface_mesh)
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
    return false;
}

bool MeshData::persist(string &filename) const
{
    if (filename.empty()) {
        return false;
    }
    return false;
}

} // namespace SubVis

