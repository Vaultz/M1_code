/*!\file window.cpp
 *
 * \brief Utilisation du CascadeClassifier pour détecter des visages et les yeux
 *
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date October 15 2015
 */
#include <stdio.h>
#include <assert.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <objdetect.hpp>

using namespace cv;
using namespace std;

int main(int argc, char ** argv) {
  CascadeClassifier * face_cc = new CascadeClassifier("haarcascade_frontalface_default.xml");
  CascadeClassifier * eye_cc = new CascadeClassifier("haarcascade_eye.xml");
  if(face_cc == NULL || eye_cc == NULL)
    return 1;
  Mat ci = imread("visages.jpg");
  Mat gsi = imread("visages.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  cvNamedWindow("Face detection", CV_WINDOW_AUTOSIZE);
  while(1) {
    vector<Rect> faces;
    face_cc->detectMultiScale(gsi, faces, 1.3, 5);
    for (vector<Rect>::iterator fc = faces.begin(); fc != faces.end(); ++fc) {
      rectangle(ci, (*fc).tl(), (*fc).br(), Scalar(0, 255, 0), 2, CV_AA);
      Mat gsi_roi = gsi(*fc); /* sub image of gsi according to the rest *fc */
      vector<Rect> eyes;
      eye_cc->detectMultiScale(gsi_roi, eyes, 1.3, 10);
      for (vector<Rect>::iterator ec = eyes.begin(); ec != eyes.end(); ++ec)
	rectangle(ci, (*fc).tl() + (*ec).tl(), (*fc).tl() + (*ec).br(), Scalar(0, 0, 255), 2, CV_AA);
    }
    imshow("Face detection", ci);
    if((cvWaitKey(10) & 0xFF) == 27)
      break;
  }
  return 0;
}
