#include <stdio.h>
#include <string.h>

void vulnerable(char *input){
	char buffer[16];

	strcpy(buffer, input);
}

int main(int argc, char **argv) {

	vulnerable(argv[1]);

	return 0;
}

