import cv2
from Filter import *

# funções dos efeitos de cada filtro

def blurFunction(img):
  return cv2.GaussianBlur(img, (13, 13), 5, 0)

def grayFunction(img):
  return cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

def cannyFunction(img):
  return cv2.Canny(img, 25, 75)

def equalizeFunction(img):
  if filters['canny'].isActive:
    return img

  if not filters['gray'].isActive:
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

  return cv2.equalizeHist(img)

def dilateFunction(img):
  kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5));
  return cv2.dilate(img, kernel)

def erodeFunction(img):
  kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5));
  return cv2.erode(img, kernel)

# todos os filtros
filters = {
  'blur': Filter(blurFunction),
  'gray': Filter(grayFunction),
  'canny': Filter(cannyFunction),
  'equalize': Filter(equalizeFunction),
  'dilate': Filter(dilateFunction),
  'erode': Filter(erodeFunction),
}

# função para ativar/desativar os filtros nos botões
def handleFilter(*args):
  filters[args[1]].isActive = not filters[args[1]].isActive