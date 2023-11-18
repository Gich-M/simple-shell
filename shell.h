#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>


#define READ_BUFFER 1024
#define WRITE_BUFFER 1024
#define FLUSH_BUFFER -1

#define NORMAL_COMMAND 0
#define OR_COMMAND 1
#define AND_COMMAND 2
#define CHAIN_COMMAND 3

#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

#define USE_CUSTOM_READ 0
#define USE_CUSTOM_TOK 0

#define HISTORY_FILE ".my_shell_history"
#define HISTORY_MAX 1024

extern char **environ;


/**
 * struct StringList - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */

typedef struct StringList
{
	int num;
	char *str;
	struct StringList *next;
} StringList;

/**
 * struct CommandInfo - encapsulates parameters for command execution,
 *                      facilitating a consistent function pointer prototype
 * @arg:            string generated from getline containing arguments
 * @argv:           array of strings generated from arg
 * @path:           string path for the current command
 * @argc:           argument count
 * @line_count:     error count
 * @err_num:        error code for exit() calls
 * @linecount_flag: flag to count this line of input
 * @fname:          program filename
 * @env:            linked list local copy of environ
 * @history:        history node
 * @alias:          alias node
 * @environ:        custom modified copy of environ from the linked list env
 * @env_altered:    flag indicating if environ was changed
 * @status:         return status of the last exec'd command
 * @cmd_buf:        address of pointer to cmd_buf, set if chaining commands
 * @cmd_buf_type:   CMD_type indicating command chaining (||, &&, ;)
 * @input_fd:       file descriptor from which to read line input
 * @histcount:      history line number count
 */

typedef struct CommandInfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;

	StringList *env;
	StringList *history;
	StringList *alias;

	char **environ;
	int env_altered;
	int status;
	char **cmd_buf;
	int cmd_buf_type;
	int input_fd;
	int histcount;
} CommandInfo;

#define COMMAND_INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}
/**
 *struct BuiltinCommand - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */

typedef struct BuiltinCommand
{
	char *type;
	int (*func)(CommandInfo *);
} BuiltinCommandTable;

/* Function prototypes */

/* shell_loop.c */
int hsh(CommandInfo *, char **);
int findBuiltin(CommandInfo *);
void findCmd(CommandInfo *);
void forkCmd(CommandInfo *);
void handleWaitStatus(CommandInfo *, int);
/* command_parser.c */
int isCmd(CommandInfo *info, char *);
char *dupChars(char *pathstr, int, int);
char *findPath(CommandInfo *info, char *, char *);

/* main.c */
int main(int ac, char **av);

/* error_handling.c */
void printError(char *);
int writeErrorChar(char);
int writeCharToFD(char c, int fd);
int printStringToFD(char *str, int fd);

/* custom_string.c */
int _strlen(const char *);
int _strcmp(const char *, const char *);
char *startsWith(const char *, const char *);
char *_strcat(char *, const char *);

/* string_operations.c */
char *_strcpy(char *, const char *);
char *_strdup(const char *);
void _puts(const char *);
int _putchar(char);

/* exits_handling.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);
/* command_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* memory_management.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* memory_operations.c */
int bfree(void **);

/* cust_atoi.c */
int isInteractive(CommandInfo *);
int isDelim(char, char *);
int isAlpha(int);
int _atoi(char *);

/* errors_handling_1.c */
int printErrorString(char *);
void printErrorInfo(CommandInfo *, char *);
int printDecimal(int, int);
char *convertNumber(long int, int, int);
void removeComments(char *);

/* builtin_commands.c */
int exitShell(CommandInfo *);
int changeDir(CommandInfo *);
int myHelp(CommandInfo *);

/* shell_builtins.c */
int displayHistory(CommandInfo *);
int unsetAlias(CommandInfo *, char *);
int setAlias(CommandInfo *, char *);
int printAlias(StringList *);
int myAlias(CommandInfo *);


/* getLine.c */
ssize_t readBuffer(CommandInfo *, char *, size_t *);
int getLine(CommandInfo *, char **, size_t *);
ssize_t bufferInput(CommandInfo *, char **, size_t *);
ssize_t getInput(CommandInfo *);
void sigintHandler(int);

/* getInfo.c */
void clearInfo(CommandInfo *);
void setInfo(CommandInfo *, char **);
void freeInfo(CommandInfo *, int);

/* environ.c */
int myEnv(CommandInfo *);
char *getEnvVar(CommandInfo *, const char *);
int setEnvVar(CommandInfo *);
int unsetEnvVar(CommandInfo *);
int populateEnvList(CommandInfo *);


/* getEnv.c */
char **getEnviron(CommandInfo *);
int unsetEnv(CommandInfo *, char *);
int setEnv(CommandInfo *, char *, char *);

/* command_history.c */
char *getHistoryFile(CommandInfo *info);
int writeHistory(CommandInfo *info);
int readHistory(CommandInfo *info);
int buildHistoryList(CommandInfo *info, char *buffer, int linecount);
int renumberHistory(CommandInfo *info);

/* linked_lists.c */
StringList *addNode(StringList **, const char *, int);
StringList *addNodeEnd(StringList **, const char *, int);
size_t printListStr(const StringList *);
int deleteNodeAtIndex(StringList **, unsigned int);
void freeList(StringList **);

/* linked_list_operations.c */
size_t listLen(const StringList *);
char **listToStrings(StringList *);
size_t printList(const StringList *);
StringList *nodeStartsWith(StringList *, char *, char);
ssize_t getNodeIndex(StringList *, StringList *);

/* var_handling.c */
int isChainDelimiter(CommandInfo *, char *, size_t *);
void checkChain(CommandInfo *, char *, size_t *, size_t, size_t);
int replaceAlias(CommandInfo *);
int replaceVars(CommandInfo *);
int replaceString(char **, char *);

#endif
