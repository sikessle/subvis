#ifndef SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H
#define SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H

#include <QObject>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <string>

#include "plugins/subvis_plugin.h"
#include "plugins/subdivision/algorithm.h"

namespace SubdivisionPlugin {

class SubdivisionAlgorithms : public QObject, public subvis::SubVisPlugin
{
    Q_OBJECT

public:
    SubdivisionAlgorithms();

    virtual const QString id() override;
    virtual const QString name() override;
    virtual void set_draw_controller(subvis::DrawController* draw_controller) override;
    virtual void draw_opengl() override;
    virtual void create_gui(QWidget* parent) override;

private:
    subvis::DrawController* draw_controller_{nullptr};
    std::map<QString, std::unique_ptr<Algorithm>> algorithms_;

    QPushButton* subdivide_{nullptr};
    QSpinBox* steps_{nullptr};
    QComboBox* dropdown_{nullptr};

    std::unique_ptr<Algorithm>& active_algorithm();

private slots:
    void subdivide_clicked(bool);
};

} // namespace SubdivisionPlugin
#endif // SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H
