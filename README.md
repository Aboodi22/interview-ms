# Interview Master — C CLI Final

A local terminal interview trainer built from the supplied Interview Master question set.

## Included

- 510 interview questions
- 510 concept definitions
- Answer + explanation for every question record
- Difficulty: Beginner / Intermediate / Advanced
- Question type: Conceptual / Comparison / Scenario / Code/Algorithm
- Topic filters
- Mixed practice
- Adaptive question selection
- Mastery from 0–100 per question
- Correct/incorrect streaks
- Mistake review
- Session score and accuracy
- 15-day learning plan
- Persistent progress in `save/progress.dat`
- Built-in data/self tests
- No web server, database, Node.js, Python, or external runtime required

## Build

```bash
make
```

## Test the installation

```bash
make test
```

or:

```bash
./interview-master --self-test
```

Expected final line:

```text
ALL SELF-TESTS PASSED
```

## Run

```bash
make run
```

or:

```bash
./interview-master
```

Run it from the project root because the program reads `data/questions.tsv` and `data/plan.txt`.

## User workflow

1. Start the program.
2. Choose `Start Practice`.
3. Choose a topic or Mixed Practice.
4. Choose Beginner, Intermediate, Advanced, or Adaptive.
5. Choose the number of questions.
6. Read the question and answer it mentally or on paper.
7. Press ENTER to reveal the reference answer and explanation.
8. Mark yourself Correct, Incorrect, or Partial.
9. The mastery score and mistake history are updated automatically.
10. At the end, the program displays session accuracy.
11. `My Progress` shows global and per-topic mastery.
12. `Review Mistakes` lists questions answered incorrectly in previous sessions.

## Adaptive engine

The selector prioritizes:

- never-seen questions
- low mastery
- repeated mistakes
- recent learning difficulty
- the selected difficulty or the current adaptive target

Correct answers increase mastery. Incorrect answers reduce mastery and reset the streak. Partial answers receive a smaller mastery reduction.

This is intentionally deterministic and transparent rather than an opaque ML model, so the rules can be inspected and extended later.

## Content verification

The question source is the supplied Interview Master material. The answer/explanation layer is original study content and carries a verification-source family per topic. Canonical verification families are listed in `data/SOURCES.txt`.

Important: a verification-source family means the topic is anchored to authoritative documentation. It does not mean copyrighted answers were copied from those sites.

## Repository workflow

```bash
git init
git add .
git commit -m "Build Interview Master C CLI final"
git branch -M main
git remote add origin YOUR_GITHUB_REPOSITORY
 git push -u origin main
```

A user can then clone it with Git, enter the folder, run `make`, then `./interview-master`.

## Data format

`data/questions.tsv` is intentionally human-readable. It can be extended without changing the C source as long as the nine columns remain present.

`data/concepts.tsv` contains the 510 concept definitions from the supplied source.

## License / ownership

All rights reserved to Abdel Bob.


## Stress-free interview mode

The main interview does not ask the user to choose a topic, difficulty, or number of questions. Topics are mixed automatically. Difficulty is an internal state: a strong answer moves the next selection upward, a partial answer holds the level for reinforcement, and a wrong answer moves the selection downward. Internal question IDs and difficulty labels are not displayed during the interview. The session continues until the user chooses to stop.

The content format is intentionally scalable to 10,000+ records. The current repository contains the verified seed dataset; future content expansion should add new records to `data/questions.tsv` without changing the interview engine.
