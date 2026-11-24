#include <stdio.h>
int ids[100];
char names[100][64];
int students = 0;
char subjects[3][30] = {
    "Programming", "Mathematics", "Computer Basics"
};
int totals[100][3] = {0};
int attempts[100][3] = {0};
int highest[100][3] = {0};

void loadData()
{
    FILE *file;
    int id, subject, score, i;
    file = fopen("quiz_students.txt", "r");
    if (file != NULL)
    {
        while (students < 100 &&
               fscanf(file, "%d %63[^\n]", &ids[students], names[students]) == 2)
            students++;
        fclose(file);
    }

    file = fopen("quiz_scores.txt", "r");
    if (file != NULL)
    {
        while (fscanf(file, "%d %d %d", &id, &subject, &score) == 3)
        {
            if (subject >= 0 && subject < 3 && score >= 0 && score <= 5)
            {
                for (i = 0; i < students; i++)
                {
                    if (ids[i] == id)
                    {
                        totals[i][subject] += score;
                        attempts[i][subject]++;
                        if (score > highest[i][subject])
                            highest[i][subject] = score;
                    }
                }
            }
        }
        fclose(file);
    }
}

void saveScore(int student, int subject, int score)
{
    FILE *file;
    int written, closed;
    file = fopen("quiz_scores.txt", "a");
    if (file == NULL)
    {
        printf("  Could not save score to file.\n");
        return;
    }
    written = fprintf(file, "%d %d %d\n", ids[student], subject, score);
    closed = fclose(file);
    if (written < 0 || closed != 0)
        printf("  Could not save score to file.\n");
    else
        printf("  [SAVED] Quiz result saved to file.\n");
}

int inputNumber()
{
    int n, ok, ch;
    ok = scanf("%d", &n);
    ch = getchar();
    while (ch != '\n' && ch != EOF)
        ch = getchar();
    if (ok == EOF)
        return -1;
    if (ok != 1)
        return -2;
    return n;
}
void addStudent()
{
    FILE *file;
    int ch, i, written, closed;
    if (students == 100)
    {
        printf("Only 100 students can register.\n");
        return;
    }
    printf("\n---------------- REGISTER STUDENT ------------------\n");
    printf("  Enter your name: ");
    if (scanf(" %63[^\n]", names[students]) != 1)
        return;
    ch = getchar();
    while (ch != '\n' && ch != EOF)
        ch = getchar();
    /* Continue from the largest registered ID. */
    ids[students] = 1;
    for (i = 0; i < students; i++)
        if (ids[i] >= ids[students])
            ids[students] = ids[i] + 1;

    file = fopen("quiz_students.txt", "a");
    if (file == NULL)
    {
        printf("  Registration failed: cannot open student file.\n");
        return;
    }
    written = fprintf(file, "%d %s\n", ids[students], names[students]);
    closed = fclose(file);
    if (written < 0 || closed != 0)
    {
        printf("  Registration failed: could not save student.\n");
        return;
    }
    printf("\n  [SUCCESS] Registration complete!\n");
    printf("  Student : %s\n", names[students]);
    printf("  Your ID : %d\n", ids[students]);
    printf("----------------------------------------------------\n");
    students++;
}
void takeQuiz(int student)
{
    char questions[3][5][200] = {
        {
            "Which prints output in C?\n  [1] scanf\n  [2] printf\n  [3] int\n  [4] return",
            "What is 7 / 2 with integers?\n  [1] 3\n  [2] 3.5\n  [3] 4\n  [4] 2",
            "First array index in C?\n  [1] 1\n  [2] -1\n  [3] 0\n  [4] 2",
            "Which loop runs at least once?\n  [1] for\n  [2] while\n  [3] None\n  [4] do-while",
            "Which operator compares equality?\n  [1] =\n  [2] ==\n  [3] +\n  [4] %"
        },
        {
            "What is 5 + 3?\n  [1] 7\n  [2] 8\n  [3] 9\n  [4] 10",
            "What is 6 * 4?\n  [1] 24\n  [2] 20\n  [3] 10\n  [4] 30",
            "Solve x + 5 = 12.\n  [1] 5\n  [2] 6\n  [3] 7\n  [4] 8",
            "Sum of angles in a triangle?\n  [1] 90\n  [2] 360\n  [3] 270\n  [4] 180",
            "Area of a square with side 4?\n  [1] 8\n  [2] 16\n  [3] 12\n  [4] 20"
        },
        {
            "Which executes instructions?\n  [1] Monitor\n  [2] CPU\n  [3] Keyboard\n  [4] Printer",
            "Which loses data when power is off?\n  [1] RAM\n  [2] SSD\n  [3] Hard disk\n  [4] ROM",
            "Which is an input device?\n  [1] Monitor\n  [2] Speaker\n  [3] Keyboard\n  [4] Printer",
            "Bits in one byte?\n  [1] 2\n  [2] 4\n  [3] 16\n  [4] 8",
            "Which is an operating system?\n  [1] CPU\n  [2] Windows\n  [3] RAM\n  [4] USB"
        }
    };
    int answers[3][5] = {
        {2, 1, 3, 4, 2},
        {2, 1, 3, 4, 2},
        {2, 1, 3, 4, 2}
    };
    int subject, answer, score = 0, i;
    printf("\n---------------- CHOOSE A SUBJECT ------------------\n");
    printf("  [1] Programming\n");
    printf("  [2] Mathematics\n");
    printf("  [3] Computer Basics\n");
    printf("----------------------------------------------------\n");
    printf("  Choose subject > ");
    subject = inputNumber();
    if (subject < 1 || subject > 3)
    {
        printf("Invalid subject.\n");
        return;
    }
    subject--;
    for (i = 0; i < 5; i++)
    {
        printf("\n====================================================\n");
        printf("  %s | QUESTION %d OF 5\n", subjects[subject], i + 1);
        printf("----------------------------------------------------\n");
        printf("%s\n", questions[subject][i]);
        printf("----------------------------------------------------\n");
        printf("  Your answer (1-4, 0 to cancel) > ");
        answer = inputNumber();
        while (answer != -1 && answer != 0 &&
               (answer < 1 || answer > 4))
        {
            printf("Enter 1-4, or 0 to cancel: ");
            answer = inputNumber();
        }
        if (answer == 0 || answer == -1)
        {
            printf("Quiz cancelled.\n");
            return;
        }
        if (answer == answers[subject][i])
        {
            score++;
            printf("\n  [CORRECT] Well done!\n");
        }
        else
        {
            printf("\n  [INCORRECT] Correct option: %d\n",
                   answers[subject][i]);
        }
    }
    printf("\n================== QUIZ RESULT =====================\n");
    printf("  Student    : %s\n", names[student]);
    printf("  Subject    : %s\n", subjects[subject]);
    printf("  Score      : %d / 5\n", score);
    printf("  Percentage : %d%%\n", score * 20);
    if (score == 5)
        printf("  Excellent! A perfect score.\n");
    else if (score >= 3)
        printf("  Good effort! Keep practicing.\n");
    else
        printf("  Keep trying! Review this subject and try again.\n");
    printf("====================================================\n");
    totals[student][subject] += score;
    attempts[student][subject]++;
    if (score > highest[student][subject])
        highest[student][subject] = score;
    saveScore(student, subject, score);
}
void showProgress(int student)
{
    int i;
    int allTotal = 0;
    int allAttempts = 0;
    int best = 0;
    float average;
    printf("\n================ PROGRESS REPORT ===================\n");
    printf("  Student: %s | ID: %d\n", names[student], ids[student]);
    printf("----------------------------------------------------\n");
    for (i = 0; i < 3; i++)
    {
        if (attempts[student][i] == 0)
        {
            printf("\n%s: not attempted\n", subjects[i]);
        }
        else
        {
            average = totals[student][i] * 20.0f /
                      attempts[student][i];
            printf("\n  SUBJECT: %s\n", subjects[i]);
            printf("  ----------------------------------------------\n");
            printf("  Attempts : %d\n", attempts[student][i]);
            printf("  Average  : %.1f%%\n", average);
            printf("  Highest  : %d%%\n", highest[student][i] * 20);
            if (average < 60)
                printf("You should practice this subject more.\n");
            allTotal += totals[student][i];
            allAttempts += attempts[student][i];
            if (highest[student][i] > best)
                best = highest[student][i];
        }
    }
    if (allAttempts == 0)
    {
        printf("\nNo quiz results yet.\n");
    }
    else
    {
        average = allTotal * 20.0f / allAttempts;
        printf("\n---------------- OVERALL PERFORMANCE ---------------\n");
        printf("  Overall average: %.1f%%\n", average);
        printf("  Overall highest: %d%%\n", best * 20);
        printf("====================================================\n");
    }
}
int main()
{
    int choice, id, found, i, menu;
    loadData();
    printf("\n====================================================\n");
    printf("                  QUIZ BUDDY\n");
    printf("            Learn - Practice - Improve\n");
    printf("====================================================\n");
    do
    {
        printf("\n==================== MAIN MENU =====================\n");
        printf("  [1] Register student\n");
        printf("  [2] Select student\n");
        printf("  [0] Exit\n");
        printf("----------------------------------------------------\n");
        printf("  Enter your choice > ");
        choice = inputNumber();
        if (choice == 1)
        {
            addStudent();
        }
        else if (choice == 2)
        {
            if (students == 0)
            {
                printf("Register a student first.\n");
                continue;
            }
            printf("\n---------------- REGISTERED STUDENTS ---------------\n");
            for (i = 0; i < students; i++)
                printf("  ID %-3d | %s\n", ids[i], names[i]);
            printf("----------------------------------------------------\n");
            printf("Enter your ID: ");
            id = inputNumber();
            found = -1;
            for (i = 0; i < students; i++)
            {
                if (ids[i] == id)
                    found = i;
            }
            if (found == -1)
            {
                printf("ID not found.\n");
            }
            else
            {
                do
                {
                    printf("\n=================== STUDENT MENU ===================\n");
                    printf("  Welcome, %s!\n", names[found]);
                    printf("----------------------------------------------------\n");
                    printf("  [1] Take quiz\n");
                    printf("  [2] View progress\n");
                    printf("  [0] Back to main menu\n");
                    printf("----------------------------------------------------\n");
                    printf("  Enter your choice > ");
                    menu = inputNumber();
                    if (menu == 1)
                        takeQuiz(found);
                    else if (menu == 2)
                        showProgress(found);
                    else if (menu != 0 && menu != -1)
                        printf("Invalid choice.\n");
                } while (menu != 0 && menu != -1);
                if (menu == -1)
                    break;
            }
        }
        else if (choice != 0 && choice != -1)
        {
            printf("Invalid choice.\n");
        }
    } while (choice != 0 && choice != -1);
    printf("\n====================================================\n");
    printf("  Thank you for using Quiz Buddy. Keep learning!\n");
    printf("====================================================\n");
    return 0;
}