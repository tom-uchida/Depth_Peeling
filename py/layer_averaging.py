# layer_averaging.py
#   Tomomasa Uchida
#   2020/08/29

import numpy as np
import cv2

# Check arguments
import sys
args = sys.argv
if len(args) != 4:
    print("\nUSAGE   : $ python layer_averaging.py [input_images_path] [num_of_layers] [image_resolution]")
    print("EXAMPLE : $ python layer_averaging.py ../IMAGE_DATA 10 512\n")
    sys.exit()



def ReadImage( _img_name ):
    # read input image
    img_BGR = cv2.imread(_img_name)

    # convert color BGR to RGB
    img_RGB = cv2.cvtColor(img_BGR, cv2.COLOR_BGR2RGB)

    return img_RGB



def run( _num_of_layers, _image_resol, _serial_img_path ):
    # Prepare empty numpy array
    R_pixel_values = np.empty( (_image_resol*1, _image_resol*1, _num_of_layers), np.uint8 )
    G_pixel_values = np.empty( (_image_resol*1, _image_resol*1, _num_of_layers), np.uint8 )
    B_pixel_values = np.empty( (_image_resol*1, _image_resol*1, _num_of_layers), np.uint8 )

    # Read intermediate images
    for i in range( _num_of_layers ):
        # Read each ensemble image
        tmp_image_RGB = ReadImage( _serial_img_path + "Layer"+str(i+1)+".bmp" )

        # Split into RGB and add to numpy array
        R_pixel_values[:,:,i] = tmp_image_RGB[:,:,0] # R
        G_pixel_values[:,:,i] = tmp_image_RGB[:,:,1] # G
        B_pixel_values[:,:,i] = tmp_image_RGB[:,:,2] # B

        if i == _num_of_layers-1:
            print("R :", R_pixel_values.shape)
            print("G :", G_pixel_values.shape)
            print("B :", B_pixel_values.shape)
    # end for i

    R_mean = np.mean(R_pixel_values, axis=2)
    G_mean = np.mean(G_pixel_values, axis=2)
    B_mean = np.mean(B_pixel_values, axis=2)
    R_mean = R_mean.astype(np.uint8)
    G_mean = G_mean.astype(np.uint8)
    B_mean = B_mean.astype(np.uint8)
    # (1000, 1000)

    # Combine R, G and B arrays
    # (3, 1000, 1000) → (1000, 1000, 3)
    # (0,    1,    2) → (   1,    2, 0)
    RGB_mean = np.array([R_mean, G_mean, B_mean]).transpose((1, 2, 0))
    
    # Save the result image
    RGB_mean_BGR = cv2.cvtColor(RGB_mean, cv2.COLOR_RGB2BGR)
    cv2.imwrite("OUTPUT_DATA/Layer_Averaging.png", RGB_mean_BGR)



if __name__ == "__main__":
    # Set the number of layers
    num_of_layers = int(args[2])
    print("Number of Layers :", num_of_layers)

    # Set image resolution
    image_resol = int(args[3])
    print("Image Resolution :", image_resol)

    # Read target images
    serial_img_path = args[1] + "/"
    run( num_of_layers, image_resol, serial_img_path )