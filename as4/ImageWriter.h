#ifndef _IMAGEWRITER_H
#define _IMAGEWRITER_H
#include <freeimage.h>

void save_opengl_image(int width, int height, char* filename)
{
  // Make the BYTE array, factor of 3 because it's RBG.
  BYTE* pixels = new BYTE[ 3 * width * height];
  glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, pixels);
  // Convert to FreeImage format & save to file
  FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, 0xFF0000, 0x0000FF, 0x00FF00, false);
  FreeImage_Save(FIF_PNG, image, filename, 0);
  //
  delete image;
  delete pixels;
} 
#endif //_IMAGEWRITER_H
