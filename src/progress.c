#include <stdio.h>
#include <string.h>
#include <time.h>
#include "progress.h"
#include "ui.h"

void progress_init(Progress *p)
{
    memset(p, 0, sizeof(Progress) * MAX_PROGRESS);
}

int progress_load(const char *path, Progress *p, int count)
{
    FILE *fp = fopen(path, "rb");
    size_t got;
    if (!fp) return 0;
    got = fread(p, sizeof(Progress), (size_t)count, fp);
    fclose(fp);
    return got == (size_t)count;
}

int progress_save(const char *path, const Progress *p, int count)
{
    FILE *fp = fopen(path, "wb");
    size_t wrote;
    if (!fp) return 0;
    wrote = fwrite(p, sizeof(Progress), (size_t)count, fp);
    fclose(fp);
    return wrote == (size_t)count;
}

void progress_record(Progress *p, int correct)
{
    p->attempts++;
    p->last_seen = (long)time(NULL);
    if (correct) {
        p->correct++;
        p->streak++;
        p->mastery += 12 + (p->streak >= 3 ? 3 : 0);
        if (p->mastery > 100) p->mastery = 100;
    } else {
        p->incorrect++;
        p->streak = 0;
        p->mastery -= 18;
        if (p->mastery < 0) p->mastery = 0;
    }
}

void show_progress(const Question *q, const Progress *p, int count)
{
    int i, attempted = 0, correct = 0, mistakes = 0;
    (void)q;
    for (i = 0; i < count; i++) {
        attempted += p[i].attempts;
        correct += p[i].correct;
        mistakes += p[i].incorrect;
    }
    printf("Attempts         : %d\n", attempted);
    printf("Correct          : %d\n", correct);
    printf("Incorrect        : %d\n", mistakes);
    printf("Accuracy         : %s", attempted ? "" : "0%\n");
    if (attempted)
        printf("%.1f%%\n", (100.0 * correct) / attempted);
    printf("\nPer-topic mastery:\n");
    for (i = TOPIC_JAVA; i <= TOPIC_SYSTEM; i++) {
        int total = 0, sum = 0, j;
        for (j = 0; j < count; j++) {
            if (q[j].topic == (Topic)i) { sum += p[j].mastery; total++; }
        }
        printf("  %-35s %.1f/100\n", topic_name((Topic)i), total ? (double)sum / total : 0.0);
    }
}

void review_mistakes(const Question *q, const Progress *p, int count)
{
    int i, found = 0;
    print_line();
    printf("QUESTIONS TO REVIEW\n");
    print_line();
    for (i = 0; i < count; i++) {
        if (p[i].incorrect > 0) {
            printf("[%s] wrong=%d mastery=%d\n", topic_name(q[i].topic),
                   p[i].incorrect, p[i].mastery);
            printf("  %s\n", q[i].question);
            found = 1;
        }
    }
    if (!found) printf("No mistakes recorded yet.\n");
}

int selection_weight(const Question *q, const Progress *p, int adaptive_target)
{
    int w = 10 + (100 - p->mastery) + (p->incorrect * 15);
    if (p->attempts == 0) w += 70;
    if (q->difficulty == (Difficulty)adaptive_target) w += 25;
    if (p->mastery < 40 && q->difficulty == DIFF_BEGINNER) w += 25;
    if (p->mastery >= 70 && q->difficulty == DIFF_ADVANCED) w += 20;
    return w;
}
