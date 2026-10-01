#ifndef UI_H
#define UI_H

void clear_input(void);
int read_int(const char *prompt, int min, int max);
void pause_enter(void);
void print_header(const char *title);
void print_line(void);
void print_topic_menu(void);
void print_main_menu(void);

#endif
