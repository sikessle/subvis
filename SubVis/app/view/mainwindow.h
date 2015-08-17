#ifndef SUBVIS_VIEW_MAINWINDOW_H
#define SUBVIS_VIEW_MAINWINDOW_H

#include "QMainWindow"
#include "QLabel"
#include "QString"
#include "view/mesh_render_widget.h"
#include "view/plugin_render_widget.h"

namespace SubVis {
namespace View {

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    const QString STATUS_TEXT {"Version: Alpha - HTWG Konstanz"};
    const QString TAB_RENDER_MESH_TEXT {"Mesh"};
    const QString TAB_RENDER_PLUGIN_TEXT {"Plugin specific"};

    QLabel *status_label;
    MeshRenderWidget *mesh_render_widget;
    PluginRenderWidget *plugin_render_widget;

    void setup_status_bar();
    void setup_render_tabs();
    void setup_plugin_tabs();
};


} // namespace View
} // namespace SubVis
#endif // SUBVIS_VIEW_MAINWINDOW_H
