#ifndef SUBIVS_IO_CONTROLLER_H
#define SUBIVS_IO_CONTROLLER_H

#include <QObject>

#include "controller/controller.h"

namespace SubVis {

class IOController : public Controller
{
    Q_OBJECT

public:
    IOController(MeshData& mesh_data);

    bool load_mesh(const std::string& filename);
    bool persist_mesh(const std::string& filename) const;
    const std::string& load_file_formats() const;
    const std::string& persist_file_formats() const;
};

} // namespace SubVis
#endif // SUBIVS_IO_CONTROLLER_H
