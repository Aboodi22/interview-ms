#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ui.h"

void clear_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int read_int(const char *prompt, int min, int max)
{
    char line[128];
    char *end;
    long value;
    for (;;) {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\nInput ended. Exiting.\n");
            exit(0);
        }
        errno = 0;
        end = NULL;
        value = strtol(line, &end, 10);
        while (end && (*end == ' ' || *end == '\t')) end++;
        if (errno == 0 && end != line && end && (*end == '\n' || *end == '\0') &&
            value >= min && value <= max)
            return (int)value;
        printf("Please enter a number from %d to %d.\n", min, max);
    }
}

void pause_enter(void)
{
    char line[8];
    printf("\nPress ENTER to continue...");
    fgets(line, sizeof(line), stdin);
}

void print_line(void)
{
    printf("------------------------------------------------------------\n");
}

void print_header(const char *title)
{
    printf("\n============================================================\n");
    printf("%s\n", title);
    printf("============================================================\n");
}

void print_main_menu(void)
{
    print_header("INTERVIEW MASTER - C CLI");
    printf("1. Start Interview\n");
    printf("2. 15-Day Learning Plan\n");
    printf("3. Skill Snapshot\n");
    printf("4. My Progress\n");
    printf("5. Review Mistakes\n");
    printf("6. Exit\n");
}

void print_topic_menu(void)
{
    print_header("TOPICS");
    printf("Topics are selected automatically in Interview Mode.\n");
}
