#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <cmath>
#include <cstdio>
#include <vector>

enum class interpolation_method {
    origin,
    bessel,
    spline,
    error_bessel,
    error_spline,
};

double func(int func_id, double x);

class interpolation
{
  private:
    const double eps = 1e-14;
    const double bound_shift = 1e-13;

    double a = 0.;
    double b = 0.;
    int n = 0;
    int func_id = 0;
    int disturb = 0;

    std::vector<double> x;
    std::vector<double> f_x;
    std::vector<double> d;
    std::vector<double> bessel_coeffs;
    std::vector<double> spline_coeffs;

    std::vector<double> diag;
    std::vector<double> up_diag;
    std::vector<double> low_diag;
    std::vector<double> ans;

    void update_bessel_coeffs();
    void update_spline_coeffs();
    static void solve(std::vector<double> &d, std::vector<double> &a,
                      std::vector<double> &c, std::vector<double> &b, int n);

  public:
    int loc_n = 4;
    const int n_bessel_max = 50;
    const int n_spline_min = 3;
    interpolation(double a, double b, int n, int func_id);
    ~interpolation() = default;

    double max_value() const;
    void change_n(int n);
    void change_func(int func_id);
    void increase_disturb();
    void decrease_disturb();
    void increase_scale();
    void decrease_scale();

    double bessel(double x);
    double bessel_error(double x);

    int binary_search(double x);
    void lagrange_polynom(double *lag_x, double *lag_f_x, int k) const;
    static double derivative_lagrange_polynom(const double *lag_x,
                                              double *lag_f_x, double x);
    double spline(double x);
    double spline_error(double x);

    double get_value(double x, interpolation_method method);
};

#endif // INTERPOLATION_H
