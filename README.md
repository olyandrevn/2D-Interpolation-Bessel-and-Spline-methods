<h1 align="center">2D Interpolation. Bessel and Spline methods</h1>
<p>
</p>

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Qt](https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white)

## Overview
This Qt-based application renders 2D visualizations of mathematical functions, employing both Bessel and spline interpolations for creating smooth curves. It utilizes the capabilities of Qt for drawing and offers an interactive interface for users to engage with and adjust mathematical plots in real-time.


<img src="img/Screenshot from 2023-11-08 14-34-43.png" width="295"/> <img src="img/Screenshot from 2023-11-08 14-35-43.png" width="295"/> 
<img src="img/Screenshot from 2023-11-08 14-37-49.png" width="295"/> <img src="img/Screenshot from 2023-11-08 14-38-03.png" width="295"/> 


## Key Features
- **2D Rendering**: Visualizes mathematical functions in a 2D space.
- **Dynamic Visualization**: Change the rendered functions and methods via GUI controls.
- **User Interface**: Utilizes Qt's `QAction`, `QMenuBar`, and `QToolBar` for in-app options.
- **Scene Customization**: Adjust parameters such as function type, disturbance levels, number of points, scale.
- **Command-Line Arguments**: Supports initialization parameters through the command line for advanced users.

## Supported Functions

The application can visualize the following mathematical functions in 3D space, selectable by the `func_id` parameter:

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
qmake
make
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
