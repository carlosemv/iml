from os import walk as _walk
from os import listdir as _listdir
from os.path import isfile as _isfile
from itertools import chain as _chain
from PIL import Image, ImageEnhance, ImageChops, ImageFile
ImageFile.LOAD_TRUNCATED_IMAGES = True

def _resize(img, dims):
    return img.resize(tuple(int(c) for c in dims))

def _crop(img, sec):
    return img.crop(tuple(int(b) for b in sec))

def _add(img1, img2):
    result = img1.copy()
    result.paste(img2)
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
    rect_sz = (right-left, lower-upper)
    pos = (left, upper)

    rect = Image.new("RGBA", rect_sz, (255, 255, 255, 0))
    removed = img.copy()
    removed.paste(rect, pos)

    if not reverse:
        return removed
    else:
        return _inv(_sub(img, removed))


p = "me.png"
img = Image.open(p).convert("RGBA")
img = img.rotate(-180)
subimg = _crop(img, (2, 2, 10, 10))
img = _resize(img, (100,100))
img = _resize(img, tuple(0.5 * dim for dim in img.size))
img = img.transpose(Image.FLIP_TOP_BOTTOM)
img = ImageEnhance.Contrast(img).enhance(5)
print(0.5)
