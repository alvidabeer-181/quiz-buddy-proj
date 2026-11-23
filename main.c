#include <stdio.h>

int ids[100];
char names[100][64];
int students = 0;

char subjects[3][30] = {
    "Programming", "Mathematics", "Computer Basics"
};

/* Each student's total, attempts and highest score per subject */
int totals[100][3] = {0};
int attempts[100][3] = {0};
int highest[100][3] = {0};

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
    int ch;

    if (students == 100)
    {
        printf("Only 100 students can register.\n");
        return;
    }

    printf("Enter your name: ");

    if (scanf(" %63[^\n]", names[students]) != 1)
        return;

    ch = getchar();
    while (ch != '\n' && ch != EOF)
        ch = getchar();

    ids[students] = students + 1;

    printf("Registered! Your ID is %d\n", ids[students]);

    students++;
}

void takeQuiz(int student)
{
    char questions[3][5][200] = {
        {
            "Which prints output in C?\n1. scanf\n2. printf\n3. int\n4. return",
            "What is 7 / 2 with integers?\n1. 3\n2. 3.5\n3. 4\n4. 2",
            "First array index in C?\n1. 1\n2. -1\n3. 0\n4. 2",
            "Which loop runs at least once?\n1. for\n2. while\n3. None\n4. do-while",
            "Which operator compares equality?\n1. =\n2. ==\n3. +\n4. %"
        },
        {
            "What is 5 + 3?\n1. 7\n2. 8\n3. 9\n4. 10",
            "What is 6 * 4?\n1. 24\n2. 20\n3. 10\n4. 30",
            "Solve x + 5 = 12.\n1. 5\n2. 6\n3. 7\n4. 8",
            "Sum of angles in a triangle?\n1. 90\n2. 360\n3. 270\n4. 180",
            "Area of a square with side 4?\n1. 8\n2. 16\n3. 12\n4. 20"
        },
        {
            "Which executes instructions?\n1. Monitor\n2. CPU\n3. Keyboard\n4. Printer",
            "Which loses data when power is off?\n1. RAM\n2. SSD\n3. Hard disk\n4. ROM",
            "Which is an input device?\n1. Monitor\n2. Speaker\n3. Keyboard\n4. Printer",
            "Bits in one byte?\n1. 2\n2. 4\n3. 16\n4. 8",
            "Which is an operating system?\n1. CPU\n2. Windows\n3. RAM\n4. USB"
        }
    };

    int answers[3][5] = {
        {2, 1, 3, 4, 2},
        {2, 1, 3, 4, 2},
        {2, 1, 3, 4, 2}
    };

    int subject, answer, score = 0, i;

    printf("\n1. Programming\n");
    printf("2. Mathematics\n");
    printf("3. Computer Basics\n");
    printf("Choose subject: ");

    subject = inputNumber();

    if (subject < 1 || subject > 3)
    {
        printf("Invalid subject.\n");
        return;
    }

    subject--;

    for (i = 0; i < 5; i++)
    {
        printf("\nQuestion %d\n", i + 1);
        printf("%s\n", questions[subject][i]);
        printf("Answer (1-4, 0 to cancel): ");

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
            printf("Correct!\n");
        }
        else
        {
            printf("Wrong. Correct option: %d\n",
                   answers[subject][i]);
        }
    }

    printf("\nScore: %d/5\n", score);
    printf("Percentage: %d%%\n", score * 20);

    /* Update progress only after completing the quiz */
    totals[student][subject] += score;
    attempts[student][subject]++;

    if (score > highest[student][subject])
        highest[student][subject] = score;
}

void showProgress(int student)
{
    int i;
    int allTotal = 0;
    int allAttempts = 0;
    int best = 0;
    float average;

    printf("\nProgress for %s\n", names[student]);

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

            printf("\nSubject: %s\n", subjects[i]);
            printf("Attempts: %d\n", attempts[student][i]);
            printf("Average: %.1f%%\n", average);
            printf("Highest: %d%%\n", highest[student][i] * 20);

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

        printf("\nOverall average: %.1f%%\n", average);
        printf("Overall highest: %d%%\n", best * 20);
    }
}

int main()
{
    int choice, id, found, i, menu;

    printf("STUDY BUDDY\n");
    printf("Students and scores reset when you close the program.\n");

    do
    {
        printf("\n1. Register student\n");
        printf("2. Select student\n");
        printf("0. Exit\n");
        printf("Choice: ");

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

            for (i = 0; i < students; i++)
                printf("ID %d: %s\n", ids[i], names[i]);

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
                    printf("\nHello %s\n", names[found]);
                    printf("1. Take quiz\n");
                    printf("2. View progress\n");
                    printf("0. Back\n");
                    printf("Choice: ");

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

    printf("Goodbye!\n");

    return 0;
}