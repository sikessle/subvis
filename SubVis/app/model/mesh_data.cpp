#include "mesh_data.h"

namespace SubVis {

MeshData::MeshData() : mesh_data(new surface_mesh::Surface_mesh)
{

}

surface_mesh::Surface_mesh& MeshData::mesh()
{
    return *mesh_data.get();
}

bool MeshData::empty() const
{
    return mesh_data->empty();
}

bool MeshData::load(std::string &filename)
{
    if (filename.empty()) {
        return false;
    }
    // only clear mesh to keep reference to it in other modules alive.
    mesh_data->clear();
    //mesh_data->load()
    return false;
}

bool MeshData::persist(std::string &filename) const
{
    if (filename.empty()) {
        return false;
    }
    return false;
}

} // namespace SubVis

