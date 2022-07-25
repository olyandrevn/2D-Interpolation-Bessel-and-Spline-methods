#ifndef WINDOW_H
#define WINDOW_H

#include "interpolation.h"
#include <QLabel>
#include <QWidget>

enum class draw_method {
    bessel,
    spline,
    both,
    errors,
};

class Window : public QWidget
{
    Q_OBJECT

  private:
    int func_id;
    std::string f_name;
    double a;
    double b;
    int n;
    int disturb = 0;
    int scale = 1;
    double eps = 1e-14;
    interpolation *f = nullptr;
    draw_method method = draw_method::bessel;

    QLabel *log_label;
    QLabel *method_label;

  public:
    Window(QWidget *parent, QLabel *log_lab, QLabel *method_lab);
    ~Window();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void func_name();
    int parse_command_line(int argc, char *argv[]);
    QPointF local2global(double x_loc, double y_loc, double y_min,
                         double y_max);
    void draw_axes(QPainter &painter, double min, double max);
    void draw_func(QPainter &painter, interpolation *f,
                   interpolation_method type, double min, double max,
                   double delta_x);
    void change_label(double y_min, double y_max);

  public slots:
    void change_func();
    void change_method();
    void increase_scale();
    void decrease_scale();
    void increase_n();
    void decrease_n();
    void increase_disturb();
    void decrease_disturb();

  protected:
    void paintEvent(QPaintEvent *event);
};

#endif
