#ifndef SUBIVS_IO_CONTROLLER_H
#define SUBIVS_IO_CONTROLLER_H

#include <QObject>
#include "model/mesh_data.h"

namespace SubVis {

class IOController : public QObject
{
    Q_OBJECT

public:
    IOController(SubVis::MeshData mesh);
    bool load_mesh(std::string &filename);
    bool persist_mesh(std::string &filename) const;

private:
    SubVis::MeshData &mesh_model;

signals:

public slots:

};

} // namespace SubVis
#endif // SUBIVS_IO_CONTROLLER_H
