import cv2
import numpy as np
from Sticker import *

stickers = [] # stickers posicionados em tela
stickerIndex = 0 # índice do sticker selecionado nas radio box

# imagem dos stickers lida
stickersImages = [
  None,
  cv2.imread("../stickers/nadaverirmao.png", cv2.IMREAD_UNCHANGED),
  cv2.imread("../stickers/mopaz.png", cv2.IMREAD_UNCHANGED),
  cv2.imread("../stickers/loadingcat.png", cv2.IMREAD_UNCHANGED),
  cv2.imread("../stickers/??.png", cv2.IMREAD_UNCHANGED),
  cv2.imread("../stickers/azideia.png", cv2.IMREAD_UNCHANGED),
  cv2.imread("../stickers/coreana.png", cv2.IMREAD_UNCHANGED),
  cv2.imread("../stickers/pigmeu.png", cv2.IMREAD_UNCHANGED),
  cv2.imread("../stickers/queloucura.png", cv2.IMREAD_UNCHANGED),
]

# escala de cada respectiva imagem dos sticker, pois cada uma foi salva com um tamanho diferente
stickersScalePercent = [
  0,
  25,
  15,
  15,
  20,
  20,
  20,
  20,
  20,
]

# função que cola o sticker transparente no frame/imagem capturado
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

# função para selecionar o sticker na radio box
def handleStickerIndex(*args):
  global stickerIndex
  stickerIndex = args[1]

# função de leitura do click do mouse para posicionar o sticker selecionado
def mouseCallback(event, x, y, flags, param):
  global stickerIndex, stickers
  if event == cv2.EVENT_LBUTTONDOWN and stickerIndex != 0:
    sticker = Sticker(x, y, cv2.cvtColor(stickersImages[stickerIndex], cv2.COLOR_BGR2BGRA), stickersScalePercent[stickerIndex])
    stickers.append(sticker)