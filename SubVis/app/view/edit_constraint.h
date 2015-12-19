#ifndef SUBVIS_VIEW_EDIT_CONSTRAINT_H
#define SUBVIS_VIEW_EDIT_CONSTRAINT_H

#include <QGLViewer/constraint.h>

namespace subvis {

///
/// \brief A constraint which can be used to constraint a manipulated frame
/// during edits. Supports the constraint of the translations of a frame
/// to a plane which is characterized by a normal.
///
class EditConstraint : public qglviewer::Constraint {
 public:
  EditConstraint();
  ///
  /// \brief Sets the normal of the vertex which is edited. This enforces
  /// the frame to translate only on the plane which the normal defines.
  /// \param normal The normal which defines the plane
  ///
  void set_vertex_normal(const qglviewer::Vec& normal);
  ///
  /// \brief Sets the plane to orthogonal or regular mode. In regular mode
  /// the plane defined by the normal is used. In orthogonal mode the normal
  /// is used to calculate a orthogonal vector and use its plane as constraint.
  /// \param orthogonal False for regular mode or true for orthogonal mode.
  ///
  void set_plane_orthogonal(bool orthogonal);

 protected:
  void constrainTranslation(qglviewer::Vec& translation,
                            qglviewer::Frame* const frame) override;

 private:
  qglviewer::LocalConstraint local_constraint_;
  bool orthogonal_ {false};
  qglviewer::Vec vertex_normal_;
};

} // namespace subvis
#endif // SUBVIS_VIEW_EDIT_CONSTRAINT_H
