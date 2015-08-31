#ifndef SUBVIS_MAINWINDOW_H
#define SUBVIS_MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>

#include "view/viewer_mesh_widget.h"
#include "view/viewer_plugin_widget.h"
#include "controller/draw_controller.h"
#include "controller/io_controller.h"
#include "plugins/plugin_manager.h"

namespace subvis {

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(DrawController& draw_controller,
               IOController& io_controller,
               const std::map<QString, PluginWrapper>& plugins);
    ~MainWindow();

private:
    Ui::MainWindow* ui_;
    IOController& io_controller_;
    const std::map<QString, PluginWrapper>& plugins_;
    // memory managed by Qt's parent-relationship (ui)
    QLabel* status_label_;

    const QString kStatusText {"Build: " __DATE__ " " __TIME__};
    const QString kSaveDialogCaption {"Save model file"};
    const QString kLoadDialogCaption {"Load model file"};

    void setup_status_bar();
    void setup_viewer_tabs(DrawController& draw_controller);
    void setup_toolbar();
    void setup_plugin_guis();

public slots:
    void load_dialog();
    void save_dialog();
    void plugin_tab_changed(int current);

signals:
    void mesh_updated();
};


} // namespace SubVis
#endif // SUBVIS_MAINWINDOW_H
