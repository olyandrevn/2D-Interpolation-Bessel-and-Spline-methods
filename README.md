<h1 align="center">2D Interpolation. Bessel and Spline methods</h1>
<p>
</p>

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Qt](https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white)

## Overview
A C++ library for performing Bessel and spline interpolations, equipped with a Qt-based GUI for visualization and manipulation of data points.

<img src="img/Screenshot from 2023-11-08 14-34-43.png" width="595"/> <img src="img/Screenshot from 2023-11-08 14-35-43.png" width="595"/> 
<img src="img/Screenshot from 2023-11-08 14-37-49.png" width="595"/> <img src="img/Screenshot from 2023-11-08 14-38-03.png" width="595"/> 

## Key Features
- **Interpolation Methods**: Implements Bessel and cubic spline interpolation for data estimation.
- **Function Analysis**: Evaluates mathematical functions and derivatives for precise interpolation.
- **Graphical User Interface**: Features interactive visualization of data points and interpolation results.
- **Adaptive Parameters**: Allows dynamic adjustment of interval range and point density.
- **Error Visualization**: Displays discrepancies between interpolated values and actual function data.
- **Command-Line Interface**: Supports initial parameter configuration via CLI.

## Supported Functions
The `func` function can evaluate the following mathematical expressions based on the provided `func_id`:
- `func_id = 0`: `f(x) = 1`.
- `func_id = 1`: `f(x) = x`.
- `func_id = 2`: `f(x) = x^2`.
- `func_id = 3`: `f(x) = x^3`.
- `func_id = 4`: `f(x) = x^4`.
- `func_id = 5`: `f(x) = exp(x)`.
- `func_id = 6`: `f(x) = 1 / (25 * x^2 + 1)`.

  
## Install

```sh
git clone git@github.com:olyandrevn/2D-Interpolation-Bessel-and-Spline-methods.git
```

## Usage

```sh
./2D_interpolation [a] [b] [n] [func_id]
```

```[a] [b]```: interval for interpolation

```[n]```: number of points

```[func_id]```: function identifier for selection



## Run on default parameters

```sh
./2D_interpolation -10 10 5 0
```

## Author

👤 **Olga Kolomyttseva**

* Github: [@olyandrevn](https://github.com/olyandrevn)
* LinkedIn: [@olyandrevn](https://linkedin.com/in/olyandrevn)
