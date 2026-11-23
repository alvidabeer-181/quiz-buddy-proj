STUDY BUDDY - BEGINNER VERSION
Dabeer Alvi (26K-1022), Haseef Ahmed Siddqui (26K-0825)
BCS-1H

Extract this folder and double-click run.bat.
Register a student, then select the student by ID to take quizzes.

Source: main.c
Build: gcc -std=c11 -Wall -Wextra -Wpedantic main.c -o StudyBuddy.exe
Or run build.bat with GCC installed and on PATH.

This beginner version uses only stdio.h, global arrays, ordinary functions,
loops, if/else, scanf/printf and text-file handling. No static, structures,
typedef, dynamic memory or randomization. FILE pointers and & in scanf are
needed for the basic file and input operations.

Three subjects have five fixed questions each. Correct options appear after
each answer. History, averages, highest scores and below-60-percent subject
recommendations are included. Random selection, dates and topic statistics
from the earlier version have been removed to simplify the program.

students.txt stores ID and name. scores.txt stores ID, subject index and score.
Files are created here when launched with run.bat. This version starts fresh;
it does not import the older version's files. Close the program and delete
both students.txt and scores.txt to reset all profiles and scores.
Maximum: 100 students. Names: 63 characters. Use one instance at a time.
Do not manually edit saved files; this simple version assumes its own format.

Read the code in this order:
main -> inputNumber -> loadStudents -> addStudent -> takeQuiz -> showProgress
questions[subject][question] selects the question text.
answers[subject][question] stores the correct option number (1-4).

Checked: warning-free GCC build, perfect quiz scoring, statistics, saving and
reloading, separate student histories, cancellation, invalid menu input and
end-of-input exit. Windows executable tested; other platforms not tested.
