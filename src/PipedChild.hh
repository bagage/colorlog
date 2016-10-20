#pragma once

#include <stdlib.h>

class PipedChild {
    public:
        int stdout;
        int pid;

        PipedChild(const char* szCommand, int aArgc, char** const aArguments);
        ~PipedChild();

    private:
        char *get_interpreter(const char *command, char *interpreter, size_t buff_size);
        void exec_process(const char* cmd, char** args);
        void report_exec_error(int err, const char *cmd, char **argv);

      int the_pipe[2];
};
