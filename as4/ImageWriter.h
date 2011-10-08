#ifndef _IMAGEWRITER_H
#define _IMAGEWRITER_H
#include <freeimage.h>
#include <vector>
#include <algorithm>

void save_opengl_image(int width, int height, char* filename, vector< vector<Vector3d> > &img)
{
  // Make the BYTE array, factor of 3 because it's RBG.
  BYTE* pixels = new BYTE[ 3 * width * height];
  for( int i = 0; i < width; i++ ) {
    for( int j = 0; j < height; j++ ) {
      pixels[(i+j*width)*3] = (BYTE) min(255, (int) (img[i][j](2) * 255));
      pixels[(i+j*width)*3+1] = (BYTE) min(255, (int) (img[i][j](1) * 255));
      pixels[(i+j*width)*3+2] = (BYTE) min(255, (int) (img[i][j](0) * 255));
    }
  }
  // Convert to FreeImage format & save to file
  FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);
  FreeImage_Save(FIF_PNG, image, filename, 0);
  //
  delete image;
  delete pixels;
} 
#endif //_IMAGEWRITER_H
