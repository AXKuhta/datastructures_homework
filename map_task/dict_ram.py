import sys

if len(sys.argv) < 2:
	print("Usage: python dict_ram.py 1000")
	exit(-1)

d = dict()

for i in range(int(sys.argv[1])):
	d[i] = i

sys.stdout.write("OK\n")
sys.stdout.flush()
sys.stdin.read(1)
