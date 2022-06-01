import cv2
import numpy as np 

#read video
cap = cv2.VideoCapture(1)

#create a window
cv2.namedWindow('Instagram')

#filter buttons
isActiveBlur = False
isActiveGray = False
isActiveCanny = False
isActiveEqualize = False
isActiveDilate = False
isActiveErode = False

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

while True: #continue to run until user breaks loop
  ret, img = cap.read()

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

  #display image
  cv2.imshow('Instagram', img)

  #armazena o que foi digitado no teclado
  k = cv2.waitKey(20) & 0xFF

  #if user pressed 'esc' break
  if k == 27:
    break

cap.release() #turn off camera 
cv2.destroyAllWindows() #close all windows