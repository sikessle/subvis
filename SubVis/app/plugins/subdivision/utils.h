#ifndef SUBDIVISIONPLUGIN_UTILS_H
#define SUBDIVISIONPLUGIN_UTILS_H

// ===============[ includes ]===============

#include <string>
#include "surface_mesh/Surface_mesh.h"

namespace SubdivisionPlugin {

// ===============[ const ]===============

enum ObjDemoFiles {
    kBigGuy = 0,
    kCube,
    kMonsterFrog,
    kTetra
};
const ::std::string kObjDemoFilesString[4] = {"bigguy.obj", "cube.obj", "monsterfrog.obj", "tetra.obj"};

const ::std::string kRootPathToObjFiles = "./../../SubVis/objs/";

// ===============[ public prototypes ]===============

/**
 * @brief utils_q_debug_mesh Print basic information (Vertices, Edges and Faces) of mesh with QDebug.
 * @param mesh
 * @param title
 */
void utils_debug_mesh(const surface_mesh::Surface_mesh& mesh, const QString &title);

void utils_debug_point(const surface_mesh::Point point, const QString& title);

} // namespace SubdivisionPlugin
#endif // SUBDIVISIONPLUGIN_UTILS_H
