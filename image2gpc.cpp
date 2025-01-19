/** @file
 * image2gpc
 * convert a pixel image file to a Chronus GPC script
 * https://github.com/doj/image2gpc
 *
 * https://guide.cronus.support/gpc
 */

#define STB_IMAGE_IMPLEMENTATION 1
#define STBI_FAILURE_USERMSG 1
#include "stb_image.h"

#include <iostream>
#include <sysexits.h>

int main(int argc, char **argv)
{
  // parse command line
  if (argc != 2)
  {
    std::cerr << "usage: image2gpc <filename>\n";
    return EX_USAGE;
  }
  const char *filename = argv[1];

  // load image
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

  // check image
  if (w > 128)
  {
    std::clog << "image width " << w << " > 128\n";
  }
  if (h > 64)
  {
    std::clog << "image height " << h << " > 64\n";
  }
  if (n != 1)
  {
    std::cerr << "The image has channels=" << n << ". Only expect 1 channel (grayscale)\n";
    return EX_DATAERR;
  }

  std::cout << "// image created with https://github.com/doj/image2gpc\n";
  std::cout << "define image_width  = " << w << "; // image width in pixels\n";
  std::cout << "define image_height = " << h << "; // image height in pixels\n";
  std::cout << "const uint16 image_data[][] = {\n";
  for(int y = 0; y < h; ++y)
  {
    std::cout << "  { ";
    uint16_t b = 0;
    int x;
    for(x = 0; x < w; ++x)
    {
      if (data[y*w + x] >= 128)
      {
        b |= (1 << (x & 15));
      }
      if ((x & 15) == 15)
      {
        std::cout << b;
        b = 0;
        if (x < w-1)
        {
          std::cout << ',';
        }
      }
    }
    if ((x & 15) != 0)
    {
      std::cout << b;
    }
    std::cout << " }";
    if (y < h-1)
    {
      std::cout << ',';
    }
    std::cout << '\n';
  }
  std::cout << "};\n";

  std::cout << R"(
// @return OLED_WHITE if @p val is TRUE  and @p invert is FALSE.
// @return OLED_BLACK if @p val is FALSE and @p invert is FALSE.
// @return OLED_BLACK if @p val is TRUE  and @p invert is TRUE.
// @return OLED_WHITE if @p val is FALSE and @p invert is TRUE.
function pixel_invert(val, invert)
{
  if (invert)
  {
    if (val)
    {
      return OLED_BLACK;
    }
    return OLED_WHITE;
  }
  if (val)
  {
    return OLED_WHITE;
  }
  return OLED_BLACK;
}

int image_x; // variable used by draw_image()
int image_y; // variable used by draw_image()
// draw the image defined in the data() section to the OLED display
// @param pos_x top left X position
// @param pos_y top left Y position
// @param invert if FALSE draw the image as defined in the data() section,
//               if TRUE draw an inverted image.
function draw_image(pos_x, pos_y, invert)
{
  for(image_y = 0; image_y < image_height; image_y++)
  {
    for(image_x = 0; image_x < image_width; image_x++)
    {
      pixel_oled(pos_x + image_x,
                 pos_y + image_y,
                 pixel_invert(test_bit(image_data[image_y][image_x/16],
                                       image_x & 15),
      invert));
    }
  }
}
)";

  stbi_image_free(data);
  return EX_OK;
}
