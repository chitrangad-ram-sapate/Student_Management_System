╔══════════════════════════════════════════════════════╗
║   STUDENT RECORD MANAGEMENT SYSTEM                   ║
║   C Project — Complete Implementation                ║
╚══════════════════════════════════════════════════════╝

────────────────────────────────────────────────────────
  HOW TO COMPILE & RUN
────────────────────────────────────────────────────────

  ANY system with a C compiler (gcc, clang, MSVC, MinGW, etc.):

    gcc -Wall -o student_records main.c student.c file_io.c utils.c
    ./student_records

  (Uses 100% standard C — no POSIX or platform-specific functions.)

────────────────────────────────────────────────────────
  QUICK WALKTHROUGH (5-Minute Demo)
────────────────────────────────────────────────────────

  1. ADD STUDENTS (option 1)
     - Add 5-6 students with different names, marks, ages.
     - The grade is auto-calculated.

  2. VIEW ALL (option 2)
     - Shows a formatted table of all records.

  3. SEARCH BY ROLL (option 3)
     - Enter a roll number to find a specific student.

  4. SEARCH BY NAME (option 4)
     - Type a partial name (e.g. "raj") — case-insensitive.

  5. UPDATE (option 5)
     - Enter roll number, then new data.

  6. DELETE (option 6)
     - Enter roll number to remove a student.

  7. SORT BY NAME (option 7)
     - Alphabetical order, case-insensitive.

  8. SORT BY MARKS (option 8)
     - Highest to lowest.

  9. STATISTICS (option 9)
     - Total students, average, highest, lowest, grade distribution.

  10. EXPORT TO TEXT (option 10)
      - Creates "records_export.txt" with formatted table.

  11. SAVE MANUALLY (option 11)
      - Saves to "records.dat" (auto-saved on add/delete/update too).

  0. EXIT
      - Saves and exits cleanly.

────────────────────────────────────────────────────────
  FILES
────────────────────────────────────────────────────────

  main.c        — CLI menu, main loop
  student.h     — Struct, function declarations, constants
  student.c     — Core linked-list operations (CRUD + sort + search)
  file_io.c     — Binary save/load, text export
  utils.c       — Grade calculator, validation, statistics, input helpers
  README.txt    — This file

────────────────────────────────────────────────────────
  DATA FILES (auto-generated)
────────────────────────────────────────────────────────

  records.dat         — Binary file (persists between runs)
  records_export.txt  — Human-readable export (option 10)

────────────────────────────────────────────────────────
  GRADING SCALE
────────────────────────────────────────────────────────

  90-100   : A
  75-89    : B
  60-74    : C
  50-59    : D
  0-49     : F

────────────────────────────────────────────────────────
  KEY DESIGN DECISIONS
────────────────────────────────────────────────────────

  * Singly linked list for in-memory storage
  * Binary file (records.dat) for persistence
  * Auto-grade on every add/update
  * Case-insensitive partial-name search
  * Insertion sort (stable, simple)
  * Input validation: duplicate roll, empty name,
    marks out of range, age out of range
  * Auto-save on add/delete/update + manual save option
  * Text export for readability

────────────────────────────────────────────────────────
  TROUBLESHOOTING
────────────────────────────────────────────────────────

  Q: Program doesn't compile?
  A: Make sure all four .c files are in the same folder.
     Use the exact gcc command above.  Only standard C
     headers are used — no POSIX or platform extensions.

  Q: Records lost?
  A: Check that records.dat is in the same folder
     and the program has write permissions.

  Q: Weird characters in name?
  A: fgets() captures newlines; the code strips them.
     If you still have issues, clear the input buffer.

────────────────────────────────────────────────────────
