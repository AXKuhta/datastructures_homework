#include <cstdlib>
#include <cstdio>
#include <map>

void run_test(int test_size) {
	std::map<int, int> map;

	for (int i = 0; i < test_size; i++) {
		map[i] = i;
	}

	printf("OK\n");
	fflush(stdout);
	getchar();
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Usage: ./map_ram.exe 1000\n");
		exit(-1);
	}

	int test_size = strtod(argv[1], NULL);

	run_test(test_size);

	return 0;
}
