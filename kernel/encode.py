#!/usr/bin/env python
# encoding: utf-8

import os, sys

def encode(s, ch) :
  if (len(s) % 8 != 0) :
    s += [ch] * (8 - (len(s) % 8))
  lst = []
  for i in xrange(0, len(s) / 8) :
    n = 0
    for j in xrange(0, 8) :
      k = i * 8 + j
      n |= (1 if (s[k] == ch) else 0) << j
    lst.append(n)
  return lst

txt = sys.argv[1]
bin = sys.argv[2]
data = []

with open(txt, "r") as fin :
  with open(bin, "wb") as fout :
    content = ''.join(fin.readlines())
    content = ''.join(content.splitlines())
    fout.write(bytearray(encode(content, '*')))
    fout.close()
  fin.close()
 