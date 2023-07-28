#!/usr/bin/env python

import os
import sys


def little_endian(n):
  xs = []
  for i in range(0, 4):
    xs.append(n % 256)
    n //= 256
  return xs


if __name__ == "__main__":
  boot = sys.argv[1]
  kernel = sys.argv[2]
  output = sys.argv[3]

  boot_size = os.stat(boot).st_size
  old_kernel_size = os.stat(kernel).st_size
  kernel_size = (old_kernel_size + 512 - 1) // 512 * 512

  if boot_size != 512:
    print("Size of boot loader is not 512.")
    sys.exit(1)

  print("Kernel size %d -> %d." % (old_kernel_size, kernel_size))
  with open(kernel, "ab") as f:
    padding = kernel_size - old_kernel_size
    f.write(bytearray(padding))

  print("Populating kernel_sectors in bootloader.")
  with open(boot, "r+b") as f:
    f.seek(2)
    byte2 = little_endian(kernel_size // 512)[:2]
    f.write(bytearray(byte2))

  os.system("cat %s %s > %s" % (boot, kernel, output))

  print("\033[92mDone!\033[0m")
