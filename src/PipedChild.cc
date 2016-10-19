#include "PipedChild.hh"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

PipedChild::PipedChild(const char* szCommand, int aArgc, char** const aArguments) : stdout(-1) {
    if (!szCommand) return;

    // exec process, but redirect STDERR to STDOUT to avoid dealing with 2 different pipes needlessly
    int malloc_size = aArgc + 2;
    char** args = (char **) malloc((malloc_size) * sizeof (char *));
    int i = 0;
    for (; i < aArgc; i++) {
        args[i] = strdup(aArguments[i]);
    }
    args[i] = strdup("2>&1");
    args[i] = NULL;

    //TODO: szCommand should not be colorlog itself - compare its value with /proc/self/exe
    // if szCommand is not a fullpath, find the executable in PATH
    char* cmd = realpath(szCommand, NULL);
    if (!cmd || access( szCommand, F_OK ) == -1) {
        char* paths = secure_getenv("PATH");
        char* it = paths;
        char* n = paths;
        char buf[PATH_MAX];
        do {
            memset(buf, 0, PATH_MAX);
            n = strchr(it, ':');
            memcpy(buf, it, n ? n - it : strlen(it));
            strcat(buf, "/");
            strcat(buf, szCommand);
            if (access( buf, F_OK ) != -1) {
                cmd = strdup(buf);
                break;
            }
            it = n ? n + 1 : NULL;
        } while (n);
    }
    if (!cmd) {
        perror("cannot find your executable");
        return;
    }

    if (pipe(the_pipe) < 0) {
        perror("allocating pipe for child output redirect");
    }

    pid = fork();
    if (0 == pid) {
        // child continues here

        // redirect stdout
        if (dup2(the_pipe[PIPE_WRITE], STDOUT_FILENO) == -1) {
            perror("redirecting stdout");
        }
        // redirect stderr
        if (dup2(the_pipe[PIPE_WRITE], STDOUT_FILENO) == -1) {
            perror("redirecting stderr");
        }

        // all these are for use by parent only
        close(the_pipe[PIPE_READ]);
        // close(the_pipe[PIPE_WRITE]);

        int nResult = execve(cmd, args, environ);

        // if we get here at all, an error occurred, but we are in the child
        // process, so just exit
        perror("exec of the child process");
        exit(nResult);
    } else if (pid > 0) {
        // parent continues here

        // close unused file descriptors, these are for child only
        close(the_pipe[PIPE_WRITE]);

        // save child stdout and we are done
        stdout = the_pipe[PIPE_READ];
    } else {
        // failed to create child
        close(the_pipe[PIPE_READ]);
        close(the_pipe[PIPE_WRITE]);
    }
}

PipedChild::~PipedChild() {
    close(the_pipe[PIPE_READ]);
}
