#include "mesh_data.h"

namespace SubVis {

MeshData::MeshData()
{
}

Surface_mesh& MeshData::mesh() const
{
    return *mesh_object_.get();
}

bool MeshData::empty() const
{
    return mesh_object_->empty();
}

void MeshData::set_updated()
{
    emit updated();
}

bool MeshData::load(const string& filename)
{
    if (filename.empty()) {
        return false;
    }

    // only clear mesh to keep reference to it in other modules alive.
    mesh_object_->clear();

    bool success = mesh_object_->read(filename);

    emit updated();

    return success;
}

bool MeshData::persist(const string& filename) const
{
    if (filename.empty()) {
        return false;
    }

    mesh_object_->garbage_collection();

    return mesh_object_->write(filename);
}

const string& MeshData::load_file_formats() const
{
    return kLoadFileFormats;
}

const string& MeshData::persist_file_formats() const
{
    return kPersistFileFormats;
}

} // namespace SubVis

