#pragma once

class PipedChild {
    public:
        int stdout;
        int pid;

        PipedChild(const char* szCommand, int aArgc, char** const aArguments);
        ~PipedChild();

    private:
      int the_pipe[2];
};