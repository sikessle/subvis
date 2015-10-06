#ifndef SUBVIS_CONTROLLER_DRAW_CONTROLLER_H
#define SUBVIS_CONTROLLER_DRAW_CONTROLLER_H

#include <QObject>

#include "controller/controller.h"

namespace subvis {

class DrawController : public Controller {
  Q_OBJECT

 public:
  DrawController(MeshData& mesh_data);

  MeshData& get_mesh_data();

 public slots:
  void triangulate_mesh();
};

} // namespace subvis
#endif // SUBVIS_CONTROLLER_DRAW_CONTROLLER_H
