import numpy
import cv2
import glob

ret = numpy.load('ret.npy')
mtx = numpy.load('mtx.npy')
dist = numpy.load('dist.npy')

print(ret)
print(mtx)
print(dist)

# Make a list of the grayscale images
images = glob.glob('*grayscale.png')

for fname in images:
    img = cv2.imread(fname)
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    h,  w = img.shape[:2]
    newcameramtx, roi=cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),0,(w,h))

    # Undistort each image
    dst = cv2.undistort(img, mtx, dist, None, newcameramtx)

    # Crop undistorted image to the same size of the original
    x,y,w,h = roi
    dst = dst[y:y+h, x:x+w]

    # Show the original and the undistorted images
    cv2.imshow('img',img)
    cv2.imshow('dst',dst)
    cv2.waitKey(500)

cv2.destroyAllWindows()

