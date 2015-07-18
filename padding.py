#!/usr/bin/env python
# encoding: utf-8

import os, sys

img = sys.argv[1]
tsize = int(sys.argv[2])
fsize = os.stat(img).st_size

print "The original size of", img, "is", fsize

f = open(img, "ab")
padding = tsize - fsize

if (padding < 0) :
  print "Error!", img, "is too big!"
else :
  f.write(bytearray(padding))
  print "Padding", padding, "bytes"

f.close()
