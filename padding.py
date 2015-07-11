#!/usr/bin/env python
# encoding: utf-8

import os, sys

img = sys.argv[1]
size = os.path.getsize(img)

print "The original size of", img, "is", size

f = open(img, "ab")
padding = 1023 * 512 - size

if (padding < 0) :
  print "Error!", img, "is too big!"
else :
  f.write(bytearray(padding))
  print "Padding", padding, "bytes"

f.close()
