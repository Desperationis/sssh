#ifndef SSSH_DECLARE
#define SSSH_DECLARE

// Constants
#define SSSH_RL_BUFFER 50
#define SSSH_TOKEN_BUFFER 10
#define SSSH_TOKEN_DELIM " \t\r\n\a"
#define SSSH_MEM_ERROR 1

// Globals
extern char* sssh_builtin_str[];
extern int (*sssh_builtin_func[])(char**);

// IO functions
char* sssh_cl_prefix();
char* sssh_readline();
char** sssh_tokenize_line(char*);

// Core functions
int sssh_command(char**);
int sssh_execute(char**);

// Builtins
int sssh_cd(char** args);
int sssh_help(char** args);
int sssh_exit(char** args);
int sssh_num_builtins();

#endif
