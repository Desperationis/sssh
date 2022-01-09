#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* sssh_cl_prefix();
char* sssh_readline();
char** sssh_tokenize_line(char*);
int sssh_execute(char**);

#define SSSH_RL_BUFFER 40
#define SSSH_MEM_ERROR 1

int main(int argc, char** argv) {
	int status = 0;

	while (status == 0) {
		char* line;
		char** args;
		char* cl_prefix;

		cl_prefix = sssh_cl_prefix();
		printf("%s ", cl_prefix);


		line = sssh_readline();
		//args = sssh_tokenize_line(line);
		//status = sssh_execute(args); // non-zero on exit
		

		// echo it back to test
		printf("%s\n", line);

		free(line);
		free(args);
		free(cl_prefix);
	}

	return 0;
}

/**
 * Return a pointer to the command line prefix. I.e. the part in the bash
 * command line that is something like "user@hostname:pwd~".
 *
 * This should not have a space at the end of it.
*/ 
char* sssh_cl_prefix() {
	char* buffer = malloc(sizeof(char) * 40);
	strcpy(buffer, "$");	

	return buffer;
}

/**
 * Read line from user. To maximize compatibility, don't use GNU's readline().
*/ 
char* sssh_readline() {
	int buffer_length = SSSH_RL_BUFFER;
	char* buffer = malloc(buffer_length * sizeof(char));
	int buffer_pos = 0;
	int c;

	if(!buffer) {
		fprintf(stderr, "sssh: Unable to allocate memory\n");
		exit(SSSH_MEM_ERROR);
	}

	while (1) {
		c = getchar();

		if(c == EOF || c == '\n') {
			buffer[buffer_pos] = '\0';
			return buffer;
		}

		buffer[buffer_pos] = c;
		buffer_pos++;

		/**
		 * If the the text is longer than the buffer keep allocating more space.
		*/ 
		if(buffer_pos >= buffer_length) {
			buffer_length += SSSH_RL_BUFFER;
			buffer = realloc(buffer, buffer_length * sizeof(char));
			
			if(!buffer) {
				fprintf(stderr, "sssh: Unable to allocate memory\n");
				exit(SSSH_MEM_ERROR);
			}
		}
	}
}
