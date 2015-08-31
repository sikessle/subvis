#ifndef SUBVIS_DRAW_CONTROLLER_H
#define SUBVIS_DRAW_CONTROLLER_H

#include <QObject>
#include "surface_mesh/Surface_mesh.h"

#include "controller/controller.h"

namespace subvis {

using surface_mesh::Surface_mesh;

class DrawController : public Controller
{
    Q_OBJECT

public:
    DrawController(MeshData& mesh_data);

    MeshData& mesh_data();
};

} // namespace subvis
#endif // SUBVIS_DRAW_CONTROLLER_H
