/** @file
 * image2gpc
 * convert a pixel image file to a Chronus GPC script
 * https://github.com/doj/image2gpc
 */

#define STB_IMAGE_IMPLEMENTATION 1
#define STBI_FAILURE_USERMSG 1
#include "stb_image.h"

#include <iostream>
#include <sysexits.h>

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr << "usage: image2gpc <filename>\n";
    return EX_USAGE;
  }
  const char *filename = argv[1];

  int w,h,n;
  unsigned char *data = stbi_load(filename, &w, &h, &n, 1);
  if (! data)
  {
    auto ok = stbi_info(filename, &w, &h, &n);
    std::cerr << "could not load " << filename << '\n'
              << stbi_failure_reason() << '\n';
    if (ok)
    {
      std::cerr << "The image has width=" << w << " height=" << h << " channels=" << n << '\n';
    }
    return EX_NOINPUT;
  }

  std::cout << "define image_width  = " << w << ";\n";
  std::cout << "define image_height = " << h << ";\n";
  std::cout << "data(";
  for(int y = 0; y < h; ++y)
  {
    uint8_t b = 0;
    int x;
    for(x = 0; x < w; ++x)
    {
      if (data[y*w + x] >= 128)
      {
        b |= (1 << (x & 7));
      }
      if ((x & 7) == 7)
      {
        std::cout << static_cast<int>(b) << ',';
        b = 0;
      }
    }
    if ((x & 7) != 0)
    {
      std::cout << static_cast<int>(b) << ',';
    }
    std::cout << '\n';
  }
  std::cout << ");\n";




  stbi_image_free(data);
  return EX_OK;
}
