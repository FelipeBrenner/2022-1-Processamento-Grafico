import cv2
from Sticker import *
from StickerControls import *
from Filter import *
from FilterControls import *

cap = cv2.VideoCapture(1) # inicia a captura do video

cv2.namedWindow('Instagram') # cria a janela

# cria os botões dos filtros
cv2.createButton('Blur', handleFilter, 'blur', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Gray', handleFilter, 'gray', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Canny', handleFilter, 'canny', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Equalize', handleFilter, 'equalize', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Dilate', handleFilter, 'dilate', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Erode', handleFilter, 'erode', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Brightness', handleFilter, 'brightness', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Sharp', handleFilter, 'sharp', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Sepia', handleFilter, 'sepia', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Pencil Sketch', handleFilter, 'pencilSketch', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('HDR', handleFilter, 'hdr', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Invert', handleFilter, 'invert', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Summer', handleFilter, 'summer', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Winter', handleFilter, 'winter', cv2.QT_PUSH_BUTTON, 1)
cv2.createButton('Witch', handleFilter, 'witch', cv2.QT_PUSH_BUTTON, 1)

# cria a seleção dos stickers
cv2.createButton('Nenhum', handleStickerIndex, 0, cv2.QT_RADIOBOX, 1)
cv2.createButton('Nada ver irmão', handleStickerIndex, 1, cv2.QT_RADIOBOX, 0)
cv2.createButton('Mó paz', handleStickerIndex, 2, cv2.QT_RADIOBOX, 0)
cv2.createButton('Loading cat', handleStickerIndex, 3, cv2.QT_RADIOBOX, 0)
cv2.createButton('??', handleStickerIndex, 4, cv2.QT_RADIOBOX, 0)
cv2.createButton('Azideia', handleStickerIndex, 5, cv2.QT_RADIOBOX, 0)
cv2.createButton('Coreana', handleStickerIndex, 6, cv2.QT_RADIOBOX, 0)
cv2.createButton('Pigmeu', handleStickerIndex, 7, cv2.QT_RADIOBOX, 0)
cv2.createButton('Que loucura', handleStickerIndex, 8, cv2.QT_RADIOBOX, 0)

# seta a função de leitura do click do mouse para posicionar o sticker selecionado
cv2.setMouseCallback('Instagram', mouseCallback)

while True: # loop para captura e tratamento dos frames/imagems do video
  # captura o frame/imagem do video
  ret, img = cap.read()

  # cola os stickers já posicionados em todo frame/imagem capturado do video
  for sticker in stickers:
    img = stickerTransparent(img, sticker.image, int((sticker.x - sticker.image.shape[0]/2)), int((sticker.y - sticker.image.shape[1]/2)))
    
  # aplica os filtros ativados em todo frame/imagem capturado do video
  for filter in filters.items():
    if filter[1].isActive:
      img = filter[1].function(img)

  cv2.imshow('Instagram', img) # exibe o frame/imagem

  k = cv2.waitKey(20) & 0xFF # armazena o que foi digitado no teclado

  if k == 27: # sai se pressionado 'esc'
    break

cap.release() # desliga o video
cv2.destroyAllWindows() # fecha todas as janelas