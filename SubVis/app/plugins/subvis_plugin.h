#ifndef SUBVIS_SUBVIS_PLUGIN_H
#define SUBVIS_SUBVIS_PLUGIN_H

#include <string>
#include <QWidget>

#include "model/mesh_data.h"

namespace subvis {

/// Interface for plugins
class SubVisPlugin {
 public:
  virtual ~SubVisPlugin() {}

  virtual const QString id() const = 0;
  virtual const QString name() const = 0;
  virtual void set_model(MeshData& mesh_data) = 0;
  virtual void mesh_updated(const surface_mesh::Surface_mesh& mesh,
                            int mesh_id) = 0;
  virtual void init_opengl(int mesh_id) = 0;
  virtual void draw_opengl(int mesh_id) = 0;
  virtual void create_gui(QWidget* parent) = 0;
};

} // namespace subvis

#endif // SUBVIS_SUBVIS_PLUGIN_H

