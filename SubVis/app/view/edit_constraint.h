#ifndef SUBVIS_VIEW_EDIT_CONSTRAINT_H
#define SUBVIS_VIEW_EDIT_CONSTRAINT_H

#include <QGLViewer/constraint.h>

namespace subvis {

class EditConstraint : public qglviewer::Constraint {
 public:
  EditConstraint();
  void set_vertex_normal(const qglviewer::Vec& normal);
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
