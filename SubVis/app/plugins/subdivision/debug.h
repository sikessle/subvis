#ifndef SUBVIS_PLUGINS_SUBDIVISION_UTILS_H
#define SUBVIS_PLUGINS_SUBDIVISION_UTILS_H

#include <string>
#include <QString>
#include "surface_mesh/Surface_mesh.h"

namespace subdivision {

// uncomment to disable debug output
#define DEBUG_SUBDIV

enum ObjDemoFiles {
  kBigGuy = 0,
  kCube,
  kMonsterFrog,
  kTetra
};
const ::std::string kObjDemoFilesString[4] = {"bigguy.obj", "cube.obj", "monsterfrog.obj", "tetra.obj"};
const ::std::string kRootPathToObjFiles = "./../../SubVis/objs/";

/**
 * @brief utils_q_debug_mesh Print basic information (Vertices, Edges and Faces) of mesh with QDebug.
 * @param mesh
 * @param title
 */
void debug_mesh(const surface_mesh::Surface_mesh& mesh,
                      const QString& title);
void debug_point(const surface_mesh::Point point, const QString& title);

#ifdef DEBUG_SUBDIV

#define DEBUG_MESH(mesh, name)      debug_mesh(mesh, name);
#define DEBUG_POINT(point, name)    debug_point(point, name);

#else

#define DEBUG_MESH(mesh, name)
#define DEBUG_POINT(point, name)

#endif

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_UTILS_H
