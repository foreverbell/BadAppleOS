#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, sys
from heapq import *

class hnode(object):
  def __init__(self, left=None, right=None):
    self.left = left
    self.right = right

def build(freq):
  queue = []
  for i in range(0, 256):
    if (freq[i] != 0):
      heappush(queue, (freq[i], i))
  while (len(queue) > 1):
    a = heappop(queue)
    b = heappop(queue)
    node = hnode(a, b)
    heappush(queue, (a[0] + b[0], node))
  return queue[0]

def walk(node, prefix="", code={}):
  f, n = node
  if (isinstance(n, hnode)):
    walk(n.left, prefix + "0", code)
    walk(n.right, prefix + "1", code)
  else:
    code[n] = prefix
  return code

def huffman(lst):
  freq = [0] * 256
  for n in lst:
    freq[n] += 1
  return walk(build(freq))

def encode(s, ch):
  while (len(s) % 8 != 0):
    s += ch
  lst = []
  for i in range(0, len(s) / 8):
    n = 0
    for j in range(0, 8):
      k = i * 8 + j
      n |= (1 if (s[k] == ch) else 0) << j
    lst.append(n)
  return lst

def compress(content):
  fcount = len(content) / 80 / 25  # frame_count
  edata = encode(content, '*')
  elen = len(edata)
  code = huffman(edata)
  hdata = ""
  for item in edata:
    hdata += code[item]
  code_bytes = [  # reverse the endian
      fcount >> 8, fcount & 0xff, elen >> 24, (elen >> 16) & 0xff,
      (elen >> 8) & 0xff, elen & 0xff,
      len(code)
  ]
  for key, value in code.iteritems():
    code_bytes.append(key)
    tmp = encode(value, '1')
    code_bytes.append(len(value))
    code_bytes += tmp
  return code_bytes + encode(hdata, '1')

if __name__ == "__main__":
  txt, bin = sys.argv[1], sys.argv[2]
  data = []

  with open(txt, "r") as fin:
    with open(bin, "wb") as fout:
      content = ''.join(fin.readlines())
      content = ''.join(content.splitlines())
      fout.write(bytearray(compress(content)))
