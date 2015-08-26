#ifndef SUBVIS_MAINWINDOW_H
#define SUBVIS_MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>

#include "view/viewer_mesh_widget.h"
#include "view/viewer_plugin_widget.h"
#include "controller/draw_controller.h"
#include "controller/io_controller.h"
#include "plugin_manager.h"

namespace SubVis {

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(DrawController& draw_controller,
               IOController& io_controller,
               PluginManager& plugin_manager);
    ~MainWindow();

    void create_plugin_guis();

private:
    Ui::MainWindow* ui_;
    IOController& io_controller_;
    PluginManager& plugin_manager_;
    // memory managed by Qt's parent-relationship (ui)
    QLabel* status_label_;

    const QString kStatusText {"Build: " __DATE__ " " __TIME__};
    const QString kSaveDialogCaption {"Save model file"};
    const QString kLoadDialogCaption {"Load model file"};

    void setup_status_bar();
    void setup_viewer_tabs(DrawController& draw_controller);
    void setup_toolbar();

public slots:
    void load_dialog();
    void save_dialog();
    void plugin_tab_changed(int current);

signals:
    void mesh_updated();
};


} // namespace SubVis
#endif // SUBVIS_MAINWINDOW_H
