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
        return _inv(_sub(img, removed))

def _scale(section, dims):
    return tuple(b*d for b, d in zip(section, 2*dims))


img = Image.open("boat.png").convert("RGBA")
circle = Image.open("circle.png").convert("RGBA")
rect = _scale((0.33, 0.33, 0.66, 0.66), (1080, 610))
cut = _rm_sec(img, rect)
a = ImageEnhance.Sharpness(cut.rotate(90)).enhance(10)
a.show()
