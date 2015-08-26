#ifndef SUBIVS_IO_CONTROLLER_H
#define SUBIVS_IO_CONTROLLER_H

#include <QObject>
#include "controller/controller.h"

namespace SubVis {

using std::string;

class IOController : public Controller
{
    Q_OBJECT

public:
    IOController(MeshData &mesh);

    bool load_mesh(const string &filename);
    bool persist_mesh(const string &filename) const;
    const string &load_supported_formats() const;

private:
    const string kLoadSupportedFormats = "*.obj *.off *.stl";
};

} // namespace SubVis
#endif // SUBIVS_IO_CONTROLLER_H
