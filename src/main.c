#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char* sssh_cl_prefix();
char* sssh_readline();
char** sssh_tokenize_line(char*);

int sssh_command(char**);
int sssh_execute(char**);

int sssh_cd(char** args);
int sssh_help(char** args);
int sssh_exit(char** args);
int sssh_num_builtins();

char* builtin_str[] = {
	"cd",
	"help",
	"exit"
};

// Function pointer array
int (*builtin_func[])(char**) = {
	sssh_cd,
	sssh_help,
	sssh_exit
};

#define SSSH_RL_BUFFER 50
#define SSSH_TOKEN_BUFFER 10
#define SSSH_TOKEN_DELIM " \t\r\n\a"
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

		args = sssh_tokenize_line(line);
		status = sssh_execute(args); // non-zero on exit

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

/**
 * Runs a command via execpv and fork().
*/ 
int sssh_command(char** args) {
	int status;
	pid_t pid;

	pid = fork();
	if(pid == 0) {
		// Child
		if(execvp(args[0], args)) {
			perror("sssh");
		}
		exit(1);
	}
	else if (pid < 0) {
		perror("sssh");
	}
	else {
		// Parent; Wait for child process
		do {
		waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 0;
}

int sssh_execute(char** args) {
	int i; 

	// Empty command
	if(args[0] == NULL)
		return 0;

	// Check for builtins.
	for(i = 0; i < sssh_num_builtins(); i++) {
		if(strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}

	sssh_command(args);
	return 0;
}

int sssh_num_builtins() {
	return sizeof(builtin_str) / sizeof(char*);
}

int sssh_cd(char** args) {
	if(args[1] == NULL) {
		fprintf(stderr, "sssh: Expected path to \"cd\"");
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
		printf("\t%s\n", builtin_str[i]);
	}

	printf("Use \"man\" for information on any other programs.\n");

	return 0;
}

int sssh_exit(char** args) {
	return 1;
}
