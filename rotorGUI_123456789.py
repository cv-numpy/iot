import cv2 as cv
import numpy as np

# servo number
servo_num = 12

# Number of servos per row
row_length = servo_num // 2

# Map keys to rotation direction
clockwise_keys = [ord(str(i)) for i in '1234567890-=']
counterclockwise_keys = [ord(ch) for ch in 'qwertyuiop[]']

# servo angle step
step = 2
width = 1280
height = 720

# Calculate the font scale and thickness based on the width and height of the image
font_scale = width // 400
thickness = width // 100

# Calculate the text size
textSize = cv.getTextSize('0', cv.FONT_HERSHEY_SIMPLEX, font_scale, 2)[0]

# Font scale for the keys
key_font_scale = int(font_scale * 2 / 3)  # Reduce the font scale for keys
key_thickness = int(thickness * 0.5)

firstRowY = height // 4
secondRowY = int( height * 7/9 )
RowX = [int((col+0.5) * width // (row_length)) for col in range(row_length)]

# Current servo angles
servo_angles = [90] * servo_num

def rotate_servo(index, direction):
    servo_angles[index] = min(180, max(0, servo_angles[index] + direction * step))

# Map keys to actions
key_actions = {key: (rotate_servo, i, 1) for i, key in enumerate(clockwise_keys)}
key_actions.update({key: (rotate_servo, i, -1) for i, key in enumerate(counterclockwise_keys)})

def display_angles(image, angles, row):
    textY = firstRowY if row == 0 else secondRowY
    for i in range(row_length):
        # get the digit size of the angle
        angleSize = len(str(angles[i]))

        angle_textSizeX = int(textSize[0] * angleSize*0.5)
        angle_textSizeY = int(textSize[1] * 0.35)

        # Display angle
        cv.putText(image, str(angles[i]), (RowX[i]-angle_textSizeX, textY+angle_textSizeY), cv.FONT_HERSHEY_SIMPLEX, font_scale, 255, thickness)

def display_keys(image, keys, row, offset):
    textY = firstRowY + offset if row == 0 else secondRowY + offset
    for i in range(row_length):
        # get size of the key
        textSize = cv.getTextSize(chr(keys[i]), cv.FONT_HERSHEY_SIMPLEX, key_font_scale, 2)[0]
        # Display key
        cv.putText(image, chr(keys[i]), (RowX[i]-int(textSize[0]*0.5), textY), cv.FONT_HERSHEY_SIMPLEX, key_font_scale, 255, key_thickness)

def generate_static_image():
    # Create a black image to capture key presses on
    image = np.zeros((height, width), dtype="uint8")

    # Display the ESC text
    cv.putText(image, 'ESC', (8, 36), cv.FONT_HERSHEY_SIMPLEX, font_scale//2, 255, thickness//4)

    # Display keys
    display_keys(image, clockwise_keys, 0, -int(textSize[1]*1.35))
    display_keys(image, counterclockwise_keys, 0, int(textSize[1]*1.65))
    display_keys(image, clockwise_keys[row_length:], 1, -int(textSize[1]*1.35))
    display_keys(image, counterclockwise_keys[row_length:], 1, int(textSize[1]*1.65))

    return image

# Generate the static image
static_image = generate_static_image()

while True:
    # Create a black image to capture key presses on
    image = static_image.copy()

    # Display the keys and angles in two sections
    display_angles(image, servo_angles[:servo_num//2], 0)
    display_angles(image, servo_angles[servo_num//2:], 1)

    cv.imshow("Image", image)
    key = cv.waitKey(100) & 0xFF

    if key in key_actions:
        action, index, direction = key_actions[key]
        action(index, direction)
    elif key == 27:  # ESC key to exit
        break

cv.destroyAllWindows()
