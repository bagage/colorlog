#include "PipedChild.hh"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include <errno.h>
#include <fcntl.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

PipedChild::PipedChild(const char* szCommand, int aArgc, char** const aArguments) : stdout(-1) {
    if (!szCommand) return;

    //TODO: szCommand should not be colorlog itself - compare its value with /proc/self/exe
    // if szCommand is not a fullpath, find the executable in PATH
    char* cmd = realpath(szCommand, NULL);
    bool is_relative = (strchr(szCommand, '/') != NULL);
    if (!is_relative && (access( szCommand, F_OK ) == -1)) {
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

        // exec process, but redirect STDERR to STDOUT to avoid dealing with 2 different pipes needlessly
        int malloc_size = aArgc + 2;
        char** args = (char **) malloc((malloc_size) * sizeof (char *));
        int i;
        for (i = 0; i < aArgc; i++) {
            args[i] = strdup(aArguments[i]);
        }
        args[i] = strdup("2>&1");
        args[i] = NULL;

        exec_process(cmd, args);

        for (i = 0; i < aArgc; i++) {
            free(args[i]);
        }
        free(args);

        // reaching that mean the children stopped/crashed
        report_exec_error(errno, cmd, args);
        exit(1);
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

void PipedChild::exec_process(const char* cmd, char** args) {
    execve(cmd, args, environ);

    // borrow from Fish shell
    int err = errno;

    // Something went wrong with execve, check for a ":", and run /bin/sh if encountered. This is a
    // weird predecessor to the shebang that is still sometimes used since it is supported on
    // Windows. OK to not use CLO_EXEC here because this is called after fork and the file is
    // immediately closed.
    int fd = open(cmd, O_RDONLY);
    if (fd >= 0) {
        char begin[1] = {0};
        ssize_t amt_read = read(fd, begin, 1);
        close(fd);

        if ((amt_read == 1) && (begin[0] == ':')) {
            // Relaunch it with /bin/sh. Don't allocate memory, so if you have more args than this,
            // update your silly script! Maybe this should be changed to be based on ARG_MAX
            // somehow.
            char sh_command[] = "/bin/sh";
            char *argv2[128];
            argv2[0] = sh_command;
            for (size_t i = 1; i < sizeof argv2 / sizeof *argv2; i++) {
                argv2[i] = args[i - 1];
                if (argv2[i] == NULL) break;
            }

            execve(sh_command, argv2, environ);
        }
    }

    errno = err;
}

void PipedChild::report_exec_error(int err, const char *cmd, char **argv) {
    fprintf(stderr,"Failed to execute process '%s'. Reason:", cmd);

    switch (err) {
        case E2BIG: {
            size_t sz = 0;
            const char *const *p;
            for (p = argv; *p; p++) {
                sz += strlen(*p) + 1;
            }

            fprintf(stderr,                           "The total size of the argument and environment lists exceeds the "
                       "operating system limit.");

            fprintf(stderr,"Try running the command again with fewer arguments.");
            break;
        }

        case ENOEXEC: {
            const char *err = strerror(errno);
            fprintf(stderr,"exec: %s", err);

            fprintf(stderr,                       "The file '%s' is marked as an executable but could not be run by the "
                       "operating system.",
                       cmd);
            break;
        }

        case ENOENT: {
            // ENOENT is returned by exec() when the path fails, but also returned by posix_spawn if
            // an open file action fails. These cases appear to be impossible to distinguish. We
            // address this by not using posix_spawn for file redirections, so all the ENOENTs we
            // find must be errors from exec().
            char interpreter_buff[128] = {}, *interpreter;
            interpreter = get_interpreter(cmd, interpreter_buff, sizeof interpreter_buff);
            if (interpreter && 0 != access(interpreter, X_OK)) {
                fprintf(stderr,                           "The file '%s' specified the interpreter '%s', which is not an "
                           "executable command.",
                           cmd, interpreter);
            } else {
                fprintf(stderr,"The file '%s' does not exist or could not be executed.", cmd);
            }
            break;
        }

        case ENOMEM: {
            fprintf(stderr,"Out of memory");
            break;
        }

        default: {
            const char *err = strerror(errno);
            fprintf(stderr,"exec: %s", err);

            // debug(0, L"The file '%ls' is marked as an executable but could not be run by the
            // operating system.", p->cmd);
            break;
        }
    }
    fprintf(stderr, "\n");
}


/// Returns the interpreter for the specified script. Returns NULL if file is not a script with a
/// shebang.
char *PipedChild::get_interpreter(const char *command, char *interpreter, size_t buff_size) {
    // OK to not use CLO_EXEC here because this is only called after fork.
    int fd = open(command, O_RDONLY);
    if (fd >= 0) {
        size_t idx = 0;
        while (idx + 1 < buff_size) {
            char ch;
            ssize_t amt = read(fd, &ch, sizeof ch);
            if (amt <= 0) break;
            if (ch == '\n') break;
            interpreter[idx++] = ch;
        }
        interpreter[idx++] = '\0';
        close(fd);
    }

    if (strncmp(interpreter, "#! /", 4) == 0) {
        return interpreter + 3;
    } else if (strncmp(interpreter, "#!/", 3) == 0) {
        return interpreter + 2;
    }

    return NULL;
}

PipedChild::~PipedChild() {
    close(the_pipe[PIPE_READ]);
}
