#ifndef QUESTION_H
#define QUESTION_H

#define MAX_QUESTION_TEXT 1024
#define MAX_QUESTIONS 10000
#define MAX_ANSWER_TEXT 2048
#define MAX_EXPLANATION_TEXT 3072
#define MAX_TAGS 512
#define MAX_SOURCE 512

typedef enum { TOPIC_JAVA, TOPIC_SPRING, TOPIC_JS, TOPIC_SQL, TOPIC_ALGO, TOPIC_WEB, TOPIC_SYSTEM, TOPIC_MIXED } Topic;
typedef enum { DIFF_BEGINNER, DIFF_INTERMEDIATE, DIFF_ADVANCED } Difficulty;
typedef enum { TYPE_CONCEPTUAL, TYPE_COMPARISON, TYPE_SCENARIO, TYPE_CODE } QuestionType;

typedef struct {
    int id;
    Topic topic;
    Difficulty difficulty;
    QuestionType type;
    char tags[MAX_TAGS];
    char question[MAX_QUESTION_TEXT];
    char answer[MAX_ANSWER_TEXT];
    char explanation[MAX_EXPLANATION_TEXT];
    char source[MAX_SOURCE];
} Question;

int load_questions(const char *path, Question *questions, int max_questions);
const char *topic_name(Topic topic);
const char *difficulty_name(Difficulty difficulty);
const char *type_name(QuestionType type);
int parse_topic(const char *s, Topic *out);
int parse_difficulty(const char *s, Difficulty *out);
int parse_type(const char *s, QuestionType *out);
void show_question(const Question *q, int reveal_answer);

#endif
