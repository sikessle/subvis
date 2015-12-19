#include "view/edit_constraint.h"

namespace subvis {

EditConstraint::EditConstraint() {
  local_constraint_.setTranslationConstraintType(
    qglviewer::AxisPlaneConstraint::PLANE);
}

void EditConstraint::constrainTranslation(qglviewer::Vec& translation,
    qglviewer::Frame* const frame) {
  local_constraint_.constrainTranslation(translation, frame);
}

void EditConstraint::set_vertex_normal(const qglviewer::Vec& normal) {
  vertex_normal_ = normal;
  qglviewer::Vec direction {vertex_normal_};

  if (orthogonal_) {
    direction = vertex_normal_.orthogonalVec();
  }

  local_constraint_.setTranslationConstraintDirection(direction);
}

void EditConstraint::set_plane_orthogonal(bool orthogonal) {
  orthogonal_ = orthogonal;
  set_vertex_normal(vertex_normal_);
}

} // namespace subvis

