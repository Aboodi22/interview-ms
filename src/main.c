#include "app.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "--self-test")) return app_self_test();
    if (argc > 1 && !strcmp(argv[1], "--version")) {
        printf("Interview Master C CLI 2.0 - 510-question adaptive interview trainer\n");
        return 0;
    }
    return app_run();
}
