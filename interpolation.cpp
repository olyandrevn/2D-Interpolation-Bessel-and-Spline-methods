#include "interpolation.h"
#include <cmath>

double func(int func_id, double x)
{
    switch (func_id) {
    case 0:
        return 1.;
    case 1:
        return x;
    case 2:
        return x * x;
    case 3:
        return x * x * x;
    case 4:
        return x * x * x * x;
    case 5:
        return exp(x);
    case 6:
        return 1. / (25. * x * x + 1.);
    default:
        return 0.;
    }
}

double func_2derivative(int func_id, double x)
{
    switch (func_id) {
    case 0:
        return 0.;
    case 1:
        return 0.;
    case 2:
        return 2.;
    case 3:
        return 6. * x;
    case 4:
        return 12. * x * x;
    case 5:
        return exp(x);
    case 6:
        return -50. / (25. * x * x + 1.) / (25. * x * x + 1.) +
               5000 * x * x / (25. * x * x + 1.) / (25. * x * x + 1.) /
                   (25. * x * x + 1.);
    }
    return 0.;
}

interpolation::interpolation(double new_a, double new_b, int new_n,
                             int new_func_id)
{
    a = new_a;
    b = new_b;
    n = new_n;
    func_id = new_func_id;
    disturb = 0;

    x.resize(n);
    f_x.resize(n);
    d.resize(n);
    bessel_coeffs.resize(4 * (n - 1));
    spline_coeffs.resize(4 * (n - 1));

    double step = (b - a) / (n - 1);
    for (int i = 0; i < n; i++) {
        x[i] = a + i * step;
        f_x[i] = func(func_id, a + i * step);
    }

    update_bessel_coeffs();
    update_spline_coeffs();
}

void interpolation::update_bessel_coeffs()
{
    double tmp1;
    double tmp2;

    for (int i = 1; i < n - 1; i++) {
        tmp1 = (f_x[i] - f_x[i - 1]) / (x[i] - x[i - 1]);
        tmp2 = (f_x[i + 1] - f_x[i]) / (x[i + 1] - x[i]);
        d[i] = ((x[i + 1] - x[i]) * tmp1 + (x[i] - x[i - 1]) * tmp2) /
               (x[i + 1] - x[i - 1]);
    }

    tmp1 = (f_x[1] - f_x[0]) / (x[1] - x[0]);
    tmp2 = (f_x[n - 1] - f_x[n - 2]) / (x[n - 1] - x[n - 2]);

    d[0] = 0.5 * (3 * tmp1 - d[1] -
                  0.5 * func_2derivative(func_id, x[0]) * (x[1] - x[0]));
    d[n - 1] = 0.5 * (3 * tmp2 - d[n - 2] +
                      0.5 * func_2derivative(func_id, x[n - 1]) *
                          (x[n - 1] - x[n - 2]));

    int j = 0;

    for (int i = 0; i < n - 1; i++) {
        bessel_coeffs[j + 0] = f_x[i];
        bessel_coeffs[j + 1] = d[i];
        tmp2 = x[i + 1] - x[i];
        tmp1 = (f_x[i + 1] - f_x[i]) / tmp2;
        bessel_coeffs[j + 2] = (3 * tmp1 - 2 * d[i] - d[i + 1]) / tmp2;
        tmp2 *= tmp2;
        bessel_coeffs[j + 3] = (d[i] + d[i + 1] - 2 * tmp1) / tmp2;
        j += 4;
    }
}

double interpolation::bessel(double new_x)
{
    int i;
    for (i = 0; i < n - 2; i++) {
        if (new_x <= x[i + 1]) {
            break;
        }
    }

    return bessel_coeffs[4 * i] + bessel_coeffs[4 * i + 1] * (new_x - x[i]) +
           bessel_coeffs[4 * i + 2] * (new_x - x[i]) * (new_x - x[i]) +
           bessel_coeffs[4 * i + 3] * (new_x - x[i]) * (new_x - x[i]) *
               (new_x - x[i]);
}

void interpolation::lagrange_polynom(double *lag_x, double *lag_f_x,
                                     int k) const
{
    for (int j = 0; j < k - 1; j++) {
        for (int i = k - 1; i > j; i--) {
            if (fabs(lag_x[i] - lag_x[i - j - 1]) >
                eps * fmax(fabs(lag_x[i]), fabs(lag_x[i - j - 1]))) {
                lag_f_x[i] = (lag_f_x[i] - lag_f_x[i - 1]) /
                             (lag_x[i] - lag_x[i - j - 1]);
            }
        }
    }
}

double interpolation::derivative_lagrange_polynom(const double *lag_x,
                                                  double *lag_f_x, double x)
{
    return lag_f_x[1] + (2 * x - (lag_x[0] + lag_x[1])) * lag_f_x[2] +
           (3 * x * x - 2 * (lag_x[0] + lag_x[1] + lag_x[2]) * x +
            (lag_x[0] * lag_x[1] + lag_x[0] * lag_x[2] + lag_x[1] * lag_x[2])) *
               lag_f_x[3];
}

void interpolation::solve(std::vector<double> &d, std::vector<double> &a,
                          std::vector<double> &c, std::vector<double> &b, int n)
{
    c[0] = c[0] / a[0];

    for (int i = 0; i < n - 2; i++) {
        a[i + 1] = a[i + 1] - d[i] * c[i];
        c[i + 1] = c[i + 1] / a[i + 1];
    }

    a[n - 1] = a[n - 1] - d[n - 2] * c[n - 2];

    b[0] = b[0] / a[0];
    for (int i = 1; i < n; i++) {
        b[i] = (b[i] - d[i - 1] * b[i - 1]) / a[i];
    }

    for (int i = n - 2; i >= 0; i--) {
        b[i] = b[i] - c[i] * b[i + 1];
    }
}

void interpolation::update_spline_coeffs()
{
    if (n > 1 && fabs(x[1] - x[0]) <= eps) {
        return;
    }

    if (n < n_spline_min) {
        return;
    }

    double step = (b - a) / (n - 1);

    double lag_f_x[4];
    double lag_x[4];

    for (int i = 0; i < loc_n; i++) {
        lag_x[i] = a + i * step;
        lag_f_x[i] = func(func_id, lag_x[i]);
    }
    lagrange_polynom(lag_x, lag_f_x, loc_n);
    double der_Q_k = derivative_lagrange_polynom(lag_x, lag_f_x, a);

    double xi = b - (loc_n - 1) * step;
    for (int i = 0; i < loc_n; i++) {
        lag_x[i] = xi;
        lag_f_x[i] = func(func_id, xi);
        xi += step;
    }
    lagrange_polynom(lag_x, lag_f_x, loc_n);
    double der_R_k = derivative_lagrange_polynom(lag_x, lag_f_x, b);

    std::vector<double> diag;
    std::vector<double> up_diag;
    std::vector<double> low_diag;
    std::vector<double> ans;

    diag.resize(n);
    up_diag.resize(n);
    low_diag.resize(n);
    ans.resize(n);

    diag[0] = 1.;
    up_diag[0] = 0.;
    ans[0] = der_Q_k;
    ans[n - 1] = der_R_k;

    for (int i = 1; i < n - 1; i++) {
        diag[i] = 4. * step;
        up_diag[i] = 1. * step;
        low_diag[i - 1] = 1. * step;
        ans[i] = 3. * (f_x[i + 1] - f_x[i - 1]);
    }

    diag[n - 1] = 1.;
    low_diag[n - 2] = 0.;

    solve(low_diag, diag, up_diag, ans, n);

    for (int i = 0; i < n - 1; i++) {
        spline_coeffs[4 * i + 0] = f_x[i];
        spline_coeffs[4 * i + 1] = ans[i];
        spline_coeffs[4 * i + 2] =
            ((f_x[i + 1] - f_x[i]) / step - ans[i]) / step;
        spline_coeffs[4 * i + 3] =
            (ans[i] + ans[i + 1] - 2. * (f_x[i + 1] - f_x[i]) / step) / step /
            step;
    }
}

int interpolation::binary_search(double curr_x)
{
    int left = 0;
    int right = n - 1;
    int medium;
    while (right - left > 1) {
        medium = (left + right) / 2;
        if (curr_x >= x[medium] && curr_x <= x[medium + 1]) {
            return medium;
        }
        if (curr_x >= x[medium]) {
            left = medium;
        }
        if (curr_x <= x[medium]) {
            right = medium;
        }
    }
    return left;
}

double interpolation::spline(double new_x)
{
    if (n > 1 && fabs(x[1] - x[0]) <= eps) {
        return 0.;
    }

    int i = binary_search(new_x);

    i = i < 0 ? 0 : i;
    i = i > n - 2 ? n - 2 : i;

    double tmp = new_x - x[i];
    return spline_coeffs[4 * i] + spline_coeffs[4 * i + 1] * tmp +
           spline_coeffs[4 * i + 2] * tmp * tmp +
           spline_coeffs[4 * i + 3] * tmp * tmp * (new_x - x[i + 1]);
}

double interpolation::bessel_error(double x)
{
    return func(func_id, x) - bessel(x);
}

double interpolation::spline_error(double x)
{
    return func(func_id, x) - spline(x);
}

double interpolation::max_value() const
{
    if (func_id == 0) {
        return 1.0;
    } else if (func_id == 5) {
        return func(func_id, b);
    } else if (func_id == 6) {
        if (a * b < 0) {
            return func(func_id, 0);
        }
        double max_val;
        if (fabs(a) < fabs(b)) {
            max_val = fabs(func(func_id, a));
        } else {
            max_val = fabs(func(func_id, b));
        }

        return max_val;
    } else {
        double max_val;
        if (fabs(a) > fabs(b)) {
            max_val = fabs(func(func_id, a));
        } else {
            max_val = fabs(func(func_id, b));
        }

        return max_val;
    }
}

void interpolation::change_func(int new_func_id)
{
    if (new_func_id == func_id) {
        return;
    }

    func_id = new_func_id;

    for (int i = 0; i < n; i++) {
        f_x[i] = func(func_id, x[i]);
    }

    update_bessel_coeffs();
    update_spline_coeffs();
}

void interpolation::change_n(int new_n)
{
    if (new_n == n) {
        return;
    }

    n = new_n;
    x.resize(n);
    f_x.resize(n);
    d.resize(n);
    bessel_coeffs.resize(4 * (n - 1));
    spline_coeffs.resize(4 * (n - 1));

    double step = (b - a) / (n - 1);
    for (int i = 0; i < n; i++) {
        x[i] = a + i * step;
        f_x[i] = func(func_id, a + i * step);
    }

    update_bessel_coeffs();
    update_spline_coeffs();
}

void interpolation::increase_disturb()
{
    disturb++;
    f_x[n / 2] = func(func_id, x[n / 2]) + disturb * 0.1 * max_value();

    update_bessel_coeffs();
    update_spline_coeffs();
}

void interpolation::decrease_disturb()
{
    disturb--;
    f_x[n / 2] = func(func_id, x[n / 2]) + disturb * 0.1 * max_value();

    update_bessel_coeffs();
    update_spline_coeffs();
}

void interpolation::increase_scale()
{
    a /= 2;
    b /= 2;

    double step = (b - a) / (n - 1);
    for (int i = 0; i < n; i++) {
        x[i] = a + i * step;
        f_x[i] = func(func_id, a + i * step);
    }

    update_bessel_coeffs();
    update_spline_coeffs();
}

void interpolation::decrease_scale()
{
    a *= 2;
    b *= 2;

    double step = (b - a) / (n - 1);
    for (int i = 0; i < n; i++) {
        x[i] = a + i * step;
        f_x[i] = func(func_id, a + i * step);
    }

    update_bessel_coeffs();
    update_spline_coeffs();
}

double interpolation::get_value(double x, interpolation_method method)
{
    switch (method) {
    case interpolation_method::origin:
        return func(func_id, x);
    case interpolation_method::bessel:
        return bessel(x);
    case interpolation_method::spline:
        return spline(x);
    case interpolation_method::error_bessel:
        return bessel_error(x);
    case interpolation_method::error_spline:
        return spline_error(x);
    }
    return 0;
}
