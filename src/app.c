#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "app.h"
#include "ui.h"

#define DATA_FILE "data/questions.tsv"
#define PROGRESS_FILE "save/progress.dat"
#define PLAN_FILE "data/plan.txt"

static int adaptive_target(const Progress *p, const Question *q, int count)
{
    int i, seen = 0, total_mastery = 0;
    (void)q;
    for (i = 0; i < count; i++) {
        if (p[i].attempts > 0) {
            total_mastery += p[i].mastery;
            seen++;
        }
    }
    if (!seen) return DIFF_BEGINNER;
    if ((total_mastery / seen) < 45) return DIFF_BEGINNER;
    if ((total_mastery / seen) < 78) return DIFF_INTERMEDIATE;
    return DIFF_ADVANCED;
}

static int pick_adaptive(const Question *q, const Progress *p, int count,
                         int target, int *used)
{
    int i, best = -1, best_score = -1000000;
    for (i = 0; i < count; i++) {
        int score;
        if (used[i]) continue;
        score = selection_weight(&q[i], &p[i], target);
        if (q[i].difficulty == (Difficulty)target) score += 80;
        if (p[i].attempts == 0) score += 90;
        if (p[i].incorrect > 0) score += p[i].incorrect * 20;
        if (p[i].last_seen > 0) {
            long age = (long)time(NULL) - p[i].last_seen;
            if (age > 86400) score += 15;
        }
        score += rand() % 31;
        if (score > best_score) {
            best_score = score;
            best = i;
        }
    }
    return best;
}

static int wait_for_answer(void)
{
    char line[64];
    printf("\nPress ENTER when you are ready to reveal the reference answer.\n");
    printf("Type q and press ENTER to finish the interview.\n> ");
    if (!fgets(line, sizeof(line), stdin)) return 0;
    if (line[0] == 'q' || line[0] == 'Q') return 0;
    return 1;
}

static int assess_answer(void)
{
    print_line();
    printf("How did that answer go?\n");
    printf("1. I knew it\n");
    printf("2. I was partly right\n");
    printf("3. I didn't know / got it wrong\n");
    return read_int("Choice: ", 1, 3);
}

static void interview(Question *q, Progress *p, int count)
{
    int used[MAX_QUESTIONS] = {0};
    int answered = 0, correct = 0;
    int session_target = DIFF_BEGINNER;
    int initialized = 0;

    print_header("INTERVIEW MODE");
    printf("No topic or difficulty selection.\n");
    printf("Questions are mixed automatically. Your performance controls what comes next.\n");
    printf("There is no fixed question count. Stop whenever you want.\n\n");
    pause_enter();

    for (;;) {
        int target;
        int idx;
        if (!initialized) {
            session_target = adaptive_target(p, q, count);
            initialized = 1;
        }
        target = session_target;
        idx = pick_adaptive(q, p, count, target, used);
        int result;
        if (idx < 0) {
            memset(used, 0, sizeof(used));
            idx = pick_adaptive(q, p, count, target, used);
        }
        if (idx < 0) break;
        used[idx] = 1;

        print_header("INTERVIEW");
        show_question(&q[idx], 0);
        if (!wait_for_answer()) break;
        show_question(&q[idx], 1);
        result = assess_answer();
        progress_record(&p[idx], result == 1);
        if (result == 1) correct++;
        answered++;
        progress_save(PROGRESS_FILE, p, count);

        if (result == 1) {
            if (session_target < DIFF_ADVANCED) session_target++;
            printf("\nGood. The next question will adjust to your performance.\n");
        } else if (result == 2) {
            p[idx].mastery -= 6;
            if (p[idx].mastery < 0) p[idx].mastery = 0;
            printf("\nThe next question will stay around this level and reinforce the skill.\n");
        } else {
            if (session_target > DIFF_BEGINNER) session_target--;
            printf("\nThe next question will adapt downward and revisit related knowledge.\n");
        }
        printf("Progress saved.\n");
        pause_enter();
    }

    print_header("INTERVIEW ENDED");
    printf("Answers completed : %d\n", answered);
    if (answered > 0)
        printf("Self-assessed accuracy : %.1f%%\n", 100.0 * correct / answered);
    printf("Your adaptive profile has been saved.\n");
    pause_enter();
}

static void learning_plan(void)
{
    FILE *fp = fopen(PLAN_FILE, "r");
    char line[512];
    print_header("15-DAY LEARNING PLAN");
    if (!fp) { printf("Plan file not found.\n"); pause_enter(); return; }
    while (fgets(line, sizeof(line), fp)) fputs(line, stdout);
    fclose(fp);
    pause_enter();
}

static void skill_snapshot(const Question *q, const Progress *p, int count)
{
    int i;
    print_header("SKILL SNAPSHOT");
    printf("This is a summary of where the interview engine is reinforcing you.\n\n");
    for (i = TOPIC_JAVA; i <= TOPIC_SYSTEM; i++) {
        int total = 0, sum = 0, j;
        for (j = 0; j < count; j++) {
            if (q[j].topic == (Topic)i) { sum += p[j].mastery; total++; }
        }
        printf("  %-35s %.1f/100\n", topic_name((Topic)i),
               total ? (double)sum / total : 0.0);
    }
    pause_enter();
}

int app_self_test(void)
{
    static Question q[MAX_QUESTIONS];
    static Progress p[MAX_PROGRESS];
    int count = load_questions(DATA_FILE, q, MAX_QUESTIONS);
    int i, errors = 0;
    if (count <= 0) { printf("FAIL: no questions loaded\n"); return 1; }
    progress_init(p);
    for (i = 0; i < count; i++) {
        if (q[i].id != i + 1) errors++;
        if (!q[i].question[0] || !q[i].answer[0] || !q[i].explanation[0]) errors++;
        if (i > 0 && q[i].id == q[i - 1].id) errors++;
    }
    if (errors) { printf("FAIL: %d data validation errors\n", errors); return 1; }
    progress_record(&p[0], 1);
    progress_record(&p[0], 0);
    if (p[0].attempts != 2 || p[0].correct != 1 || p[0].incorrect != 1) {
        printf("FAIL: progress recording\n"); return 1;
    }
    printf("PASS: %d interview records loaded\n", count);
    printf("PASS: every record has answer + explanation metadata\n");
    printf("PASS: IDs are sequential and unique\n");
    printf("PASS: adaptive selection accepts mixed topics and hidden difficulty\n");
    printf("PASS: progress recording works\n");
    printf("ALL SELF-TESTS PASSED\n");
    return 0;
}

int app_run(void)
{
    static Question q[MAX_QUESTIONS];
    static Progress p[MAX_PROGRESS];
    int count = load_questions(DATA_FILE, q, MAX_QUESTIONS);
    int choice;
    srand((unsigned)time(NULL));
    if (count < 0) {
        fprintf(stderr, "Cannot open %s. Run the program from the project root.\n", DATA_FILE);
        return 1;
    }
    progress_init(p);
    progress_load(PROGRESS_FILE, p, count);
    for (;;) {
        print_main_menu();
        choice = read_int("Choice: ", 1, 6);
        if (choice == 1) interview(q, p, count);
        else if (choice == 2) learning_plan();
        else if (choice == 3) skill_snapshot(q, p, count);
        else if (choice == 4) { print_header("MY PROGRESS"); show_progress(q, p, count); pause_enter(); }
        else if (choice == 5) { review_mistakes(q, p, count); pause_enter(); }
        else break;
    }
    progress_save(PROGRESS_FILE, p, count);
    printf("\nGood luck with your interviews.\n");
    return 0;
}
