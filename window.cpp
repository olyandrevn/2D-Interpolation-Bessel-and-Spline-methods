#include <QPainter>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include "interpolation.h"
#include "window.h"

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 10

void Window::func_name()
{
    switch (func_id) {
    case 0:
        f_name = "f(x) = 1";
        break;
    case 1:
        f_name = "f(x) = x";
        break;
    case 2:
        f_name = "f(x) = x^2";
        break;
    case 3:
        f_name = "f(x) = x^3";
        break;
    case 4:
        f_name = "f(x) = x^4";
        break;
    case 5:
        f_name = "f(x) = e^x";
        break;
    case 6:
        f_name = "f(x) = 1/(25x^2 + 1)";
        break;
    default:
        f_name = "";
    }
}

Window::Window(QWidget *parent, QLabel *log_lab, QLabel *method_lab)
    : QWidget(parent), log_label(log_lab), method_label(method_lab)
{
    a = DEFAULT_A;
    b = DEFAULT_B;
    n = DEFAULT_N;

    func_id = 0;
    func_name();
}

QSize Window::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize Window::sizeHint() const
{
    return QSize(1000, 1000);
}

int Window::parse_command_line(int argc, char *argv[])
{
    if (argc != 5 || sscanf(argv[1], "%lf", &a) != 1 ||
        sscanf(argv[2], "%lf", &b) != 1 || b - a < 1.e-6 ||
        (argc > 3 && sscanf(argv[3], "%d", &n) != 1) || n <= 0 ||
        sscanf(argv[4], "%d", &func_id) != 1) {
        printf("Usage %s a b n k\n", argv[0]);

        return 1;
    }

    f = new interpolation(a, b, n, func_id);

    return 0;
}

Window::~Window()
{
    if (f) {
        delete f;
    }
}

void Window::change_func()
{
    func_id = (func_id + 1) % 7;
    f->change_func(func_id);
    func_name();

    update();
}

void Window::change_method()
{
    switch (method) {
    case draw_method::bessel:
        method = draw_method::spline;
        break;
    case draw_method::spline:
        method = draw_method::both;
        break;
    case draw_method::both:
        method = draw_method::errors;
        break;
    case draw_method::errors:
        method = draw_method::bessel;
        break;
    }

    update();
}

void Window::increase_scale()
{
    a /= 2;
    b /= 2;
    scale /= 2;
    f->increase_scale();

    update();
}

void Window::decrease_scale()
{
    a *= 2;
    b *= 2;
    scale *= 2;
    f->decrease_scale();

    update();
}

void Window::increase_n()
{
    n *= 2;
    f->change_n(n);

    update();
}

void Window::decrease_n()
{
    if (n > 3) {
        n /= 2;
        f->change_n(n);
    }

    update();
}

void Window::increase_disturb()
{
    disturb++;
    f->increase_disturb();

    update();
}

void Window::decrease_disturb()
{
    disturb--;
    f->decrease_disturb();

    update();
}

void Window::change_label(double y_min, double y_max)
{
    QPen pen;
    std::string log_lab;
    std::string method_lab;

    log_lab = "func_id = " + std::to_string(func_id) + "\n";
    log_lab = log_lab + f_name + "\n";
    double f_max = fmax(y_min, y_max);
    log_lab = log_lab + "|f(x)| = " + std::to_string(f_max) + "\n";
    log_lab = log_lab + "scale = " + std::to_string(scale) + "\n";
    log_lab = log_lab + "n = " + std::to_string(n) + "\n";
    log_lab = log_lab + "disturbance = " + std::to_string(disturb) + "\n";

    log_label->setText(QString::fromStdString(log_lab));

    switch (method) {
    case draw_method::bessel:
        method_lab = method_lab + "<br> <FONT COLOR=Black>origin</FONT>" +
                     "<br>" + "<FONT COLOR=Indigo>bessel</FONT>" + "<br>";
        break;
    case draw_method::spline:
        method_lab = method_lab + "<br> <FONT COLOR=Black>origin</FONT>" +
                     "<br>" + "<FONT COLOR=ForestGreen>spline</FONT>" + "<br>";
        break;
    case draw_method::both:
        method_lab = method_lab + "<br> <FONT COLOR=Black>origin" + "<br>" +
                     "<FONTCOLOR=Indigo>bessel</FONT>" + "<br>" + "and" +
                     "<br>" + "<FONT COLOR=ForestGreen>spline</FONT>" + "<br>";
        break;
    case draw_method::errors:
        method_lab = method_lab +
                     "<br> <FONT COLOR=Orange>bessel error</FONT>" + "<br>" +
                     "and" + "<br>" +
                     "<FONT COLOR=OrangeRed>spline error</FONT>" + "<br>";
        break;
    }

    method_label->setText(QString::fromStdString(method_lab));
}

QPointF Window::local2global(double x_loc, double y_loc, double y_min,
                             double y_max)
{
    double x_global = (x_loc - a) / (b - a) * width();
    double y_global = (y_max - y_loc) / (y_max - y_min) * height();
    return QPointF(x_global, y_global);
}

void Window::draw_axes(QPainter &painter, double min, double max)
{
    QPen pen;
    pen.setWidth(1);
    pen.setColor("grey");
    painter.setPen(pen);

    QLineF x_axe;
    x_axe.setP1(local2global(a, 0, min, max));
    x_axe.setP2(local2global(b, 0, min, max));

    QLineF y_axe;
    y_axe.setP1(local2global(0, min, min, max));
    y_axe.setP2(local2global(0, max, min, max));

    painter.drawLine(x_axe);
    painter.drawLine(y_axe);
}

void Window::draw_func(QPainter &painter, interpolation *f,
                       interpolation_method method, double y_min, double y_max,
                       double delta_x)
{
    QPen pen;
    pen.setWidth(3);

    switch (method) {
    case interpolation_method::origin:
        pen.setColor("Black");
        break;
    case interpolation_method::bessel:
        pen.setColor("Indigo");
        break;
    case interpolation_method::spline:
        pen.setColor("ForestGreen");
        break;
    case interpolation_method::error_bessel:
        pen.setColor("Orange");
        break;
    case interpolation_method::error_spline:
        pen.setColor("OrangeRed");
        break;
    }
    painter.setPen(pen);

    QLineF line;
    double x1 = a;
    double y1 = f->get_value(x1, method);
    double x2 = 0;
    double y2 = 0;

    for (x2 = x1 + delta_x; x2 - b < eps; x2 += delta_x) {
        y2 = f->get_value(x2, method);

        // local coords to global coords
        line.setP1(local2global(x1, y1, y_min, y_max));
        line.setP2(local2global(x2, y2, y_min, y_max));

        painter.drawLine(line);
        x1 = x2;
        y1 = y2;
    }

    x2 = b;
    y2 = f->get_value(x2, method);

    line.setP1(local2global(x1, y1, y_min, y_max));
    line.setP2(local2global(x2, y2, y_min, y_max));

    painter.drawLine(line);
}

void Window::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    double y_min;
    double y_max;
    double delta_x;
    double delta_y;
    double x;
    double y;
    int x_width = width();

    delta_x = (b - a) / x_width;
    y_min = eps;
    y_max = -eps;

    // calculate min and max for current function
    for (x = a; x - b < eps; x += delta_x) {
        if (method == draw_method::bessel) {
            if (n < f->n_bessel_max) {
                y = f->get_value(x, interpolation_method::bessel);
                y_max = fmax(y_max, y);
                y_min = fmin(y_min, y);
            }

            y = f->get_value(x, interpolation_method::origin);
            y_max = fmax(y_max, y);
            y_min = fmin(y_min, y);
        }
        if (method == draw_method::spline) {
            y = f->get_value(x, interpolation_method::spline);
            y_max = fmax(y_max, y);
            y_min = fmin(y_min, y);

            y = f->get_value(x, interpolation_method::origin);
            y_max = fmax(y_max, y);
            y_min = fmin(y_min, y);
        }
        if (method == draw_method::both) {
            if (n < f->n_bessel_max) {
                y = f->get_value(x, interpolation_method::bessel);
                y_max = fmax(y_max, y);
                y_min = fmin(y_min, y);
            }

            y = f->get_value(x, interpolation_method::spline);
            y_max = fmax(y_max, y);
            y_min = fmin(y_min, y);

            y = f->get_value(x, interpolation_method::origin);
            y_max = fmax(y_max, y);
            y_min = fmin(y_min, y);
        }
        if (method == draw_method::errors) {
            y = f->get_value(x, interpolation_method::error_bessel);
            y_max = fmax(y_max, y);
            y_min = fmin(y_min, y);

            y = f->get_value(x, interpolation_method::error_spline);
            y_max = fmax(y_max, y);
            y_min = fmin(y_min, y);
        }
    }

    if (fabs(y_max - y_min) <= eps) {
        y_max += eps;
        y_min -= eps;
    }

    delta_y = 0.01 * (y_max - y_min);
    y_min -= delta_y;
    y_max += delta_y;

    painter.save();

    draw_axes(painter, y_min, y_max);

    if (method == draw_method::bessel && n < f->n_bessel_max) {
        draw_func(painter, f, interpolation_method::bessel, y_min, y_max,
                  delta_x);
        draw_func(painter, f, interpolation_method::origin, y_min, y_max,
                  delta_x);
    }
    if (method == draw_method::spline) {
        draw_func(painter, f, interpolation_method::spline, y_min, y_max,
                  delta_x);
        draw_func(painter, f, interpolation_method::origin, y_min, y_max,
                  delta_x);
    }
    if (method == draw_method::both) {
        if (n < f->n_bessel_max) {
            draw_func(painter, f, interpolation_method::bessel, y_min, y_max,
                      delta_x);
        }
        draw_func(painter, f, interpolation_method::spline, y_min, y_max,
                  delta_x);
        draw_func(painter, f, interpolation_method::origin, y_min, y_max,
                  delta_x);
    }
    if (method == draw_method::errors) {
        draw_func(painter, f, interpolation_method::error_bessel, y_min, y_max,
                  delta_x);
        draw_func(painter, f, interpolation_method::error_spline, y_min, y_max,
                  delta_x);
    }
    painter.restore();
    change_label(y_min, y_max);
}
