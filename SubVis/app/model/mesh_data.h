#ifndef SUBVIS_MESH_DATA_H
#define SUBVIS_MESH_DATA_H

#include <string>
#include <memory>
#include <QObject>
#include "surface_mesh/Surface_mesh.h"

namespace subvis {

using surface_mesh::Surface_mesh;

class MeshData : public QObject
{
    Q_OBJECT

public:
    MeshData();

    Surface_mesh& mesh() const;
    /**
     * @brief Should be called if the mesh was modified.
     * Note that load() will also trigger this method.
     */
    void set_updated();
    bool empty() const;
    bool load(const std::string& filename);
    void load(std::unique_ptr<Surface_mesh> mesh);
    bool persist(const std::string& filename) const;
    const std::string& load_file_formats() const;
    const std::string& persist_file_formats() const;

private:
    std::unique_ptr<Surface_mesh> mesh_object_{new Surface_mesh};
    const std::string kLoadFileFormats {"*.obj *.off *.stl"};
    const std::string kPersistFileFormats {"*.off"};

signals:
    void updated();

public slots:

};

} // namespace SubVis
#endif // SUBVIS_MESH_DATA_H
