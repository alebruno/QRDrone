#include <iostream>
#include <zbar.h>
#include "../lib/cvdrone/src/ardrone/ardrone.h"
using namespace zbar;
using namespace cv;
using namespace std;

typedef struct
{
  string type;
  string data;
  vector <Point> location;
} decodedObject;

// The following functions are adapted from the tutorial example https://github.com/spmallick/learnopencv/blob/master/barcode-QRcodeScanner/barcode-QRcodeScanner.cpp

// Find and decode barcodes and QR codes
void decode(Mat &im, vector<decodedObject>&decodedObjects)
{
  
  // Create zbar scanner
  ImageScanner scanner;

  // Configure scanner
  scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
  
  // Convert image to grayscale
  Mat imGray;
  cvtColor(im, imGray,COLOR_BGR2GRAY);

  // Wrap image data in a zbar image
  Image image(im.cols, im.rows, "Y800", (uchar *)imGray.data, im.cols * im.rows);

  // Scan the image for barcodes and QRCodes
  int n = scanner.scan(image);
  
  // Print results
  for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
  {
    decodedObject obj;
    
    obj.type = symbol->get_type_name();
    obj.data = symbol->get_data();
    
    // Print type and data
    cout << "Type : " << obj.type << endl;
    cout << "Data : " << obj.data << endl << endl;
    
    // Obtain location
    for(int i = 0; i< symbol->get_location_size(); i++)
    {
      obj.location.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
    }
    
    decodedObjects.push_back(obj);
  }
}


// Display barcode and QR code location  
void display(Mat &im, vector<decodedObject>&decodedObjects, string name)
{
  // Loop over all decoded objects
  for(int i = 0; i < decodedObjects.size(); i++)
  {
    vector<Point> points = decodedObjects[i].location;
    vector<Point> hull;
    
    // If the points do not form a quad, find convex hull
    if(points.size() > 4)
      convexHull(points, hull);
    else
      hull = points;
    
    // Number of points in the convex hull
    int n = hull.size();
    
    for(int j = 0; j < n; j++)
    {
      line(im, hull[j], hull[ (j+1) % n], Scalar(255,0,0), 3);
    }
  }

  // Display results 
  imshow(name.c_str(), im);  
}

// Application QRDrone
int main(int argc, char *argv[])
{
    std::cout << "Running QRDrone" << std::endl;
    
    // Initialize the camera matrix and the vector of distorsion coefficients
    Mat distCoeffs = Mat::zeros(5, 1, CV_64F);
    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);

    // Fill the camera matrix and the vector of distorsion coefficients and print them
    cameraMatrix.at<double>(0,0) = 587.81239186F;
    cameraMatrix.at<double>(1,1) = 602.71124829F;
    cameraMatrix.at<double>(2,2) = 1.0F;
    cameraMatrix.at<double>(0,2) = 327.56233304F;
    cameraMatrix.at<double>(1,2) = 198.4306577F;
    distCoeffs.at<double>(0,0) = -0.66037173F;
    distCoeffs.at<double>(1,0) = 0.88396557F;
    distCoeffs.at<double>(2,0) = -0.00291488F;
    distCoeffs.at<double>(3,0) = -0.0029825F;
    distCoeffs.at<double>(4,0) = -0.81633575F;
    std::cout << "cameraMatrix " << cameraMatrix << std::endl;
    std::cout << "distCoeffs " << distCoeffs << std::endl;

    // Initialize an ARDrone class
    ARDrone ardrone("192.168.1.1");

    // Initialize the ImageScanner class of the ZBar library
    ImageScanner scanner;
 
    // Configure scanner
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
   
    // Stay in the loop untill a key is pressed
    while (true) {
      // Declare the vectors of decoded object for the distorted and the undistorted images
	    vector<decodedObject> decodedObjects;
      vector<decodedObject> decodedObjectsUndistorted;

      // Get an image from the drone
      cv::Mat image = ardrone.getImage();
      
      // Undistort the image using the given parameters
      Mat undistorted = image.clone();
      undistort(image, undistorted, cameraMatrix, distCoeffs);

      // Decode barcodes and QR codes and print them to stdout
      decode(image, decodedObjects);
      decode(undistorted, decodedObjectsUndistorted);

      // Print distorted and undistorted picture with an overlay of the detected barcodes and QR codes
      display(image, decodedObjects, "DetectedDistorted");
      display(undistorted, decodedObjectsUndistorted, "DetectedUndistorted");
      sleep(0.05);

      // Press Esc to exit
      if (cv::waitKey(1) == 0x1b) break;
    }
  return 0;
}
