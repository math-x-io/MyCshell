#include <stdbool.h>
#include <stdio.h>
#include "shell.h"

int main(int argc, char** argv)
{
    struct Shell shell;
    system("curl -s -L 'https://raw.githubusercontent.com/shinya/pokemon-terminal-art/main/hello.sh' | bash");
    printf("Hello To MyShell :D \n");
    shell_init(& shell);
    shell_run(& shell);
    shell_free(& shell);
    return 0;
}