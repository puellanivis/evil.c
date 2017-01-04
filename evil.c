#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ADD_COUNT 24
#define MUL_COUNT 5
#define DIV_COUNT 3

#define MAX_PAGE  (1 << 28)

int n = 0;
char **pages;
int PAGE_SIZE;

void setup_memory(void) {
	pages = (char **) malloc(MAX_PAGE * sizeof(char *));

	n = 0;
	while(n < MAX_PAGE) {
		pages[n] = malloc(PAGE_SIZE);

		if(pages[n] == NULL) {
			printf("Final Page: %i\n", n);
			break;
		}

		n++;
	}

	printf("Pages: %i\n", n);
}

void tax_memory(void) {
	int x = 0;

	while (1) {
		x += pages[random() % n][random() % PAGE_SIZE] + random();
		pages[random() % n][random() % PAGE_SIZE] = x;
	}
}

void tax_cpu(void) {
	int i = 0, j = 0, p;

	i = random() % (n - 1);

	for (j = 0; j < PAGE_SIZE; j++) {
		pages[i][j] = random();
		pages[i+1][j] = random();
	}

	while (1) {
		for (p = 0; p < ADD_COUNT; p++) {
			pages[i][j] += pages[i+1][j++];
			j %= PAGE_SIZE;
		}

		for (p = 0; p < MUL_COUNT; p++) {
			pages[i][j] *= pages[i+1][j++];
			j %= PAGE_SIZE;
		}

		for (p = 0; p < DIV_COUNT; p++) {
			pages[i][j] /= pages[i+1][j++];
			j %= PAGE_SIZE;
		}
	}
}

int main(void) {
	pid_t pid;
	int p = 0;

 	PAGE_SIZE = getpagesize();

	srandom(time(NULL));

	do {
		pid = fork();
		p++;
	} while (pid);

	setup_memory();

	if (p & 0x03) {
		tax_cpu();
	} else {
		tax_memory();
	}

	return 0;
}
