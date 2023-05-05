from collections import Counter
from random import random
from time import time_ns
import os

def crc(bstr: bytes):
	h = 0

	for byte in bstr:
		highorder = h & 0xf8000000
		h = h & 0xffffffff
		h = h << 5
		h = h ^ (highorder >> 27)
		h = h ^ byte

	return h

def pjw(bstr: bytes):
	h = 0

	for byte in bstr:
		h = (h << 4) + byte
		h = h & 0xffffffff
		g = h & 0xf0000000
		if (g != 0):
			h = h ^ (g >> 24)
			h = h ^ g

	return h

R = [int(2**32 * random()) for i in range(256)]

def buz(bstr: bytes):
	h = 0

	for byte in bstr:
		highorder = h & 0x80000000
		h = h << 1
		h = h & 0xffffffff
		h = h ^ (highorder >> 31)
		h = h ^ R[byte]

	return h

def find_duplicates(files: list[str], hash_function: callable) -> list[str]:
	hashlist = []

	total_time_ns = 0

	for file in files:
		with open(file, "rb") as f:
			bstr = f.read()

			start = time_ns()
			result = hash_function(bstr)
			total_time_ns += (time_ns() - start)

			hashlist.append(result)

	count = Counter(hashlist)

	duplicates = 0

	for k, v in count.items():
		if v > 1:
			duplicates += 1

	print("=====", hash_function.__name__, "=====")
	print("Duplicates:", duplicates)
	print("Time:", total_time_ns, "ns")

path = "out/"
filelist = [path + x for x in os.listdir(path)]

find_duplicates(filelist, crc)
find_duplicates(filelist, pjw)
find_duplicates(filelist, buz)
find_duplicates(filelist, hash)
