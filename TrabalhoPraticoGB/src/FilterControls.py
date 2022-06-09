import cv2
import numpy as np
from Filter import *

# funções dos efeitos de cada filtro

def blurFunction(img):
  return cv2.GaussianBlur(img, (13, 13), 5, 0)

def grayFunction(img):
  return cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

def cannyFunction(img):
  return cv2.Canny(img, 25, 75)

def equalizeFunction(img):
  if not filters['gray'].isActive:
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

  return cv2.equalizeHist(img)

def dilateFunction(img):
  kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))
  return cv2.dilate(img, kernel)

def erodeFunction(img):
  kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))
  return cv2.erode(img, kernel)

def brightnessFunction(img):
  return cv2.convertScaleAbs(img, beta=-60)

def sharpFunction(img):
  kernel = np.array([[-1, -1, -1], [-1, 9.5, -1], [-1, -1, -1]])
  return cv2.filter2D(img, -1, kernel)

def sepiaFunction(img):
  img = np.array(img, dtype=np.float64) # converting to float to prevent loss
  img = cv2.transform(img, np.matrix([[0.272, 0.534, 0.131],
                                  [0.349, 0.686, 0.168],
                                  [0.393, 0.769, 0.189]])) # multipying image with special sepia matrix
  img[np.where(img > 255)] = 255 # normalizing values greater than 255 to 255
  return np.array(img, dtype=np.uint8)

def pencilSketchFunction(img):
  sk_gray, sk_color = cv2.pencilSketch(img, sigma_s=60, sigma_r=0.07, shade_factor=0.1)
  return sk_color

def hdrFunction(img):
  return cv2.detailEnhance(img, sigma_s=12, sigma_r=0.15)

def invertFunction(img):
  return cv2.bitwise_not(img)

from scipy.interpolate import UnivariateSpline
def LookupTable(x, y):
  spline = UnivariateSpline(x, y)
  return spline(range(256))

def summerFunction(img):
  increaseLookupTable = LookupTable([0, 64, 128, 256], [0, 80, 160, 256])
  decreaseLookupTable = LookupTable([0, 64, 128, 256], [0, 50, 100, 256])
  blue_channel, green_channel,red_channel  = cv2.split(img)
  red_channel = cv2.LUT(red_channel, increaseLookupTable).astype(np.uint8)
  blue_channel = cv2.LUT(blue_channel, decreaseLookupTable).astype(np.uint8)
  return cv2.merge((blue_channel, green_channel, red_channel ))

def winterFunction(img):
  increaseLookupTable = LookupTable([0, 64, 128, 256], [0, 80, 160, 256])
  decreaseLookupTable = LookupTable([0, 64, 128, 256], [0, 50, 100, 256])
  blue_channel, green_channel,red_channel = cv2.split(img)
  red_channel = cv2.LUT(red_channel, decreaseLookupTable).astype(np.uint8)
  blue_channel = cv2.LUT(blue_channel, increaseLookupTable).astype(np.uint8)
  return cv2.merge((blue_channel, green_channel, red_channel))

# todos os filtros
filters = {
  'blur': Filter(blurFunction),
  'gray': Filter(grayFunction),
  'canny': Filter(cannyFunction),
  'equalize': Filter(equalizeFunction),
  'dilate': Filter(dilateFunction),
  'erode': Filter(erodeFunction),
  'brightness': Filter(brightnessFunction),
  'sharp': Filter(sharpFunction),
  'sepia': Filter(sepiaFunction),
  'pencilSketch': Filter(pencilSketchFunction),
  'hdr': Filter(hdrFunction),
  'invert': Filter(invertFunction),
  'summer': Filter(summerFunction),
  'winter': Filter(winterFunction),
}

# função para ativar/desativar os filtros nos botões
def handleFilter(*args):
  filters[args[1]].isActive = not filters[args[1]].isActive