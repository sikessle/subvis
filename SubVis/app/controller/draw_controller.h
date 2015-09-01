#ifndef SUBVIS_CONTROLLER_DRAW_CONTROLLER_H
#define SUBVIS_CONTROLLER_DRAW_CONTROLLER_H

#include <QObject>

#include "controller/controller.h"

namespace subvis {

class DrawController : public Controller {
  Q_OBJECT

 public:
  DrawController(MeshData& mesh_data);

  MeshData& mesh_data();
};

} // namespace subvis
#endif // SUBVIS_CONTROLLER_DRAW_CONTROLLER_H
