#!/usr/bin/env python3
from os import walk as _walk
from os import listdir as _listdir
from os.path import isfile as _isfile
from itertools import chain as _chain
from pathlib import Path as _Path
from PIL import Image, ImageEnhance, ImageChops, ImageFile
ImageFile.LOAD_TRUNCATED_IMAGES = True

def _resize(img, dims):
    return img.resize(tuple(int(c) for c in dims))

def _crop(img, sec):
    return img.crop(tuple(int(b) for b in sec))

def _add(img1, img2):
    result = img1.copy()
    result.paste(img2, mask=img2)
    return result

def _sub(img1, img2):
   return ImageChops.difference(img1, img2)

def _mult(img1, img2):
    return ImageChops.blend(img1, img2, 0.5)

def _div(img1, img2):
    return _mult(img1, _inv(img2))
    
def _inv(img):
    *rgb, a = img.split()
    img_rgb = Image.merge('RGB', rgb)
    img_inv = ImageChops.invert(img_rgb)
    img_inv.putalpha(a)

    return img_inv

def _rm_sec(img, sec, reverse=False):
    left, upper, right, lower = sec
    rect_sz = (int(right-left), int(lower-upper))
    pos = (int(left), int(upper))

    rect = Image.new("RGBA", rect_sz, 4*(0,))
    removed = img.copy()
    removed.paste(rect, pos)

    if not reverse:
        return removed
    else:
        return _sub(img, removed)

def _scale(section, dims):
    return tuple(b*d for b, d in zip(section, 2*dims))

def _save(img, path):
    if path.suffix in ('.jpg', '.jpeg'):
        img.convert("RGB").save(path)
    else:
        img.save(path)

def _path_sum(p1, p2):
    name = p1.stem + p2.stem + p1.suffix
    return p1.with_name(name)

def _path_div(p1, p2):
    if p1.suffix:
        path = p1.parent / p2
        if p2.suffix:
            return path
        else:
            return path / p1.name
    else:
        return p1 / p2


if (False or False):
    print(_Path("yay"))
else:
    if False:
        print(_Path("nay"))
    else:
        print(_Path("never"))


