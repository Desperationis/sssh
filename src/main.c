#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "sssh_declare.h"
#include "sssh_builtins.c"
#include "sssh_io.c"
#include "sssh_core.c"

int main(int argc, char** argv) {
	int status = 0;

	while (status == 0) {
		char* line;
		char** args;
		char* cl_prefix;

		cl_prefix = sssh_cl_prefix();
		printf("%s ", cl_prefix);


		line = sssh_readline();

		args = sssh_tokenize_line(line);
		status = sssh_execute(args); // non-zero on exit

		free(line);
		free(args);
		free(cl_prefix);
	}

	return 0;
}
