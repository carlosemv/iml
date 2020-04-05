# IML
IML is an Image Manipulation Language: a DSL for working with and editing images.

[A recent version](https://github.com/carlosemv/iml/releases/tag/sblp2019) is discussed and
partially documented in [this short paper](https://dl.acm.org/doi/10.1145/3355378.3355382), which
you can use if you wish to cite this work:
```
@inproceedings{vieira2019iml,
  title={IML - An Image Manipulation Language},
  author={Vieira, Carlos and de Medeiros, Sergio Queiroz},
  booktitle={Proceedings of the XXIII Brazilian Symposium on Programming Languages},
  pages={27--29},
  year={2019}
}
```

This implementation currently compiles to Python, with a dependency to the [Pillow/PIL](https://github.com/python-pillow/Pillow)
library.

## Building
IML is written in C++ and built using CMake (>= 3.13):
```bash
mkdir build && cd build
cmake ..
make
sudo make install
```

The last command is of course optional if you don't want to install IML system-wide:
use can just use the `iml` executable inside the build directory.
Alternatively, if you want to install it locally (instead of the default, to /usr/local), after `cd build`:
```bash
cmake -DCMAKE_INSTALL_PREFIX=$HOME ..
make
make install
```
Then add `$HOME/bin` to your `PATH`: `export PATH=$HOME/bin:$PATH`.

## Running
To compile an IML program, run `iml <IML program file> <output file>`, e.g. `iml ex.iml ex.py`.
Then `python <output file>` (`python ex.py`). As previously mentioned, you'll need a working python
interpreter with the [Pillow/PIL](https://github.com/python-pillow/Pillow) library installed.

## Documentation

### Types
IML has six basic data types:
* image;
* usual numeric type (integer or float);
* usual boolean type;
* path, an extension of the usual string type, used for representing a path to a directory or file;
* dimensions, an image's dimensions in pixels as a (width, height) tuple; and
* section, a rectangular section of an image, as a tuple with leftmost, uppermost, rightmost, and lowermost limits, in pixels.

Note that when using sections, an image's origin is taken to be its upper left corner.

### Statements
All statements in IML are one of:
* Assignment (e.g. `red = image in "red_column.png"`)
* Image operation (e.g. `flip red horizontally`)
* Save statement (e.g. `save purple as "purple.png"`)
* Print statement (e.g. `print purple`)
* For-each loop
* If-else statement

New variables are declared on assignment. Control structures (if-else statements and for-each loops) are described [here](#control).

### Preview

```python
1   red = image in "red_column.png"
2   blue = image in "blue_bar.png"

4   flip red horizontally
5   purple = flip red * blue vertically
6   purple = purple + blue

8   center = (0.4, 0.4, 0.6, 0.6)
9   center = center * (purple dimensions)

11  print purple - center
```

In [the example script above](/examples/docs/script1.iml) we can see in line 1 the assignment of an image to a variable called `red` (first image), loaded from a file named 'red\_column.png' (a path), and likewise for `blue` (second image) in the following line:

<img src="https://github.com/carlosemv/iml/raw/master/examples/docs/pregen/red_column.png" width="200"> <img src="https://github.com/carlosemv/iml/raw/master/examples/docs/pregen/blue_bar.png" width="200">

Note that grey checkered sections are transparent. Line 4 flips `red` horizontally (first image). In line 5 we *compose* (i.e. produce and even blend of) `red` and `blue` (second image); then flip it vertically, and assign it to a new variable called `purple` (third image):

<img src="https://github.com/carlosemv/iml/raw/master/examples/docs/pregen/red1.png" width="200"> <img src="https://github.com/carlosemv/iml/raw/master/examples/docs/pregen/purple1.png" width="200"> <img src="https://github.com/carlosemv/iml/raw/master/examples/docs/pregen/purple2.png" width="200">

In line 6 we *overlay* (i.e. paste right hand side image over the left one) `purple` with `blue` (first image). In line 8 we define a section with relative points, then scale them up to image (`purple`) dimensions on line 9.
In line 11 the section is *removed* from `purple` and the resulting image is displayed (second image):

<img src="https://github.com/carlosemv/iml/raw/master/examples/docs/pregen/purple3.png" width="200"> <img src="https://github.com/carlosemv/iml/raw/master/examples/docs/pregen/no_center.png" width="200">

### Image operations
In lines 4 and 5 we saw our first example of an **image operation**: these are statements that produce one image from another, and that can be used both as expressions, representing the newly produced image, as in line 5; and as commands, altering the original image, as in line 4.

There are five of these operations:
* *rotate*, rotates an image anticlockwise by the given number of degrees (clockwise if negative);
* *flip*, simply flips an image vertically or horizontally;
* *resize*, resizes an image to an absolute dimensions value, or relatively by a given multiplicative factor;
* *crop*, crops a section from an image, producing a new image from that image section;
* *modify*, modifies a color attribute of the image by a given factor.

Regarding the modify operation, there are four possible color attributes which can be modified: sharpness, contrast, brightness, and color.

### Unary and binary operators

Images can also be modified in IML through **unary and binary operators**.
The unary operators are:
* unary minus (`-`), valid only for numbers;
* boolean negation (`not`), valid only for boolean expressions;
* 'dimensions' operator, valid only for images; and
* channel access operators, also only for images, of the form `(R)`, `(G)`, or `(B)` (e.g. `print purple(R)`).

<img src="https://github.com/carlosemv/iml/raw/master/examples/docs/binary.png" height="300">

Above, we can see most binary operations supported by IML, excluding comparison operators (such as `==`, `<`, etc.), boolean operators (`and`, `or`), and operations between paths/strings: when both operands are paths, the `+` operator appends the filename on the second path to the first; and `/` extends the first path with the second (ignoring filename).

When both operands are images, we have the following operations: with the `+` operator, the right hand side image is simply pasted over the left hand side one; for `-`, the resulting image is given by the pixel-by-pixel difference between the two operands; for `*`, a composite image is produced from the two operands, being an even blend of both; for `/`, we have the same behaviour of `*`, but between the first operand, and the color inverse of the second.

When both operands are of numeric types, we have the usual operations. When both operands are of the same type, and are dimensions or sections, the operation is done numerically, element-wise. We have a similar behaviour (element-wise numerical operation) when one of the operands is a dimenions or section, and the other is numerical. When one of the operands is a dimension and the other is a section, the section is considered to be given as proportions, and `*` produces a section appropriately scaled to the given dimensions.

Finally, when one of the operands is an image and the other is a section, we have the `-` operator, which removes a section from an image, or removes the image except for the section given, depending on the order of operands.

### <a name="control"></a> Control structures

## Examples
