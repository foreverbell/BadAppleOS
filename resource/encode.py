#!/usr/bin/env python

import os
import sys
from heapq import *


class hnode(object):
  def __init__(self, freq, val=None, left=None, right=None):
    self.freq = freq
    self.val = val
    self.left = left
    self.right = right

  def is_leaf(self):
    return self.val is not None

  def __lt__(self, r):
    return self.freq < r.freq


def build(freq):
  queue = []
  for i in range(0, 256):
    if freq[i] != 0:
      heappush(queue, hnode(freq[i], val=i))
  while len(queue) > 1:
    a = heappop(queue)
    b = heappop(queue)
    heappush(queue, hnode(a.freq + b.freq, left=a, right=b))
  return queue[0]


def walk(node, prefix="", code={}):
  if node.is_leaf():
    code[node.val] = prefix
  else:
    walk(node.left, prefix + "0", code)
    walk(node.right, prefix + "1", code)
  return code


def huffman(lst):
  freq = [0] * 256
  for n in lst:
    freq[n] += 1
  return walk(build(freq))


def encode(s, ch):
  while len(s) % 8 != 0:
    s += ch
  lst = []
  for i in range(0, len(s) // 8):
    n = 0
    for j in range(0, 8):
      k = i * 8 + j
      n |= (1 if (s[k] == ch) else 0) << j
    lst.append(n)
  return lst


def compress(content):
  fcount = len(content) // 80 // 25  # frame_count
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
  for key, value in code.items():
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
