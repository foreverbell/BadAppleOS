#!/usr/bin/env python
# encoding: utf-8

import os, sys

def encode(s) :
  lst = []
  for i in xrange(0, 10) :
    n = 0
    for j in xrange(0, 8) :
      k = i * 8 + j
      n |= (1 if (s[k] == ' ') else 0) << j
    lst.append(n)
  return lst

txt = sys.argv[1]
bin = sys.argv[2]

with open(txt, "r") as fin :
  with open(bin, "wb") as fout :
    content = fin.readlines()
    for line in content :
      fout.write(bytearray(encode(line)))
    fout.close()
  fin.close()
