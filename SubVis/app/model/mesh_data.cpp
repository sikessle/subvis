#include "mesh_data.h"

namespace SubVis {
namespace Model {

MeshData::MeshData() : mesh_data(new Surface_mesh)
{

}

Surface_mesh& MeshData::mesh()
{
    return *mesh_data.get();
}

bool MeshData::empty() const
{
    return mesh_data->empty();
}

bool MeshData::load(string &filename)
{
    if (filename.empty()) {
        return false;
    }
    // only clear mesh to keep reference to it in other modules alive.
    mesh_data->clear();
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

} // namespace Model
} // namespace SubVis

