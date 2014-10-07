#include "isolateGlove.h"

Rect locateGlove(Mat cameraFrame) {
  Rect gloveLocation;
  //Add smarts

  gloveLocation = Rect( (iWidth/2.0), (iHeight/2.0), 200, 200);
  return gloveLocation;
}

//If in future may be worth it to merge this with cleanupImage so only a single cycle over fullsize image. But current is better for code clarity
Mat reduceDimensions(Mat region, int targetWidth, int targetHeight) {
  int horizontalSkip = region.cols / targetWidth; 
  int verticalSkip = region.rows / targetHeight;

  //Create new Mat large enough to hold glove image
  Mat shrunkFrame = region(Rect(0,0,targetWidth, targetHeight)).clone();

  int numRows = shrunkFrame.rows;
  int numChannelsInImage = shrunkFrame.channels();
  int numColumns = shrunkFrame.cols * numChannelsInImage;

  //Clear matrix to black square: (todo: Figure out OpenCV Mat constructor "type" field that creates matrix of same colour type as region given any camera input)
  for (int i=0;i< numRows; ++i){ 
    uchar *p = shrunkFrame.ptr<uchar>(i);
    for (int j=0;j<numColumns;++j){
      p[j] = 0;
    }
  }

  //Reduce colour depth reduction table (simplifies future comparisons)
  /*uchar table[256];
  for (int i=0;i< 256; ++i) {
    //table[i] = (uchar)(10 * (i/10));
    table[i] = (uchar)i; //No pixel colour change
    }*/

  //Shrink image by merging adjacent pixels in square
  for (int i=0;i< numRows; ++i){ 
    uchar *p = shrunkFrame.ptr<uchar>(i);
    for (int j=0;j<numColumns;++j){
      //p[j] = table[region.ptr<uchar>(i*horizontalSkip)[j*verticalSkip]]/(horizontalSkip*verticalSkip);
      p[j] = region.ptr<uchar>(i*horizontalSkip)[j*verticalSkip];
      //Merge adjacent pixels:
      /*for (int k=0; k<horizontalSkip; k++){
	p[j] += table[region.ptr<uchar>(i*horizontalSkip+k)[j*verticalSkip]]/(horizontalSkip*verticalSkip);
      }
      for (int l=0; l<verticalSkip; l++){
	p[j] += table[region.ptr<uchar>(i*horizontalSkip)[j*verticalSkip+l]]/(horizontalSkip*verticalSkip);
	}*/
    }
  }


  return (shrunkFrame);
}
