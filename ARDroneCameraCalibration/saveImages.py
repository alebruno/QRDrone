import time, sys, cv2, numpy
import ps_drone

# Startup sequence as suggested in the documentation of the psdrone library
drone = ps_drone.Drone()
drone.startup()

drone.reset()
while (drone.getBattery()[0] == -1): time.sleep(0.1)
drone.useDemoMode(True)

# Select front camera and use default resolution
drone.setConfigAllID()
drone.sdVideo()
drone.frontCam()

# Wait till the current configuration changes
CDC = drone.ConfigDataCount
while CDC == drone.ConfigDataCount: time.sleep(0.0001)

# Start the video
drone.startVideo()
drone.showVideo()

# Store the number of the current video frame
IMC =    drone.VideoImageCount
stop =   False
ground = False
counter = 0

# Define a touple with the termination criteria
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

# Define the 7x7 3D points corresponding to chessboard corners (0,0,0), (1,0,0), (2,0,0) ....,(6,6,0)
objp = numpy.zeros((7*7,3), numpy.float32)
objp[:,:2] = numpy.mgrid[0:7,0:7].T.reshape(-1,2) * 4

# Arrays to store 3D points and image points from all the images.
objpoints = [] # 3d points
imgpoints = [] # image points

# Collect a total of 30 images of the chesssboard for calibration
while (counter < 30):
    # If images are captured too fast, introduce here a delay with e.g. time.sleep(0.1)

    # Wait until the next video-frame, then update the counter
    while drone.VideoImageCount == IMC: time.sleep(0.01)
    IMC = drone.VideoImageCount

    # Retrieve BRG image, convert it to grayscale and show it
    colorImage = numpy.copy(drone.VideoImage)
    grayscaleImage = cv2.cvtColor(colorImage, cv2.COLOR_BGR2GRAY)
    cv2.imshow('Graycale image', grayscaleImage)

    # Find corners of the chessboard
    ret, corners = cv2.findChessboardCorners(grayscaleImage, (7,7),None)

    # Process the corners if they have been found
    if ret == True:

        # Add another copy of the 3D points for later processing
        objpoints.append(objp)

        # Refine the location of the corners at a subpixel level
        cv2.cornerSubPix(grayscaleImage,corners,(11,11),(-1,-1),criteria)

        # Add the corners for later processing
        imgpoints.append(corners)

        # Overlay the corners on the color image and display it
        cv2.drawChessboardCorners(colorImage, (7,7), corners,ret)
        cv2.imshow('img',colorImage)
        cv2.waitKey(500)

        # Save the grayscale image and the color image
        cv2.imwrite(str(counter) + '_corners.png',colorImage)
        cv2.imwrite(str(counter) + '_grayscale.png',grayscaleImage)
        counter = counter + 1

# Estimate intrinsic parameters 
ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, grayscaleImage.shape[::-1],None,None)

# Save and print relevant information for debugging
numpy.save('imgpoints', imgpoints)
numpy.save('objpoints', objpoints)
numpy.save('ret', ret)
numpy.save('mtx', mtx)
numpy.save('dist', dist)
numpy.save('rvecs', rvecs)
numpy.save('tvecs',tvecs)

print(ret)
print(mtx)
print(dist)
print(rvecs)
print(tvecs)
