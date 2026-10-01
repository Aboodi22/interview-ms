#ifndef PROGRESS_H
#define PROGRESS_H

#include "question.h"

#define MAX_PROGRESS 10000

typedef struct {
    int attempts;
    int correct;
    int incorrect;
    int streak;
    int mastery;
    long last_seen;
} Progress;

void progress_init(Progress *p);
int progress_load(const char *path, Progress *p, int count);
int progress_save(const char *path, const Progress *p, int count);
void progress_record(Progress *p, int correct);
void show_progress(const Question *q, const Progress *p, int count);
void review_mistakes(const Question *q, const Progress *p, int count);
int selection_weight(const Question *q, const Progress *p, int adaptive_target);

#endif
