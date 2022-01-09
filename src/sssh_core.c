#ifndef SSSH_CORE
#define SSSH_CORE

#include "sssh_declare.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

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
		if(strcmp(args[0], sssh_builtin_str[i]) == 0) {
			return (*sssh_builtin_func[i])(args);
		}
	}

	sssh_command(args);
	return 0;
}












#endif
