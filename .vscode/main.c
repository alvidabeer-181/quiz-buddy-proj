#include <stdio.h>

int ids[100];
char names[100][64];
int students = 0;
char subjects[3][30] = {"Programming", "Mathematics", "Computer Basics"};


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

void loadStudents()
{
    FILE *file;
    file = fopen("students.txt", "r");
    if (file == NULL)
        return;

    while (students < 100)
    {
        if (fscanf(file, "%d %63[^\n]", &ids[students], names[students]) != 2)
            break;
        students++;
    }
    fclose(file);
}

void addStudent()
{
    FILE *file;
    int i, ch;
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

    ids[students] = 1;
    for (i = 0; i < students; i++)
    {
        if (ids[i] >= ids[students])
            ids[students] = ids[i] + 1;
    }
    file = fopen("students.txt", "a");
    if (file == NULL)
    {
        printf("Could not save student.\n");
        return;
    }
    i = fprintf(file, "%d %s\n", ids[students], names[students]);
    if (fclose(file) != 0 || i < 0)
    {
        printf("Could not save student.\n");
        return;
    }
    printf("Registered! Your ID is %d\n", ids[students]);
    students++;
}

void takeQuiz(int id)
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
    int answers[3][5] = {{2,1,3,4,2}, {2,1,3,4,2}, {2,1,3,4,2}};
    int subject, answer, score = 0, i, saved;
    FILE *file;

    printf("1. Programming\n2. Mathematics\n3. Computer Basics\nChoose subject: ");
    subject = inputNumber();
    if (subject < 1 || subject > 3)
    {
        printf("Invalid subject.\n");
        return;
    }
    subject--;
    for (i = 0; i < 5; i++)
    {
        printf("\nQuestion %d\n%s\nAnswer (1-4, 0 to cancel): ", i + 1, questions[subject][i]);
        answer = inputNumber();
        while (answer != -1 && answer != 0 && (answer < 1 || answer > 4))
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
            printf("Wrong. Correct option: %d\n", answers[subject][i]);
    }
    printf("\nScore: %d/5\nPercentage: %d%%\n", score, score * 20);
    file = fopen("scores.txt", "a");
    if (file == NULL)
    {
        printf("Could not save score.\n");
        return;
    }
    saved = fprintf(file, "%d %d %d\n", id, subject, score);
    if (fclose(file) != 0 || saved < 0)
        printf("Could not save score.\n");
    else
        printf("Score saved.\n");
}

void showProgress(int id)
{
    FILE *file;
    int studentId, subject, score, i;
    int total[3] = {0,0,0};
    int count[3] = {0,0,0};
    int highest[3] = {0,0,0};
    int allTotal = 0, allCount = 0, best = 0;
    float average;
    file = fopen("scores.txt", "r");
    if (file == NULL)
    {
        printf("No saved scores available.\n");
        return;
    }
    printf("\nPrevious results:\n");
    while (fscanf(file, "%d %d %d", &studentId, &subject, &score) == 3)
    {
        if (studentId == id && subject >= 0 && subject < 3 && score >= 0 && score <= 5)
        {
            printf("%s: %d/5 (%d%%)\n", subjects[subject], score, score * 20);
            total[subject] += score;
            count[subject]++;
            if (score > highest[subject])
                highest[subject] = score;
            allTotal += score;
            allCount++;
            if (score > best)
                best = score;
        }
    }
    fclose(file);
    if (allCount == 0)
    {
        printf("This student has no quiz results yet.\n");
        return;
    }
    for (i = 0; i < 3; i++)
    {
        if (count[i] == 0)
            printf("\n%s: not attempted\n", subjects[i]);
        else
        {
            average = total[i] * 20.0f / count[i];
            printf("\n%s: %d attempts\nAverage: %.1f%%\nHighest: %d%%\n",
                   subjects[i], count[i], average, highest[i] * 20);
            if (average < 60)
                printf("You should practice this subject more.\n");
        }
    }
    printf("\nOverall average: %.1f%%\nOverall highest: %d%%\n",
           allTotal * 20.0f / allCount, best * 20);
}

int main()
{
    int choice, id, found, i, menu;
    loadStudents();
    printf("STUDY BUDDY\n");
    do
    {
        printf("\n1. Register student\n2. Select student\n0. Exit\nChoice: ");
        choice = inputNumber();
        if (choice == 1)
            addStudent();
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
                if (ids[i] == id)
                    found = i;
            if (found == -1)
                printf("ID not found.\n");
            else
            {
                do
                {
                    printf("\nHello %s\n1. Take quiz\n2. History and progress\n0. Back\nChoice: ", names[found]);
                    menu = inputNumber();
                    if (menu == 1)
                        takeQuiz(id);
                    else if (menu == 2)
                        showProgress(id);
                    else if (menu != 0 && menu != -1)
                        printf("Invalid choice.\n");
                } while (menu != 0 && menu != -1);
                if (menu == -1)
                    break;
            }
        }
        else if (choice != 0 && choice != -1)
            printf("Invalid choice.\n");
    } while (choice != 0 && choice != -1);
    printf("Goodbye!\n");
    return 0;
}
