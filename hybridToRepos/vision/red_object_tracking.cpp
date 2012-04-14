
/* COMPILE:::::
 $ g++ -g -I../cvBlob/ `pkg-config opencv --cflags --libs` ../cvBlob/*.cpp ../src/red_object_tracking.cpp -o red_object_tracking
 $ ./red_blob_tracking
*/

#include <iostream>
//#include <iomanip>

#include <cv.h>
#include <highgui.h>


#include <cvblob.h>
using namespace cvb;

int main()
{
  CvTracks tracks;

  cvNamedWindow("red_object_tracking",0);
  cvResizeWindow("red_object_tracking",500,500);

  cvNamedWindow("red_object_tracking", CV_WINDOW_AUTOSIZE);

  CvCapture *capture = cvCaptureFromCAM(1);
  cvGrabFrame(capture);
  IplImage *img = cvRetrieveFrame(capture);

  CvSize imgSize = cvGetSize(img);

  IplImage *frame = cvCreateImage(imgSize, img->depth, img->nChannels);

  IplConvKernel* morphKernel = cvCreateStructuringElementEx(5, 5, 1, 1, CV_SHAPE_RECT, NULL);


   unsigned int frameNumber = 0;

  while (cvGrabFrame(capture))
  {
    IplImage *img = cvRetrieveFrame(capture);

    cvConvertScale(img, frame, 1, 0);


    IplImage *segmentated = cvCreateImage(imgSize, 8, 1);
    
    // Detecting red pixels:
    // (This is very slow, use direct access better...)
    for (unsigned int j=0; j<imgSize.height; j++)
      for (unsigned int i=0; i<imgSize.width; i++)
      {
	CvScalar c = cvGet2D(frame, j, i);

	double b = ((double)c.val[0])/255.;
	double g = ((double)c.val[1])/255.;
	double r = ((double)c.val[2])/255.;
	unsigned char f = 255*((r>0.2+g)&&(r>0.2+b));

	cvSet2D(segmentated, j, i, CV_RGB(f, f, f));
      }


    // Convert the image into an HSV image
    IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
    cvCvtColor(img, imgHSV, CV_BGR2HSV);

/////
   // Calculate the moments to estimate the position of the ball
        CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
        cvMoments(segmentated, moments, 1);
 
        // The actual moment values
        double moment10 = cvGetSpatialMoment(moments, 1, 0);
        double moment01 = cvGetSpatialMoment(moments, 0, 1);
        double area = cvGetCentralMoment(moments, 0, 0);

       // Holding the last and current ball positions
//        static int posX = 0;
//        static int posY = 0;
           int posX = moment10/area; 
           int posY = moment01/area;

//        int lastX = posX;
//        int lastY = posY;
 
//        posX = moment10/area;
//        posY = moment01/area;


   if ((posX > 0) && (posY > 0))
   {
       if ( posX < 310) { std::cout << "A" << std::endl;  }

       if ( posX > 311) { std::cout << "D" << std::endl;  }

     //  if ( posY > 311) { std::cout << "W" << std::endl;  }

     //  if ( posX < 311) { std::cout << "S" << std::endl;  }

   }


   
////



    cvMorphologyEx(segmentated, segmentated, NULL, morphKernel, CV_MOP_OPEN, 1);

    ////// cvShowImage("segmentated", segmentated);

    IplImage *labelImg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_LABEL, 1);

    CvBlobs blobs;
    unsigned int result = cvLabel(segmentated, labelImg, blobs);
//    cvFilterByArea(blobs, 500, 1000000);
//    cvUpdateTracks(blobs, tracks, 200., 5);

    cvFilterByArea(blobs, 200, 10000);
    cvUpdateTracks(blobs, tracks, 300., 5); 


    cvRenderTracks(tracks, frame, frame, CV_TRACK_RENDER_ID|CV_TRACK_RENDER_BOUNDING_BOX);

    cvShowImage("red_object_tracking", frame);

    /*std::stringstream filename;
    filename << "redobject_" << std::setw(5) << std::setfill('0') << frameNumber << ".png";
    cvSaveImage(filename.str().c_str(), frame);*/

    cvReleaseBlobs(blobs);
    cvReleaseImage(&labelImg);
    cvReleaseImage(&segmentated);

    if ((cvWaitKey(10)&0xff)==27)
      break;

    frameNumber++;
  }

  cvReleaseStructuringElement(&morphKernel);
  cvReleaseImage(&frame);

  cvDestroyWindow("red_object_tracking");

  return 0;
}
