#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "question.h"
#include "ui.h"

static void trim_newline(char *s)
{
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r'))
        s[--n] = '\0';
}

static char *field(char **cursor)
{
    char *start = *cursor;
    char *tab = strchr(start, '\t');
    if (tab) {
        *tab = '\0';
        *cursor = tab + 1;
    } else {
        *cursor = start + strlen(start);
    }
    return start;
}

int parse_topic(const char *s, Topic *out)
{
    if (!strcmp(s, "JAVA")) *out = TOPIC_JAVA;
    else if (!strcmp(s, "SPRING")) *out = TOPIC_SPRING;
    else if (!strcmp(s, "JAVASCRIPT")) *out = TOPIC_JS;
    else if (!strcmp(s, "SQL")) *out = TOPIC_SQL;
    else if (!strcmp(s, "ALGORITHMS")) *out = TOPIC_ALGO;
    else if (!strcmp(s, "WEB")) *out = TOPIC_WEB;
    else if (!strcmp(s, "SYSTEM_SECURITY")) *out = TOPIC_SYSTEM;
    else return 0;
    return 1;
}

int parse_difficulty(const char *s, Difficulty *out)
{
    if (!strcmp(s, "BEGINNER")) *out = DIFF_BEGINNER;
    else if (!strcmp(s, "INTERMEDIATE")) *out = DIFF_INTERMEDIATE;
    else if (!strcmp(s, "ADVANCED")) *out = DIFF_ADVANCED;
    else return 0;
    return 1;
}

int parse_type(const char *s, QuestionType *out)
{
    if (!strcmp(s, "CONCEPTUAL")) *out = TYPE_CONCEPTUAL;
    else if (!strcmp(s, "COMPARISON")) *out = TYPE_COMPARISON;
    else if (!strcmp(s, "SCENARIO")) *out = TYPE_SCENARIO;
    else if (!strcmp(s, "CODE")) *out = TYPE_CODE;
    else return 0;
    return 1;
}

const char *topic_name(Topic topic)
{
    switch (topic) {
        case TOPIC_JAVA: return "Core Java";
        case TOPIC_SPRING: return "Spring Boot & Backend";
        case TOPIC_JS: return "JavaScript & React";
        case TOPIC_SQL: return "Databases & SQL";
        case TOPIC_ALGO: return "Algorithms & Data Structures";
        case TOPIC_WEB: return "Web, HTTP, Networking & DevOps";
        case TOPIC_SYSTEM: return "System Design, Clean Code & Security";
        default: return "Mixed";
    }
}

const char *difficulty_name(Difficulty difficulty)
{
    if (difficulty == DIFF_BEGINNER) return "Beginner";
    if (difficulty == DIFF_INTERMEDIATE) return "Intermediate";
    return "Advanced";
}

const char *type_name(QuestionType type)
{
    if (type == TYPE_CONCEPTUAL) return "Conceptual";
    if (type == TYPE_COMPARISON) return "Comparison";
    if (type == TYPE_SCENARIO) return "Scenario";
    return "Code / Algorithm";
}

int load_questions(const char *path, Question *questions, int max_questions)
{
    FILE *fp = fopen(path, "r");
    char line[9000];
    int count = 0;
    if (!fp) return -1;
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return -1;
    }
    while (count < max_questions && fgets(line, sizeof(line), fp)) {
        char *p = line;
        char *f[9];
        int i;
        trim_newline(line);
        for (i = 0; i < 9; i++) {
            f[i] = field(&p);
            if (i < 8 && *f[i] == '\0') break;
        }
        if (i != 9) continue;
        if (!parse_topic(f[1], &questions[count].topic) ||
            !parse_difficulty(f[2], &questions[count].difficulty) ||
            !parse_type(f[3], &questions[count].type))
            continue;
        questions[count].id = atoi(f[0]);
        snprintf(questions[count].tags, sizeof(questions[count].tags), "%s", f[4]);
        snprintf(questions[count].question, sizeof(questions[count].question), "%s", f[5]);
        snprintf(questions[count].answer, sizeof(questions[count].answer), "%s", f[6]);
        snprintf(questions[count].explanation, sizeof(questions[count].explanation), "%s", f[7]);
        snprintf(questions[count].source, sizeof(questions[count].source), "%s", f[8]);
        count++;
    }
    fclose(fp);
    return count;
}

void show_question(const Question *q, int reveal_answer)
{
    print_line();
    printf("INTERVIEW QUESTION\n");
    print_line();
    printf("%s\n", q->question);
    if (reveal_answer) {
        printf("\nREFERENCE ANSWER\n%s\n", q->answer);
        printf("\nWHY\n%s\n", q->explanation);
        printf("\nVerification basis: %s\n", q->source);
    }
}
