#ifndef UTILS
#define UTILS

// ===============[ includes ]===============

#include <string>

namespace SubVis {
namespace Algo {

// ===============[ const ]===============

enum ObjDemoFiles {
    kBigGuy = 0,
    kCube,
    kMonsterFrog,
    kTetra
};
const std::string kObjDemoFilesString[4] = {"bigguy.obj", "cube.obj", "monsterfrog.obj", "tetra.obj"};

const std::string kRootPathToObjFiles = "./../../SubVis/objs/";

// ===============[ public prototypes ]===============

/*!
 * \brief utils_q_debug_mesh Print basic information (Vertices, Edges and Faces) of mesh with QDebug.
 * \param mesh
 * \param title
 */
void utils_q_debug_mesh(const surface_mesh::Surface_mesh& mesh, const QString &title);


} // namespace Algo
} // namespace SubVis

#endif // UTILS

