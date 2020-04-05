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

## Examples
