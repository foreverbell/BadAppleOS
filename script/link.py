#!/usr/bin/env python
# encoding: utf-8

import os, sys
from subprocess import call

binary = [sys.argv[1], sys.argv[2], sys.argv[3]]
output = sys.argv[4]

size = [0, 0, 0]
size_aligned = [0, 0, 0]

for i in xrange(0, 3) :
  size[i] = os.stat(binary[i]).st_size

assert size[0] == 512
size_aligned[0] = size[0]

size_aligned[1] = (size[1] - 1) / 512 + 1
while (size_aligned[1] % 8 != 7) :
  size_aligned[1] += 1
size_aligned[1] *= 512

size_aligned[2] = (size[2] - 1) / 4096 + 1
size_aligned[2] *= 4096

for i in xrange(0, 3) :
  print "The original size of binary %s is %d, aligned to %d." % (binary[i], size[i], size_aligned[i])
  with open(binary[i], "ab") as f :
    padding = size_aligned[i] - size[i]
    assert padding >= 0
    f.write(bytearray(padding))
    f.close()

print "Linking."
with open(binary[0], "r+b") as f :
  f.seek(2)
  s1 = size_aligned[1] / 512
  s2 = size_aligned[2] / 512
  assert (s1 * 512 == size_aligned[1])
  assert (s2 * 512 == size_aligned[2])
  bytes4 = [
    s1 & 0xff, 
    s1 >> 8,
    s2 & 0xff, 
    s2 >> 8,
  ]
  f.write(bytearray(bytes4))
  f.close()

os.system("cat " + sys.argv[1] + " " + sys.argv[2] + " " +  sys.argv[3] + " > " + sys.argv[4])

print "Done!"