image2gpc
==========

convert a pixel image file to a Chronus GPC script

https://github.com/doj/image2gpc

Supports the following image file formats: JPEG, PNG, BMP, PSD, TGA, GIF, HDR, PIC, PNM (.ppm and .pgm)

Because the Chronus Zen only has a black and white display,
the image2gpc program only considers grayscale input values.
Maximum image size: 128x64
A pixel of the input image with a value < 128 will be black,
a pixel of the input image with a value >= 128 will be white.

Usage
------

image2gpc <filename>

GPC Example
------------

Study the GPC scripts on https://www.cubic.org/~doj/ps3/ how the image data
created by this program can be used.

Contact
--------

write an email to Dirk Jagdmann <doj@cubic.org>
