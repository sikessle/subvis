#ifndef SUBVIS_PLUGINS_SUBDIVISION_ALGORITHM_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_ALGORITHM_RENDERER_H

#include "plugins/subdivision/sd_algorithm.h"
#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

struct AlgorithmRenderer {
  std::unique_ptr<SdAlgorithm> algorithm;
  /// shared_ptr to allow multiple algorithms to use the same renderer
  std::shared_ptr<GLRenderer> renderer;
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_ALGORITHM_RENDERER_H
