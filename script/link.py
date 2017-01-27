#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, sys

boot = sys.argv[1]
kernel = sys.argv[2]
output = sys.argv[3]

boot_size = os.stat(boot).st_size
kernel_size = os.stat(kernel).st_size

assert boot_size == 512

new_kernel_size = (kernel_size + 512 - 1) / 512 * 512

print "Kernel size %d -> %d." % (kernel_size, new_kernel_size)
with open(kernel, "ab") as f:
  padding = new_kernel_size - kernel_size
  assert padding >= 0
  f.write(bytearray(padding))

print "Populating 'kernel_sectors' in bootloader."
with open(boot, "r+b") as f:
  f.seek(2)
  assert new_kernel_size % 512 == 0
  nsectors = new_kernel_size / 512
  print "nsectors = %d." % nsectors
  bytes2 = [ nsectors % 256, nsectors / 256 ]
  f.write(bytearray(bytes2))

os.system("cat %s %s > %s" % (boot, kernel, output))

print "Done!"
