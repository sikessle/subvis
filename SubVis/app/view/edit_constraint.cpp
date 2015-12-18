#include "view/edit_constraint.h"

namespace subvis {

EditConstraint::EditConstraint() {
  local_constraint_.setTranslationConstraintType(qglviewer::AxisPlaneConstraint::AXIS);
  //local_constraint_.setTranslationConstraintDirection(qglviewer::Vec(0.0, 0.0, 0.0));
}

void EditConstraint::constrainTranslation(qglviewer::Vec& translation,
                                     qglviewer::Frame* const frame) {
  local_constraint_.constrainTranslation(translation, frame);
}

} // namespace subvis

