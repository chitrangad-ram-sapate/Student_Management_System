#include <stdio.h>
#include <stdlib.h>
#include "student.h"

/* ---------- Helper: prompt for student data ---------- */
static void input_student(Student *s) {
    printf("    Roll number : ");
    read_int(&s->roll);
    clear_input();

    printf("    Name        : ");
    fgets(s->name, MAX_NAME, stdin);
    s->name[strcspn(s->name, "\n")] = '\0';   /* strip newline */

    printf("    Age         : ");
    read_int(&s->age);
    clear_input();

    printf("    Marks (0-100): ");
    read_float(&s->marks);
    clear_input();

    s->grade = compute_grade(s->marks);
}

/* ---------- Print validation error ---------- */
static void print_val_error(int code) {
    switch (code) {
        case -1: printf("  ERROR: Duplicate roll number!\n"); break;
        case -2: printf("  ERROR: Name cannot be empty.\n"); break;
        case -3: printf("  ERROR: Marks must be between 0 and 100.\n"); break;
        case -4: printf("  ERROR: Age must be between 1 and 99.\n"); break;
        case -5: printf("  ERROR: Roll number must be positive.\n"); break;
    }
}

/* ================================================================
 *  MAIN
 * ================================================================ */
int main(void) {
    Node *head = NULL;
    int   running = 1;
    int   choice;

    /* --- Load saved data on startup --- */
    head = load_from_file(FILENAME);

    /* --- Main loop --- */
    while (running) {
        printf("\n");
        printf("  ========================================\n");
        printf("  |   STUDENT RECORD MANAGEMENT SYSTEM   |\n");
        printf("  |======================================|\n");
        printf("  |  1. Add Student                      |\n");
        printf("  |  2. View All Students                |\n");
        printf("  |  3. Search by Roll Number            |\n");
        printf("  |  4. Search by Name                   |\n");
        printf("  |  5. Update Student                   |\n");
        printf("  |  6. Delete Student                   |\n");
        printf("  |  7. Sort by Name (A-Z)               |\n");
        printf("  |  8. Sort by Marks (High-Low)         |\n");
        printf("  |  9. Statistics Report                |\n");
        printf("  | 10. Export to Text File              |\n");
        printf("  | 11. Save to Binary File              |\n");
        printf("  |  0. Exit                             |\n");
        printf("  ========================================\n");
        
        printf("  Enter choice: ");

        if (!read_int(&choice)) {
            clear_input();
            printf("  Invalid input. Try again.\n");
            continue;
        }
        clear_input();

        switch (choice) {

        /* ---------- ADD ---------- */
        case 1: {
            Student s;
            printf("\n  --- Add New Student ---\n");
            input_student(&s);

            int val = validate_student(head, s, 0);
            if (val != 0) {
                print_val_error(val);
                break;
            }

            head = add_student(head, s);
            printf("  Student added successfully!\n");
            save_to_file(head, FILENAME);
            break;
        }

        /* ---------- VIEW ALL ---------- */
        case 2:
            print_all(head);
            break;

        /* ---------- SEARCH BY ROLL ---------- */
        case 3: {
            int roll;
            printf("  Enter roll number: ");
            if (!read_int(&roll)) { clear_input(); break; }
            clear_input();

            Node *n = search_by_roll(head, roll);
            if (n) {
                printf("\n  Found:\n");
                printf("  Roll: %d | Name: %s | Age: %d | Marks: %.2f | Grade: %c\n",
                       n->data.roll, n->data.name, n->data.age,
                       n->data.marks, n->data.grade);
            } else {
                printf("  Student not found.\n");
            }
            break;
        }

        /* ---------- SEARCH BY NAME ---------- */
        case 4: {
            char partial[MAX_NAME];
            printf("  Enter name or part of name: ");
            fgets(partial, MAX_NAME, stdin);
            partial[strcspn(partial, "\n")] = '\0';
            search_by_name(head, partial);
            break;
        }

        /* ---------- UPDATE ---------- */
        case 5: {
            int roll;
            printf("  Enter roll number to update: ");
            if (!read_int(&roll)) { clear_input(); break; }
            clear_input();

            Node *n = search_by_roll(head, roll);
            if (!n) {
                printf("  Student not found.\n");
                break;
            }

            printf("  Enter new data:\n");
            Student s;
            input_student(&s);

            int val = validate_student(head, s, 1);  /* is_update=1 */
            if (val != 0) {
                print_val_error(val);
                break;
            }

            if (update_student(head, roll, s))
                printf("  Updated successfully!\n");
            else
                printf("  Update failed.\n");

            save_to_file(head, FILENAME);
            break;
        }

        /* ---------- DELETE ---------- */
        case 6: {
            int roll;
            printf("  Enter roll number to delete: ");
            if (!read_int(&roll)) { clear_input(); break; }
            clear_input();

            head = delete_student(head, roll);
            save_to_file(head, FILENAME);
            break;
        }

        /* ---------- SORT BY NAME ---------- */
        case 7:
            head = sort_by_name(head);
            printf("  Sorted by name (A-Z).\n");
            print_all(head);
            break;

        /* ---------- SORT BY MARKS ---------- */
        case 8:
            head = sort_by_marks(head);
            printf("  Sorted by marks (highest first).\n");
            print_all(head);
            break;

        /* ---------- STATISTICS ---------- */
        case 9:
            print_statistics(head);
            break;

        /* ---------- EXPORT TO TEXT ---------- */
        case 10:
            export_to_text(head, EXPORT_FILE);
            break;

        /* ---------- MANUAL SAVE ---------- */
        case 11:
            save_to_file(head, FILENAME);
            break;

        /* ---------- EXIT ---------- */
        case 0:
            printf("  Saving and exiting...\n");
            save_to_file(head, FILENAME);
            free_list(head);
            running = 0;
            break;

        default:
            printf("  Invalid choice. Please enter 0-11.\n");
        }
    }

    return 0;
}
