*This project has been created as part of the 42 curriculum by bkelav.*

## Description

Fract'ol is a computer graphics project designed to render mathematically generated fractals, specifically the Mandelbrot set and Julia set. The program uses the MLX42 graphics library to handle window management and pixel pushing, and is optimized to efficiently execute millions of complex floating-point calculations per frame.

## Table of Contents
* [Mathematical Foundation and Loop Optimization](#mathematical-foundation-and-loop-optimization)
* [Algorithmic Reductions (Mandelbrot)](#algorithmic-reductions-mandelbrot)
* [Fractal Variants](#fractal-variants)
* [Rendering Engine and MLX42 Integration](#rendering-engine-and-mlx42-integration)
* [Event Hook Logic](#event-hook-logic)
* [Instructions](#instructions)
  * [Compilation](#compilation)
  * [Execution](#execution)
  * [Controls](#controls)
  * [Memory Testing (Valgrind)](#memory-testing-valgrind)
* [Resources](#resources)

### Mathematical Foundation and Loop Optimization

The core generation of these fractals relies on the continuous iteration of the quadratic polynomial:
$$f(z) = z^2 + c$$
Where $z$ and $c$ are complex numbers in the form $x + yi$. To compute $z_{n+1}$, the function is expanded algebraically into its real and imaginary components:
$$z_{n+1} = (z_r^2 - z_i^2 + c_r) + (2 z_r z_i + c_i) i$$

**Caching Squares:**
Computing $z_r^2$ and $z_i^2$ multiple times per loop iteration introduces significant processing overhead. To optimize this, the squared values are cached in a structured variable (`z2.r` and `z2.i`) at the end of each iteration sequence. These pre-calculated squares are then directly applied to the real component calculation of the subsequent iteration, reducing the total number of floating-point multiplications per loop.

**Escape Condition Optimization:**
A complex coordinate mathematically escapes the fractal set if its magnitude exceeds 2. The standard magnitude formula is:
$$|z| = \sqrt{z_r^2 + z_i^2}$$
To bypass the computationally expensive `sqrt()` function, the inequality $|z| \leq 2$ is squared on both sides, yielding $z_r^2 + z_i^2 \leq 4$. By utilizing the previously cached `z2` values, the loop's escape condition evaluates strictly via a rapid addition operation: `z2.r + z2.i <= 4.0`.

### Algorithmic Reductions (Mandelbrot)

Points located deep within the Mandelbrot set will iterate until the `max_iterations` limit is reached, consuming maximum CPU cycles. To mitigate this, geometric boundary checks and periodicity tracking are implemented to mathematically guarantee a point's inclusion within the set before iterating.

**Main Cardioid Bounds Checking:**
The central body of the Mandelbrot set forms a cardioid. A complex point $c$ is determined to be within this cardioid if it satisfies the following equation:
$$q = \left(c_r - \frac{1}{4}\right)^2 + c_i^2$$
$$q\left(q + \left(c_r - \frac{1}{4}\right)\right) \leq \frac{1}{4} c_i^2$$
If true, the iteration loop is bypassed entirely, and the point is rendered as part of the inner set.

**Period-2 Bulb Bounds Checking:**
The largest circular geometry extending to the left of the main cardioid is the period-2 bulb, centered at $c = -1$ with a radius of 0.25. The inclusion check is calculated as:
$$(c_r + 1)^2 + c_i^2 \leq \frac{1}{16}$$

**Periodicity Checking:**
For points that fall outside the main cardioid and period-2 bulb, the iterative sequence of $z$ values may eventually fall into a repeating, closed loop. Tracking the entire history of $z$ values would exhaust memory bandwidth. Instead, the algorithm records an `old` $z$ reference strictly at power-of-two intervals utilizing a bitwise AND operator: `(iter & 31) == 0`. If the current $z$ coordinates match the `old` $z$ coordinates, a periodic cycle is mathematically confirmed, guaranteeing the point will never escape. The loop is immediately aborted.

### Fractal Variants

**Julia Set:**
Unlike the Mandelbrot set where $z$ starts at 0 and $c$ maps to the pixel coordinate, the Julia set assigns the pixel coordinate to the starting $z$ value, while $c$ remains a constant parametric constraint defined by the user via command-line arguments.

**Burning Ship:**
The Burning Ship modifies the core algebraic iteration by taking the absolute value of both the real and imaginary components before squaring:
$$z_{n+1} = (|\Re(z_n)| + i|\Im(z_n)|)^2 + c$$
This is executed by applying the `fabs()` function specifically to the $2 z_r z_i$ calculation for the imaginary component. The real component remains algebraically identical to the Mandelbrot equation because squaring an absolute value negates the need for sign conversion.

### Rendering Engine and MLX42 Integration

The program relies on an off-screen frame buffer approach to eliminate screen tearing and minimize CPU-to-GPU memory transfer latency. Mathematical results are written to an `mlx_image_t` buffer mapped to the window dimensions and pushed sequentially as completed frames.

**Bypassing Bounds Checking (`opt_pixel_put`):**
The native `mlx_put_pixel` function executes internal conditional checks to ensure target $x$ and $y$ coordinates fall within the image boundary. Because the primary rendering loops (`while (y < f->height)` and `while (x < f->width)`) strictly enforce coordinate bounds by design, these native checks are redundant. A custom `opt_pixel_put` function was built to write directly to memory, skipping the conditional overhead.

**1D Array Mapping:**
The MLX42 image buffer (`img->pixels`) exists as a continuous 1-dimensional array of 8-bit unsigned integers (`uint8_t`). Because memory is strictly linear, the 2D Cartesian coordinates must be adapted to locate the correct index. Furthermore, each visual pixel requires exactly 4 consecutive bytes to store its RGBA channel data. The specific memory offset for any given coordinate is calculated as:
$$offset = (y \times width + x) \times 4$$
* **Flattening:** `y * width + x` computes how many total pixels precede the target coordinate by multiplying the current row by the total row width, and adding the horizontal offset.
* **Byte Alignment:** `* 4` shifts the index to account for the 32-bit (4-byte) structure of a single pixel.

**Bitwise Unpacking and Color Construction:**
Colors are dynamically generated using iteration-based prime-number multipliers wrapped by a modulo operator (`% 256`). These independent 8-bit channels are packed into a single 32-bit `uint32_t` integer.

```text
=========================================================================
BITWISE COLOR SHIFT:
uint32_t has 32 binary slots, grouped into 4 bytes:
[  Byte 4  ] [  Byte 3  ] [  Byte 2  ] [  Byte 1  ]
[ 00000000 ] [ 00000000 ] [ 00000000 ] [ 00000000 ]  (Empty 32-bit int)

Calculated channels: r = 0xAA, g = 0xBB, b = 0xCC, Alpha = 0xFF

STEP 1: (<<)
Shifting moves the 8 bits of color data left by N spaces to isolate the bytes.
r << 24 : [ AAAAAAAA ] [ 00000000 ] [ 00000000 ] [ 00000000 ]
g << 16 : [ 00000000 ] [ BBBBBBBB ] [ 00000000 ] [ 00000000 ]
b << 8  : [ 00000000 ] [ 00000000 ] [ CCCCCCCC ] [ 00000000 ]
0xFF    : [ 00000000 ] [ 00000000 ] [ 00000000 ] [ 11111111 ]

STEP 2: (|)
The OR operator snaps isolated bytes together without memory corruption:
  [ AAAAAAAA ] [ 00000000 ] [ 00000000 ] [ 00000000 ]  (r << 24)
| [ 00000000 ] [ BBBBBBBB ] [ 00000000 ] [ 00000000 ]  (g << 16)
| [ 00000000 ] [ 00000000 ] [ CCCCCCCC ] [ 00000000 ]  (b << 8)
| [ 00000000 ] [ 00000000 ] [ 00000000 ] [ 11111111 ]  (0xFF)
-----------------------------------------------------
= [ AAAAAAAA ] [ BBBBBBBB ] [ CCCCCCCC ] [ 11111111 ]  (0xAABBCCFF)
=========================================================================
```
Once mapped to the correct 1D array offset, the custom pixel putter shifts the bits back to the right (`>> 24`, `>> 16`, `>> 8`) and masks them using the bitwise AND operator (`& 0xFF`) to safely write the isolated byte directly into the `img->pixels` buffer.

### Event Hook Logic

* **Adaptive Mouse Zoom (`scroll_hook`):** Static center-screen zoom inherently causes the user's targeted visual zone to drift. Adaptive zoom locks the origin strictly to the mouse pointer. The exact complex coordinates of the mouse (`mouse_r` and `mouse_i`) are calculated prior to applying the zoom multiplier. After the scale is adjusted, the complex plane offset (`shift_x` and `shift_y`) is translated by the difference between the pre-zoom and post-zoom complex coordinates at the mouse's physical screen location.
* **Dynamic Viewport Resizing (`size_hook`):** Altering the application window distorts the mathematical projection if the scale remains static. The sizing hook intercepts boundary changes, mutates the underlying `mlx_image_t` dimensions, and recalculates the scaling axes (`scale_x` and `scale_y`). By dividing the applied zoom against the minimum dimensional side (`min_side / 4.0`), the aspect ratio of the fractal equation is continuously preserved regardless of window geometry stretching.
* **State-Driven Color Cycling (`key_hook`):** Pressing the SPACE key increments a tracked state variable `f->scheme` modulo 3. This routes the rendering pipeline through an alternative sequence of prime multipliers for the RGB shifting algorithm. This modifies the visual depth without requiring the iterative mathematical sets to be recomputed.

## Instructions

### Compilation
To compile the mandatory section of the project, navigate to the root directory and run:
```bash
make
```
To compile with the bonus features (Burning Ship, adaptive zoom, dynamic colors), run:
```bash
make bonus
```
Additionally, valgrind rule can be used;
```

```

### Execution
The program requires specific command-line parameters to determine which fractal to display. 

**Mandelbrot:**
```bash
./fractol mandelbrot
```
**Burning Ship (Bonus):**
```bash
./fractol burning_ship
```
**Julia:**
The Julia set requires two additional parameters defining the real and imaginary components of the constant $c$:
```bash
./fractol julia <real> <imaginary>
```
*Julia Examples:*
* Basilica: `./fractol julia -1.0 0.0`
* Dendrite: `./fractol julia 0.0 1.0`
* Snowflake: `./fractol julia -0.8 0.156`

### Controls
* **Scroll Wheel:** Zoom in and out adaptively based on the cursor's location.
* **Spacebar:** Cycle through color schemes.
* **ESC / Window Cross:** Cleanly exit the program.

### Memory Testing (Valgrind)
[cite_start]The Makefile includes a `valgrind` rule to test the executable for memory leaks and memory errors using full tracking parameters[cite: 3].

[cite_start]By default, executing `make valgrind` will run the `mandelbrot` parameter[cite: 3]. [cite_start]You can test specific fractals or pass specific coordinates by overriding the `ARGS` variable directly in the command line[cite: 3]:

```bash
make valgrind ARGS="julia -0.8 0.156"
```

**Testing Bonus Features:**
[cite_start]Both the mandatory and bonus rules compile into the exact same executable name (`fractol`)[cite: 1, 3]. [cite_start]If you want to run Valgrind on the bonus features (such as `burning_ship`), you must strictly compile the bonus objects first by running `make bonus` before calling the memory test[cite: 1, 3]:

```bash
make bonus
make valgrind ARGS="burning_ship"
```

## Resources

### Literature and Papers
* **A First Course in Chaotic Dynamical Systems: Theory and Experiment (Second Edition)** — Chapters 15, 16, and 17 cover the foundational mathematics and orbital mechanics of chaotic systems.
* **An Introduction to Complex Numbers** by Jan van de Craats — Used for understanding the core algebraic principles required to map and calculate complex arithmetic on a 2D plane.
* **Fractal Geometry: The Mandelbrot and Julia Sets** by Stephanie Avalos-Bock (July 2009) — Provided geometric definitions and mathematical constraints for iterative fractal sets.

### Technical References and Tutorials
* **[Lode's Computer Graphics Tutorial: Julia and Mandelbrot Sets](https://lodev.org/cgtutor/juliamandelbrot.html)** — Reference for implementing the core iterative loops and understanding the correlation between mathematical coordinates and screen pixels.
* **[mathr.co.uk: Periodicity Scan](https://mathr.co.uk/blog/2017-05-17_periodicity_scan.html)** — Explains the logic and mathematical proofs behind the power-of-two periodicity checking optimization used to abort trapped iterations early.
* **[Dynamic Math: Mandelbrot & Julia Sets](https://www.dynamicmath.xyz/mandelbrot-julia/)** — Interactive visualization tool used to cross-reference coordinates and behavioral characteristics of the sets.
* **Graphics Library:** [MLX42 Documentation](https://github.com/codam-coding-college/MLX42)
* **AI Usage:** AI tools were utilized strictly as a technical writing assistant to help structure and draft this `README.md` file. I wrote the code and the underlying logic independently; the AI was provided with my rough notes and prompted to help document the mathematical optimizations, memory mapping logic, and geometric checks to ensure clear, precise technical communication.
