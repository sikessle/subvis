#ifndef SUBVIS_CONTROLLER_CONTROLLER_H
#define SUBVIS_CONTROLLER_CONTROLLER_H

#include <QObject>

#include "model/mesh_data.h"

namespace subvis {

class Controller : public QObject {
  Q_OBJECT

 public:
  Controller(MeshData& mesh_data);

 protected:
  MeshData& mesh_data_;
};

} // namespace subvis
#endif // SUBVIS_CONTROLLER_CONTROLLER_H
