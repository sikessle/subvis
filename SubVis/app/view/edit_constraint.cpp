#include "view/edit_constraint.h"

namespace subvis {

EditConstraint::EditConstraint() {
  local_constraint_.setTranslationConstraintType(qglviewer::AxisPlaneConstraint::AXIS);
  // X axis
  // TODO: Try to use normal of Vertex.
  local_constraint_.setTranslationConstraintDirection(qglviewer::Vec(1.0, 0.0, 0.0));
  // SWITCH ON KEY PRESS (x, y and z)
}

void EditConstraint::constrainTranslation(qglviewer::Vec& translation,
                                     qglviewer::Frame* const frame) {
  local_constraint_.constrainTranslation(translation, frame);
}

} // namespace subvis

