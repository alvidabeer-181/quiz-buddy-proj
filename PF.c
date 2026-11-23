/* Study Buddy - Programming Fundamentals final project.
 * Standard C11; no external libraries. See README.txt for usage. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#define MAX_PROFILES 100
#define NAME_LEN 64
#define QUIZ_SIZE 5
#define SUBJECTS 3
#define TOPICS 3

typedef struct { int id; char name[NAME_LEN]; } Profile;
typedef struct {
    int subject, topic;
    const char *text;
    const char *options[4];
    int answer;
    const char *explanation;
} Question;
typedef struct {
    int profile, subject, correct, total;
    long long timestamp;
    int topic_correct[TOPICS], topic_total[TOPICS];
} Result;

static const char *subjects[] = {"Programming Fundamentals", "Mathematics", "Computer Fundamentals"};
static const char *topics[SUBJECTS][TOPICS] = {
    {"Variables and operators", "Control flow", "Arrays and functions"},
    {"Arithmetic", "Algebra", "Geometry"},
    {"Hardware", "Software", "Data representation"}
};
static const Question bank[] = {
 {0,0,"Which format specifier prints an int?",{"%f","%d","%c","%s"},1,"%d prints a signed decimal integer."},
 {0,0,"What is the value of 7 / 2 using integer operands?",{"3.5","4","3","2"},2,"Integer division discards the fractional part."},
 {0,0,"Which operator finds the remainder?",{"/","%","*","&"},1,"The % operator calculates the integer remainder."},
 {0,1,"Which loop executes its body at least once?",{"for","while","do-while","None"},2,"A do-while checks its condition after the body."},
 {0,1,"What does break do inside a loop?",{"Restarts it","Exits the nearest loop","Exits the program","Skips only one statement"},1,"break exits the nearest loop or switch."},
 {0,1,"Which operator tests equality?",{"=","!=","==","<="},2,"== compares values; = assigns a value."},
 {0,2,"What is the first valid index of an array in C?",{"0","1","-1","Depends on its size"},0,"C arrays start at index zero."},
 {0,2,"Which keyword returns a value from a function?",{"break","return","continue","goto"},1,"return ends a function and can provide its result."},
 {0,2,"How many elements does int a[5] contain?",{"4","6","5","10"},2,"The declaration reserves five int elements."},
 {1,0,"What is 15 percent of 200?",{"15","20","30","35"},2,"200 multiplied by 0.15 is 30."},
 {1,0,"What is 8 + 3 * 4?",{"44","20","32","24"},1,"Multiplication is evaluated before addition."},
 {1,0,"What is 3/4 expressed as a decimal?",{"0.25","0.34","0.5","0.75"},3,"Three divided by four is 0.75."},
 {1,1,"Solve 2x + 6 = 14.",{"2","3","4","5"},2,"Subtract six, then divide both sides by two."},
 {1,1,"What is x squared when x = -3?",{"-9","9","6","-6"},1,"(-3) multiplied by (-3) is 9."},
 {1,1,"Simplify 3a + 2a.",{"5a","6a","5a squared","a"},0,"Add the coefficients of like terms."},
 {1,2,"What is the sum of angles in a triangle?",{"90 degrees","180 degrees","270 degrees","360 degrees"},1,"The interior angles of a Euclidean triangle sum to 180 degrees."},
 {1,2,"What is the area of a rectangle of length 6 and width 4?",{"10","20","24","12"},2,"Area is length multiplied by width."},
 {1,2,"A square has side length 5. What is its perimeter?",{"10","15","20","25"},2,"Perimeter is four times the side length."},
 {2,0,"Which component executes program instructions?",{"Monitor","CPU","Keyboard","Printer"},1,"The CPU executes program instructions."},
 {2,0,"Which memory normally loses its contents when power is off?",{"SSD","ROM","RAM","Hard disk"},2,"RAM is volatile memory."},
 {2,0,"Which is an input device?",{"Monitor","Speaker","Printer","Keyboard"},3,"A keyboard sends input to the computer."},
 {2,1,"Which is an operating system?",{"Windows","HTML","CPU","USB"},0,"Windows is an operating system."},
 {2,1,"What does a compiler do?",{"Stores electricity","Translates source code","Displays only images","Repairs hardware"},1,"A compiler translates source code into another form, such as machine code."},
 {2,1,"Which is application software?",{"RAM","CPU","Word processor","Motherboard"},2,"A word processor performs a user task."},
 {2,2,"How many bits are in one byte?",{"2","4","8","16"},2,"One byte contains eight bits."},
 {2,2,"What is binary 101 in decimal?",{"3","5","6","7"},1,"101 base two equals 4 + 0 + 1 = 5."},
 {2,2,"Which digits are used in binary?",{"0 and 1","1 and 2","0 through 7","0 through 9"},0,"Binary uses only zero and one."}
};

/* Read a complete bounded line. Drain oversized input so menus stay aligned. */
static int read_line(char *buffer, size_t size) {
    size_t n; int c;
    if (!fgets(buffer, (int)size, stdin)) return 0;
    n = strlen(buffer);
    if (n && buffer[n-1] == '\n') buffer[--n] = '\0';
    else if (!feof(stdin)) {
        c = getchar();
        if (c != '\n' && c != EOF) {
            while ((c = getchar()) != '\n' && c != EOF) {}
            puts("Input too long. Please try again."); return -1;
        }
    }
    if (n && buffer[n-1] == '\r') buffer[n-1] = '\0';
    return 1;
}

static int number(const char *prompt, int low, int high) {
    char line[128], *end; long value; int status;
    for (;;) {
        printf("%s", prompt); fflush(stdout);
        status = read_line(line, sizeof line);
        if (!status) return -1;
        if (status < 0) continue;
        errno = 0; value = strtol(line, &end, 10);
        if (end != line) {
            while (isspace((unsigned char)*end)) end++;
            if (!errno && !*end && value >= low && value <= high) return (int)value;
        }
        printf("Enter a whole number from %d to %d.\n", low, high);
    }
}

static int load_profiles(Profile *p) {
    FILE *f = fopen("profiles.txt", "r");
    char line[256], name[NAME_LEN], extra; int id, count = 0, i, duplicate;
    if (!f) { if (errno != ENOENT) perror("Cannot read profiles.txt"); return 0; }
    while (fgets(line, sizeof line, f)) {
        if (sscanf(line, "%d|%63[^\r\n]%c", &id, name, &extra) < 2 || id < 1 || strchr(name, '|')) continue;
        duplicate = 0;
        for (i = 0; i < count; i++) if (p[i].id == id) duplicate = 1;
        if (!duplicate && count < MAX_PROFILES) { p[count].id = id; strcpy(p[count++].name, name); }
    }
    if (ferror(f)) perror("Error reading profiles");
    fclose(f); return count;
}

static int create_profile(Profile *p, int *count) {
    char name[NAME_LEN]; int status, i, id = 1; size_t start, end;
    FILE *f;
    if (*count >= MAX_PROFILES) { puts("Profile limit reached."); return -1; }
    for (;;) {
        printf("Student name (1-63 characters, no |): "); fflush(stdout);
        status = read_line(name, sizeof name);
        if (!status) return -1;
        if (status < 0) continue;
        start = 0; end = strlen(name);
        while (start < end && isspace((unsigned char)name[start])) start++;
        while (end > start && isspace((unsigned char)name[end-1])) end--;
        name[end] = '\0'; memmove(name, name + start, end - start + 1);
        if (*name && !strchr(name, '|')) break;
        puts("Please enter a nonempty name without |.");
    }
    /* Choose an unused positive ID without relying on file order. */
    for (i = 0; i < *count; i++) if (p[i].id == id) { id++; i = -1; }
    f = fopen("profiles.txt", "a");
    if (!f) { perror("Cannot save profile"); return -1; }
    status = fprintf(f, "%d|%s\n", id, name) >= 0;
    if (fclose(f) != 0) status = 0;
    if (!status) { puts("Profile could not be saved."); return -1; }
    p[*count].id = id; strcpy(p[*count].name, name);
    (*count)++; puts("Profile saved."); return *count - 1;
}

static int save_result(const Result *r) {
    FILE *f = fopen("results.txt", "a"); int ok, t;
    if (!f) { perror("Cannot save result"); return 0; }
    ok = fprintf(f, "%d %d %d %d %lld", r->profile, r->subject, r->correct, r->total, r->timestamp) >= 0;
    for (t = 0; t < TOPICS; t++)
        if (fprintf(f, " %d %d", r->topic_correct[t], r->topic_total[t]) < 0) ok = 0;
    if (fputc('\n', f) == EOF) ok = 0;
    if (fclose(f) != 0) ok = 0;
    if (!ok) puts("Result could not be saved. Check available disk space.");
    return ok;
}

static int parse_result(const char *line, Result *r) {
    char extra; int t, correct = 0, total = 0;
    if (sscanf(line, "%d %d %d %d %lld %d %d %d %d %d %d %c",
        &r->profile, &r->subject, &r->correct, &r->total, &r->timestamp,
        &r->topic_correct[0], &r->topic_total[0], &r->topic_correct[1],
        &r->topic_total[1], &r->topic_correct[2], &r->topic_total[2], &extra) != 11) return 0;
    if (r->profile < 1 || r->subject < 0 || r->subject >= SUBJECTS || r->total != QUIZ_SIZE) return 0;
    for (t = 0; t < TOPICS; t++) {
        if (r->topic_correct[t] < 0 || r->topic_total[t] < r->topic_correct[t] || r->topic_total[t] > QUIZ_SIZE) return 0;
        correct += r->topic_correct[t]; total += r->topic_total[t];
    }
    return correct == r->correct && total == r->total;
}

static void progress(int profile, int history) {
    FILE *f = fopen("results.txt", "r"); char line[512]; Result r;
    int n[SUBJECTS] = {0}, count = 0, skipped = 0, s, t;
    double sum[SUBJECTS] = {0}, high[SUBJECTS] = {0};
    long tc[SUBJECTS][TOPICS] = {{0}}, tt[SUBJECTS][TOPICS] = {{0}};
    if (!f) { if (errno == ENOENT) puts("No saved results yet. Take a quiz first."); else perror("Cannot read results"); return; }
    if (history) puts("\n--- Quiz history (saved order) ---");
    while (fgets(line, sizeof line, f)) {
        double percent;
        if (!parse_result(line, &r)) { skipped++; continue; }
        if (r.profile != profile) continue;
        s = r.subject; percent = 100.0 * r.correct / r.total;
        n[s]++; count++; sum[s] += percent;
        if (percent > high[s]) high[s] = percent;
        for (t = 0; t < TOPICS; t++) { tc[s][t] += r.topic_correct[t]; tt[s][t] += r.topic_total[t]; }
        if (history) {
            char date[64] = "Time unavailable"; time_t when = (time_t)r.timestamp;
            struct tm *local = localtime(&when);
            if (local) strftime(date, sizeof date, "%Y-%m-%d %H:%M", local);
            printf("%s | %s | %d/%d (%.0f%%)\n", date, subjects[s], r.correct, r.total, percent);
        }
    }
    if (ferror(f)) perror("Error reading results");
    fclose(f);
    if (skipped) printf("Skipped %d invalid result record(s).\n", skipped);
    if (!count) { puts("No saved results for this student."); return; }
    if (!history) {
        double overall = 0, highest = 0;
        puts("\n--- Progress dashboard ---");
        for (s = 0; s < SUBJECTS; s++) {
            overall += sum[s]; if (high[s] > highest) highest = high[s];
            if (!n[s]) { printf("\n%s: not attempted yet.\n", subjects[s]); continue; }
            printf("\n%s: %d quiz(es), average %.1f%%, highest %.1f%%\n", subjects[s], n[s], sum[s]/n[s], high[s]);
            if (sum[s]/n[s] < 60) puts("  Recommendation: practice this subject more.");
            for (t = 0; t < TOPICS; t++) {
                if (!tt[s][t]) printf("  %s: not assessed yet\n", topics[s][t]);
                else printf("  %s: %.1f%%%s\n", topics[s][t], 100.0*tc[s][t]/tt[s][t],
                    100.0*tc[s][t]/tt[s][t] < 60 ? " - needs practice" : "");
            }
        }
        printf("\nOverall: %d quiz(es), average %.1f%%, highest %.1f%%\n", count, overall/count, highest);
        puts("Practice recommendations use a score below 60% across saved attempts.");
    }
}

static void quiz(int profile) {
    int s, i, j, k, indices[sizeof bank / sizeof bank[0]], size = 0;
    int answers[QUIZ_SIZE]; Result r = {0};
    puts("\nChoose a subject (0 cancels):");
    for (i = 0; i < SUBJECTS; i++) printf("%d. %s\n", i+1, subjects[i]);
    s = number("Subject: ", 0, SUBJECTS);
    if (s <= 0) return;
    s--; r.profile = profile; r.subject = s; r.total = QUIZ_SIZE;
    for (i = 0; i < (int)(sizeof bank / sizeof bank[0]); i++) if (bank[i].subject == s) indices[size++] = i;
    /* Fisher-Yates shuffle selects distinct questions in a random order. */
    for (i = size-1; i > 0; i--) { j = rand() % (i+1); k = indices[i]; indices[i] = indices[j]; indices[j] = k; }
    puts("Enter 1-4 for each answer, or 0 to cancel without saving.");
    for (i = 0; i < QUIZ_SIZE; i++) {
        const Question *q = &bank[indices[i]];
        printf("\nQuestion %d/%d [%s]\n%s\n", i+1, QUIZ_SIZE, topics[s][q->topic], q->text);
        for (j = 0; j < 4; j++) printf("  %d. %s\n", j+1, q->options[j]);
        k = number("Answer: ", 0, 4);
        if (k <= 0) { puts("Quiz cancelled; no result saved."); return; }
        answers[i] = k-1; r.topic_total[q->topic]++;
        if (answers[i] == q->answer) { r.correct++; r.topic_correct[q->topic]++; }
    }
    printf("\nFinal score: %d/%d (%.0f%%)\n", r.correct, r.total, 100.0*r.correct/r.total);
    r.timestamp = (long long)time(NULL);
    if (save_result(&r)) puts("Result saved.");
    puts("\n--- Answer review ---");
    for (i = 0; i < QUIZ_SIZE; i++) {
        const Question *q = &bank[indices[i]];
        printf("\n%d. %s\nYour answer: %s [%s]\nCorrect answer: %s\n%s\n",
            i+1, q->text, q->options[answers[i]], answers[i] == q->answer ? "Correct" : "Incorrect",
            q->options[q->answer], q->explanation);
    }
}

int main(void) {
    Profile profiles[MAX_PROFILES]; int count, active = -1, choice, i;
    srand((unsigned)time(NULL)); count = load_profiles(profiles);
    puts("========================================\n STUDY BUDDY - Quiz and Progress Tracker\n========================================");
    for (;;) {
        if (active < 0) {
            puts("\n1. Create student profile\n2. Select student profile\n0. Exit");
            choice = number("Choice: ", 0, 2);
            if (choice <= 0) break;
            if (choice == 1) active = create_profile(profiles, &count);
            else {
                if (!count) { puts("No profiles yet. Create one first."); continue; }
                for (i = 0; i < count; i++) printf("%d. %s (ID %d)\n", i+1, profiles[i].name, profiles[i].id);
                choice = number("Select profile (0 cancels): ", 0, count);
                if (choice > 0) active = choice-1;
                else if (choice < 0) break;
            }
        } else {
            printf("\nStudent: %s\n1. Take quiz\n2. View progress\n3. View history\n4. Switch student\n0. Exit\n", profiles[active].name);
            choice = number("Choice: ", 0, 4);
            if (choice <= 0) break;
            if (choice == 1) quiz(profiles[active].id);
            else if (choice == 2) progress(profiles[active].id, 0);
            else if (choice == 3) progress(profiles[active].id, 1);
            else active = -1;
        }
    }
    puts("Goodbye! Keep learning."); return 0;
}