#ifndef SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H
#define SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H

#include <QObject>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <string>

#include "plugins/subvis_plugin.h"
#include "plugins/subdivision/algorithm.h"

namespace subdivision {

class SubdivisionAlgorithmsPlugin : public QObject, public subvis::SubVisPlugin
{
    Q_OBJECT

public:
    SubdivisionAlgorithmsPlugin();

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

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_SUBDIVISON_ALGORITHMS_PLUGIN_H
