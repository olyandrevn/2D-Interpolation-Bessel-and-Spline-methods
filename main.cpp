#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QPalette>
#include <QToolBar>
#include <QVBoxLayout>

#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("plastique");
    QFont f("Helvetica");
    f.setStyleHint(QFont::System);
    f.setPixelSize(18);
    f.setFamily("courier");
    f.setBold(2);
    qApp->setFont(f);

    QMainWindow *window = new QMainWindow;
    QMenuBar *menu_bar = new QMenuBar(window);
    QToolBar *tool_bar = new QToolBar(window);
    QLabel *log_label = new QLabel;
    QLabel *method_label = new QLabel;
    QAction *action;

    Window *graph_area = new Window(window, log_label, method_label);

    QPalette graph_pal(graph_area->palette());
    graph_pal.setColor(QPalette::Window, "Honeydew");
    graph_area->setAutoFillBackground(true);
    graph_area->setPalette(graph_pal);

    QPalette tool_pal(tool_bar->palette());
    tool_pal.setColor(QPalette::Window, "WhiteSmoke");
    tool_bar->setAutoFillBackground(true);
    tool_bar->setPalette(tool_pal);

    menu_bar->setMaximumHeight(30);
    tool_bar->setFixedWidth(400);

    if (graph_area->parse_command_line(argc, argv) != 0) {
        qWarning("Wrong input arguments!");
        delete menu_bar;
        return -1;
    }

    action = menu_bar->addAction("&Change function", graph_area,
                                 SLOT(change_func()));
    action->setShortcut(QString("0"));
    action = menu_bar->addAction("&Change method", graph_area,
                                 SLOT(change_method()));
    action->setShortcut(QString("1"));
    action = menu_bar->addAction("&Increase scale", graph_area,
                                 SLOT(increase_scale()));
    action->setShortcut(QString("2"));
    action = menu_bar->addAction("&Decrease scale", graph_area,
                                 SLOT(decrease_scale()));
    action->setShortcut(QString("3"));
    action = menu_bar->addAction("&Increase n", graph_area, SLOT(increase_n()));
    action->setShortcut(QString("4"));
    action = menu_bar->addAction("&Decrease n", graph_area, SLOT(decrease_n()));
    action->setShortcut(QString("5"));
    action = menu_bar->addAction("&Increase disturbance", graph_area,
                                 SLOT(increase_disturb()));
    action->setShortcut(QString("6"));
    action = menu_bar->addAction("&Decrease disturbance", graph_area,
                                 SLOT(decrease_disturb()));
    action->setShortcut(QString("7"));
    action = menu_bar->addAction("E&xit", window, SLOT(close()));
    action->setShortcut(QString("Ctrl+X"));

    window->setMenuBar(menu_bar);
    window->setCentralWidget(graph_area);
    window->addToolBar(Qt::RightToolBarArea, tool_bar);
    tool_bar->addWidget(log_label);
    tool_bar->addWidget(method_label);
    window->setWindowTitle("Graph");

    window->show();

    return app.exec();
}
