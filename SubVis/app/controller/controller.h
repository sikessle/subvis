#ifndef SUBVIS_CONTROLLER_H
#define SUBVIS_CONTROLLER_H

#include <QObject>

#include "model/mesh_data.h"

namespace SubVis {

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(MeshData& mesh_data);

protected:
    MeshData& mesh_data_;

signals:

public slots:
};

} // namespace SubVis
#endif // SUBVIS_CONTROLLER_H
