"""This script detects the presence of blobs and tracks it as it moves 
around in the video frames.

Reference code: http://www.pyimagesearch.com/2015/09/14/ball-tracking-with-opencv/
"""

import sys
from collections import deque

import numpy as np
import cv2
import imutils

BUFFER_SIZE = 64

# pixel values set as reference for thresholding
color = {
    'yellow': [76, 208, 201],
    'blue': [198, 129, 56],
    'red': [74, 62, 192],
}


# Hue range is [0..179], Saturation is [0..255] and Value is [0..255]
# here are some default values.
hsv_color = {
    'yellow': 60,
    'blue': 233,
    'red': 0,
}


def get_boundaries_from_HSV_range(color, range):
    """Function to provide some default values for testing. Beware that
    HSV values for white and black colors goes through all the hue range
    without any change to saturation and value.
    """

    lower = np.array([color, 33, 33], dtype='int16')
    upper = np.array[(color, 255, 255], dtype='int16')

    lower[0] = np.clip(lower[0], 0, 179)
    upper[0] = np.clip(upper[0], 0, 179)

    return (lower, upper)


def get_boundaries_from_BRG_range(color, upper, lower):
    # calculate upper and lower boundaries in tuple
    lower = np.array(color, dtype='int16') - lower
    upper = np.array(color, dtype='int16') + upper
    
    # negative values and higher than 255 are not valid
    lower = np.clip(lower, 0, 255)
    upper = np.clip(upper, 0, 255)

    return (lower, upper)


def main():
    pts = deque(maxlen=BUFFER_SIZE)
    camera = cv2.VideoCapture(sys.argv[1])

    while True:
        (grabbed, frame) = camera.read()

        if not grabbed:
            break
        
        # resize frame, and convert it to HSV
        frame = imutils.resize(frame, width=600)
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        lower, upper = get_boundaries_from_HSV_range(hsv_color, 20)

        mask = cv2.inRange(hsv, lower, upper)
        mask = cv2.erode(mask, None, iterations=2)
        mask = cv2.dilate(mask, None, iterations=2)

        cv2.imshow("Frame", mask)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    camera.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    if (len(sys.argv) != 2):
        print('Programa requer um video como argumento. Saindo.')
        sys.exit(1)
    main()
