#include "view/edit_constraint.h"

namespace subvis {

EditConstraint::EditConstraint() {
  local_constraint_.setTranslationConstraintType(qglviewer::AxisPlaneConstraint::PLANE);
}

void EditConstraint::constrainTranslation(qglviewer::Vec& translation,
                                     qglviewer::Frame* const frame) {
  local_constraint_.constrainTranslation(translation, frame);
}

void EditConstraint::set_vertex_normal(const surface_mesh::Normal &normal) {
  auto direction = qglviewer::Vec(normal[0], normal[1], normal[2]);
  local_constraint_.setTranslationConstraintDirection(direction);
}

} // namespace subvis

