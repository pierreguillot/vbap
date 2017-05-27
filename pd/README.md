## VBAP for Pd
This part of the repository contains the [Pure Data](http://msp.ucsd.edu/software.html) implementation of the vector based amplitude panning. The *abs* folder contains the implementation as abstractions and the *ext* folder contains the implementation as externals.

### Contents

- *p.vbap*: An abstraction that outputs the coefficients for a set of loudspeakers depending on the direction of the virtual source. The abstraction works in 2d and 3d.
- *vbap.coeff*: An external that outputs the coefficients for a set of loudspeakers depending on the direction of the virtual source. The external works in 2d and 3d.
- *vbap~*: An external that generates the signals for a set of loudspeakers depending on a sound source and its direction. It can be used for real-time processing. The external works in 2d and 3d.

### Compilation

Use the basic CMake approach:
```
mkdir build
cd build
cmake ..
cmake --build .
```

### Author

Pierre Guillot | CICM | Université Paris 8
