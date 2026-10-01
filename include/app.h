#ifndef APP_H
#define APP_H

#include "question.h"
#include "progress.h"

#define MAX_QUESTIONS 10000
#define SESSION_DEFAULT 10

int app_run(void);
int app_self_test(void);

#endif
