#ifndef SUBVIS_VIEW_EDIT_CONSTRAINT_H
#define SUBVIS_VIEW_EDIT_CONSTRAINT_H

#include <QGLViewer/constraint.h>

namespace subvis {

class EditConstraint : public qglviewer::Constraint {
 public:
  EditConstraint();

 protected:
  void constrainTranslation(qglviewer::Vec& translation,
                            qglviewer::Frame* const frame) override;

  private:
  qglviewer::LocalConstraint local_constraint_;
};

} // namespace subvis
#endif // SUBVIS_VIEW_EDIT_CONSTRAINT_H
