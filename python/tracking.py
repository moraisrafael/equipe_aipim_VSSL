
import sys
from collections import deque

import numpy as np
import cv2
import imutils

BUFFER_SIZE = 64

# bgr pixel values set as reference for thresholding
color = {
    'yellow': [76, 208, 201],
    'blue': [198, 129, 56],
    'red': [74, 62, 192]}

def get_HSV_boundaries_from_RGB(color_bgr, value_range):
    """Function to provide some default values for testing. Beware that
    HSV values for white and black colors goes through all the hue range
    without any change to saturation and value.
    """
    color_bgr = np.uint8([[color_bgr]])
    hsv_color = cv2.cvtColor(color_bgr,cv2.COLOR_BGR2HSV)[0][0][0]
    
    lower = np.array([hsv_color-value_range, 125, 15], dtype='int16')
    upper = np.array([hsv_color+value_range, 255, 230], dtype='int16')

    lower[0] = np.clip(lower[0], 0, 179)
    upper[0] = np.clip(upper[0], 0, 179)

    return (lower, upper)

def get_boundaries_from_BRG_range(color_bgr, upper, lower):
    # calculate upper and lower boundaries in tuple
    lower = np.array(color_bgr, dtype='int16') - lower
    upper = np.array(color_bgr, dtype='int16') + upper
    
    # negative values and higher than 255 are not valid
    lower = np.clip(lower, 0, 255)
    upper = np.clip(upper, 0, 255)

    return (lower, upper)

def main(videoSource):
    pts = deque(maxlen=BUFFER_SIZE)
    camera = cv2.VideoCapture(videoSource)

    while True:
        (grabbed, frame) = camera.read()

        if not grabbed:
            print('Ended video')
            break
        
        # resize frame, and convert it to HSV
        frame = imutils.resize(frame, width=600)
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        lower, upper = get_HSV_boundaries_from_RGB(color[sys.argv[2]], 40)

        mask = cv2.inRange(hsv, lower, upper)
        mask = cv2.erode(mask, None, iterations=2)
        mask = cv2.dilate(mask, None, iterations=2)

        # find contours in the mask
        cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
                cv2.CHAIN_APPROX_SIMPLE)[-2]
        # initializes set of xy axes for the tracked items
        center_list = []

        # check if there is a contour
        if len(cnts) > 0:
            center_list = draw_contours(cnts, frame, pts)

        cv2.imshow("Frame", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    camera.release()
    cv2.destroyAllWindows()


def draw_contours(cnts, frame, pts):

    center_list = []
    
    # gets the biggest 5 biggest contours by area to track
    biggest_cnts = sorted(cnts, key=cv2.contourArea)[:4]

    # get the biggest contour in the mask
    #c = max(cnts, key=cv2.contourArea)

    for c in biggest_cnts:
        ((x, y), radius) = cv2.minEnclosingCircle(c)

        # tries to find the centroid using moments
        M = cv2.moments(c)
        new_center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
        center_list.append(new_center)

        if radius > 10:
            # draw circle and centroid on the frame
            cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255,255), 2)
            cv2.circle(frame, new_center, 5, (0,0,255), -1)
        
        pts.appendleft(new_center)

    return center_list


main(sys.argv[1])



