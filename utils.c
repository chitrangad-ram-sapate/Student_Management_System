#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "student.h"

/* ================================================================
 *  Compute grade from marks (0‑100)
 * ================================================================ */
char compute_grade(float marks) {
    if (marks >= 90.0f) return 'A';
    if (marks >= 75.0f) return 'B';
    if (marks >= 60.0f) return 'C';
    if (marks >= 50.0f) return 'D';
    return 'F';
}

/* ================================================================
 *  Check for duplicate roll number
 * ================================================================ */
static int is_duplicate_roll(Node *head, int roll, int skip_roll) {
    Node *cur = head;
    while (cur) {
        if (cur->data.roll == roll && cur->data.roll != skip_roll)
            return 1;
        cur = cur->next;
    }
    return 0;
}

/* ================================================================
 *  Validate student data
 *  Returns 0 if valid, or negative error code
 *  is_update > 0 means skip the duplicate check for its own roll
 * ================================================================ */
int validate_student(Node *head, Student s, int is_update) {
    if (s.roll <= 0) return -5;

    /* empty name check */
    if (s.name[0] == '\0') return -2;
    int all_spaces = 1;
    for (int i = 0; s.name[i]; i++) {
        if (!isspace((unsigned char)s.name[i])) {
            all_spaces = 0;
            break;
        }
    }
    if (all_spaces) return -2;

    /* marks range */
    if (s.marks < 0.0f || s.marks > 100.0f) return -3;

    /* age range */
    if (s.age < 1 || s.age > 99) return -4;

    /* duplicate roll — skip own roll during update */
    int skip = is_update ? s.roll : -1;
    if (is_duplicate_roll(head, s.roll, skip)) return -1;

    return 0;
}

/* ================================================================
 *  Print statistics report
 * ================================================================ */
void print_statistics(Node *head) {
    if (!head) {
        printf("\n  (No students — nothing to report.)\n");
        return;
    }

    int   count      = 0;
    float total      = 0.0f;
    float highest    = -1.0f;
    float lowest     = 101.0f;
    int   grade_counts[5] = {0};  /* A,B,C,D,F */

    Node *cur = head;
    while (cur) {
        count++;
        total += cur->data.marks;

        if (cur->data.marks > highest) highest = cur->data.marks;
        if (cur->data.marks < lowest)  lowest  = cur->data.marks;

        switch (cur->data.grade) {
            case 'A': grade_counts[0]++; break;
            case 'B': grade_counts[1]++; break;
            case 'C': grade_counts[2]++; break;
            case 'D': grade_counts[3]++; break;
            case 'F': grade_counts[4]++; break;
        }
        cur = cur->next;
    }

    float average = total / count;

    printf("\n");
    printf("  ╔═══════════════════════════╗\n");
    printf("  ║     CLASS STATISTICS      ║\n");
    printf("  ╠═══════════════════════════╣\n");
    printf("  ║ Total students : %-7d ║\n", count);
    printf("  ║ Average marks  : %-7.2f ║\n", average);
    printf("  ║ Highest marks  : %-7.2f ║\n", highest);
    printf("  ║ Lowest marks   : %-7.2f ║\n", lowest);
    printf("  ╠═══════════════════════════╣\n");
    printf("  ║ Grade Distribution:      ║\n");
    printf("  ║   A : %-3d    D : %-3d    ║\n", grade_counts[0], grade_counts[3]);
    printf("  ║   B : %-3d    F : %-3d    ║\n", grade_counts[1], grade_counts[4]);
    printf("  ║   C : %-3d              ║\n", grade_counts[2]);
    printf("  ╚═══════════════════════════╝\n\n");
}

/* ================================================================
 *  Read int from stdin safely
 * ================================================================ */
int read_int(int *value) {
    return scanf("%d", value) == 1;
}

/* ================================================================
 *  Read float from stdin safely
 * ================================================================ */
int read_float(float *value) {
    return scanf("%f", value) == 1;
}

/* ================================================================
 *  Clear input buffer
 * ================================================================ */
void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
