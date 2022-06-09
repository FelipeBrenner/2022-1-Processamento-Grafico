import cv2

class Sticker:
  def __init__(self, x, y, image, scale):
    self.x = x
    self.y = y
    self.scale = scale
    
    # redimensiona a imagem do sticker conforme a sua determinada escala
    width = int(image.shape[1] * scale / 100)
    height = int(image.shape[0] * scale / 100)
    dim = (width, height)
    self.image = cv2.resize(image, dim, interpolation = cv2.INTER_AREA)