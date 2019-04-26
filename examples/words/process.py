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


npath = "nouns/"
for noun in (_f for _f in _listdir(npath) if _isfile(npath+_f)):
    resized = _resize(Image.open((npath + noun)).convert("RGBA"), (100, 100))
    resized.save(((npath + "processed/") + noun))

vpath = "verbs/"
vmark = Image.open("verb_mark.png").convert("RGBA")
for verb in (_f for _f in _listdir(vpath) if _isfile(vpath+_f)):
    resized = _resize(Image.open((vpath + verb)).convert("RGBA"), (100, 100))
    _add(resized, vmark).save(((vpath + "processed/") + verb))

apath = "adjectives/"
amark = Image.open("adjective_mark.png").convert("RGBA")
for adj in (_f for _f in _listdir(apath) if _isfile(apath+_f)):
    resized = _resize(Image.open((apath + adj)).convert("RGBA"), (100, 100))
    _add(resized, amark).save(((apath + "processed/") + adj))

