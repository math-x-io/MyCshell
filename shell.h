#include <unistd.h>
#include <stdbool.h>
struct Shell {
    bool running;
    int line_number;
    char * buffer;
    size_t buffer_size;
};

void shell_init(struct Shell *s);
void shell_run(struct Shell *s);
void shell_free(struct Shell *s);

void shell_read_line(struct Shell *s);
void shell_execute_line(struct Shell *s);

