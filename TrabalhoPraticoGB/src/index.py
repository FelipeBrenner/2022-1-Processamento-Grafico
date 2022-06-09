from re import I
import cv2
import numpy as np 
from Sticker import *

isActiveBlur = False
isActiveGray = False
isActiveCanny = False
isActiveEqualize = False
isActiveDilate = False
isActiveErode = False

stickerIndex = 0
stickersImages = [
  None,
  cv2.imread("../stickers/nadaverirmao.png", cv2.IMREAD_UNCHANGED),
  cv2.imread("../stickers/crying.png", cv2.IMREAD_UNCHANGED),
  cv2.imread("../stickers/mopaz.png", cv2.IMREAD_UNCHANGED),
  cv2.imread("../stickers/loadingcat.png", cv2.IMREAD_UNCHANGED),
  cv2.imread("../stickers/??.png", cv2.IMREAD_UNCHANGED),
]
stickersScalePercent = [
  0,
  25,
  10,
  15,
  15,
  20,
]
stickers = []

cap = cv2.VideoCapture(1) # read video

cv2.namedWindow('Instagram') # create a window

def handleBlur(*args):
  global isActiveBlur
  isActiveBlur = not isActiveBlur

def handleGray(*args):
  global isActiveGray
  isActiveGray = not isActiveGray

def handleCanny(*args):
  global isActiveCanny
  isActiveCanny = not isActiveCanny

def handleEqualize(*args):
  global isActiveEqualize
  isActiveEqualize = not isActiveEqualize

def handleDilate(*args):
  global isActiveDilate
  isActiveDilate = not isActiveDilate

def handleErode(*args):
  global isActiveErode
  isActiveErode = not isActiveErode

cv2.createButton('Blur', handleBlur, None, cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Gray', handleGray, None, cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Canny', handleCanny, None, cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Equalize', handleEqualize, None, cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Dilate', handleDilate, None, cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Erode', handleErode, None, cv2.QT_PUSH_BUTTON, 1)

def handleStickerIndex(*args):
  global stickerIndex
  stickerIndex = args[1]

cv2.createButton('Nenhum Sticker', handleStickerIndex, 0, cv2.QT_RADIOBOX, 1)
cv2.createButton('Nada ver irmão', handleStickerIndex, 1, cv2.QT_RADIOBOX, 0)
cv2.createButton('Crying', handleStickerIndex, 2, cv2.QT_RADIOBOX, 0)
cv2.createButton('Mó paz', handleStickerIndex, 3, cv2.QT_RADIOBOX, 0)
cv2.createButton('Loading cat', handleStickerIndex, 4, cv2.QT_RADIOBOX, 0)
cv2.createButton('??', handleStickerIndex, 5, cv2.QT_RADIOBOX, 0)

def stickerTransparent(background, sticker, x_offset=None, y_offset=None):
  bg_h, bg_w, bg_channels = background.shape
  fg_h, fg_w, fg_channels = sticker.shape

  assert bg_channels == 3, f'background image should have exactly 3 channels (RGB). found:{bg_channels}'
  assert fg_channels == 4, f'sticker image should have exactly 4 channels (RGBA). found:{fg_channels}'

  # center by default
  if x_offset is None: x_offset = (bg_w - fg_w) // 2
  if y_offset is None: y_offset = (bg_h - fg_h) // 2

  w = min(fg_w, bg_w, fg_w + x_offset, bg_w - x_offset)
  h = min(fg_h, bg_h, fg_h + y_offset, bg_h - y_offset)

  if w < 1 or h < 1: return

  # clip sticker and background images to the overlapping regions
  bg_x = max(0, x_offset)
  bg_y = max(0, y_offset)
  fg_x = max(0, x_offset * -1)
  fg_y = max(0, y_offset * -1)
  sticker = sticker[fg_y:fg_y + h, fg_x:fg_x + w]
  background_subsection = background[bg_y:bg_y + h, bg_x:bg_x + w]

  # separate alpha and color channels from the sticker image
  sticker_colors = sticker[:, :, :3]
  alpha_channel = sticker[:, :, 3] / 255  # 0-255 => 0.0-1.0

  # construct an alpha_mask that matches the image shape
  alpha_mask = np.dstack((alpha_channel, alpha_channel, alpha_channel))

  # combine the background with the sticker image weighted by alpha
  composite = background_subsection * (1 - alpha_mask) + sticker_colors * alpha_mask

  # overwrite the section of the background image that has been updated
  background[bg_y:bg_y + h, bg_x:bg_x + w] = composite

  return background

def mouseCallback(event, x, y, flags, param):
  global stickerIndex, stickers
  if event == cv2.EVENT_LBUTTONDOWN and stickerIndex != 0:
    sticker = Sticker(x, y, cv2.cvtColor(stickersImages[stickerIndex], cv2.COLOR_BGR2BGRA), stickersScalePercent[stickerIndex])
    stickers.append(sticker)

cv2.setMouseCallback('Instagram', mouseCallback)

while True:
  ret, img = cap.read()

  if len(stickers) > 0:
    for sticker in stickers:
      img = stickerTransparent(img, sticker.image, int((sticker.x - sticker.image.shape[0]/2)), int((sticker.y - sticker.image.shape[1]/2)))
      
  if isActiveBlur:
    img = cv2.GaussianBlur(img, (13, 13), 5, 0)
  if isActiveGray:
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
  if isActiveCanny:
    img = cv2.Canny(img, 25, 75)
  if isActiveEqualize and not isActiveCanny:
    if not isActiveGray:
      img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img = cv2.equalizeHist(img)
  if isActiveDilate:
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5));
    img = cv2.dilate(img, kernel)
  if isActiveErode:
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5));
    img = cv2.erode(img, kernel)

  cv2.imshow('Instagram', img) # display image

  k = cv2.waitKey(20) & 0xFF # armazena o que foi digitado no teclado

  if k == 27: # if user pressed 'esc' break
    break

cap.release() # turn off camera 
cv2.destroyAllWindows() # close all windows