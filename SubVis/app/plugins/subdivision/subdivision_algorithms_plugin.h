#ifndef SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H
#define SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H

#include <QObject>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <string>

#include "plugins/subvis_plugin.h"
#include "plugins/subdivision/sd_algorithm.h"
#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

struct AlgorithmRenderer {
  std::unique_ptr<SubdivAlgorithm> algorithm; 
  /**
   * @brief shared_ptr to allow multiple algorithms to use the same renderer
   */
  std::shared_ptr<GLRenderer> renderer;
};

class SubdivisionAlgorithmsPlugin : public QObject,
  public subvis::SubVisPlugin {
  Q_OBJECT

 public:
  SubdivisionAlgorithmsPlugin();

  virtual const QString id() const override;
  virtual const QString name() const override;
  virtual void set_draw_controller(subvis::DrawController& draw_controller)
  override;
  virtual void draw_opengl() override;
  virtual void create_gui(QWidget* parent) override;

 private:
  subvis::DrawController* draw_controller_{nullptr};
  /**
   * @brief name->(algorithm, renderer)
   */
  std::map<const QString, AlgorithmRenderer> algorithms_;

  // memory managed by Qt's parent-relationship
  QPushButton* subdivide_{nullptr};
  QSpinBox* steps_{nullptr};
  QComboBox* dropdown_{nullptr};

  AlgorithmRenderer& active_algorithm_renderer_pair();

 private slots:
  void subdivide_clicked(bool);
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H
