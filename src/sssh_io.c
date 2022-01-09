#ifndef SSSH_IO
#define SSSH_IO

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sssh_declare.h"


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

		// If the the text is longer than the buffer keep allocating more
		// space.
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


/**
 * Split a line into execvp compatible tokens simply by whitespace. For sssh,
 * we will not tokenize strings.
*/ 
char** sssh_tokenize_line(char* line) {
	int buffer_length = SSSH_TOKEN_BUFFER;
	char** tokens = malloc(buffer_length * sizeof(char*));
	char* token;
	int position = 0;

	token = strtok(line, SSSH_TOKEN_DELIM); 
	while(token != NULL) {
		tokens[position] = token;
		position++;

		// Allocate more memory if we run out
		if(position >= buffer_length) {
			buffer_length += SSSH_TOKEN_BUFFER;
			tokens = realloc(tokens, buffer_length * sizeof(char*));

			if(!tokens) {
				fprintf(stderr, "sssh: Unable to allocate memory\n");
				exit(SSSH_MEM_ERROR);
			}
		}

		token = strtok(NULL, SSSH_TOKEN_DELIM);
	}

	tokens[position] = NULL; // needed for execvp
	return tokens;
}






#endif
