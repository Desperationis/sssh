#ifndef SSSH_BUILTINS 
#define SSSH_BUILTINS

#include "sssh_declare.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

char* sssh_builtin_str[] = {
	"cd",
	"help",
	"exit"
};

// Function pointer array
int (*sssh_builtin_func[])(char**) = {
	sssh_cd,
	sssh_help,
	sssh_exit
};

int sssh_num_builtins() {
	return sizeof(sssh_builtin_str) / sizeof(char*);
}

int sssh_cd(char** args) {
	if(args[1] == NULL) {
		fprintf(stderr, "sssh: Expected path to \"cd\"\n");
	}
	else {
		if(chdir(args[1]) != 0) {
			perror("sssh");
		}
	}

	return 0;
}

int sssh_help(char** args) {
	int i;
	printf("Diego Contreras' Super Simple Shell.\n");
	printf("The following are built ins:\n");

	for(i = 0; i < sssh_num_builtins(); i++) {
		printf("\t%s\n", sssh_builtin_str[i]);
	}

	printf("Use \"man\" for information on any other programs.\n");

	return 0;
}

int sssh_exit(char** args) {
	return 1;
}



#endif
