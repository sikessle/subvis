#include "mesh_data.h"

namespace subvis {

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

void MeshData::load(std::unique_ptr<Surface_mesh> mesh)
{
    mesh_object_ = std::move(mesh);
}

bool MeshData::load(const std::string& filename)
{
    if (filename.empty()) {
        return false;
    }

    // create new mesh
    mesh_object_.reset(new Surface_mesh);

    bool success = mesh_object_->read(filename);

    emit updated();

    return success;
}

bool MeshData::persist(const std::string& filename) const
{
    if (filename.empty()) {
        return false;
    }

    mesh_object_->garbage_collection();

    return mesh_object_->write(filename);
}

const std::string& MeshData::load_file_formats() const
{
    return kLoadFileFormats;
}

const std::string& MeshData::persist_file_formats() const
{
    return kPersistFileFormats;
}

} // namespace SubVis

