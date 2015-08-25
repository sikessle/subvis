#ifndef SUBVIS_DRAW_CONTROLLER_H
#define SUBVIS_DRAW_CONTROLLER_H

#include <QObject>
#include "controller/controller.h"
#include "surface_mesh/Surface_mesh.h"

namespace SubVis {

using surface_mesh::Surface_mesh;

class DrawController : public Controller
{
    Q_OBJECT

public:
    DrawController(MeshData &mesh);

    Surface_mesh &mesh();

signals:
    void redraw_required();

public slots:
    void mesh_updated();
};

} // namespace SubVis
#endif // SUBVIS_DRAW_CONTROLLER_H
