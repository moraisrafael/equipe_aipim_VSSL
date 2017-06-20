import sys

import numpy as np
import cv2


# pixel values set as reference for thresholding
color = {
    'yellow': [76, 208, 201],
    'blue': [198, 129, 56],
    'red': [74, 62, 192]
}


def get_boundaries_from_range(color, upper, lower):
    # calculate upper and lower boundaries in tuple
    lower = np.array(color, dtype='int16') - lower
    upper = np.array(color, dtype='int16') + upper
    
    # negative values and higher than 255 are not valid
    lower = np.clip(lower, 0, 255)
    upper = np.clip(upper, 0, 255)

    return (lower, upper)


def img_color_threshold(img, boundaries):
    '''Performs color thresholding according to the color and the upper and
    lower ranges (pixel values) given.'''
    
    lower = boundaries[0]
    upper = boundaries[1]

    # find the colors within the specified boundaries and apply the mask
    mask = cv2.inRange(img, lower, upper)
    output = cv2.bitwise_and(img, img, mask = mask)

    cv2.imshow("images", np.hstack([img, output]))
    if cv2.waitKey(1) & 0xFF == ord('q'):
        sys.exit(1)

    return output


def show_images(*imgs):
    for img in imgs:
        cv2.imshow('', img)
        cv2.waitKey(0)
    cv2.destroyAllWindows()
        

def main():
    #img = cv2.imread(sys.argv[1])
    boundaries_blue = get_boundaries_from_range(color['blue'], 80, 80)
    boundaries_red = get_boundaries_from_range(color['red'], 50, 50)
    boundaries_yellow = get_boundaries_from_range(color['yellow'], 50, 50)

    #threshold = img_color_threshold(img, boundaries_yellow)
    #threshold = img_color_threshold(img, boundaries_blue)
    #threshold = img_color_threshold(img, boundaries_red)
    

    camera = cv2.VideoCapture(0)

    while True:
        ret, frame = camera.read()

        threshold = img_color_threshold(frame, boundaries_blue)

        #cv2.imshow('frame', frame)
        #if cv2.waitKey(1) & 0xFF == ord('q'):
            #break


if __name__ == '__main__':
    main()
