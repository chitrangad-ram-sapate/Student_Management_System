#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "student.h"

/* ---------- Standard C case-insensitive string compare ---------- */
static int stricmp_custom(const char *a, const char *b) {
    while (*a && *b) {
        int diff = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (diff != 0) return diff;
        a++; b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

/* ================================================================
 *  Create a new node (copies data)
 * ================================================================ */
Node* create_node(Student s) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (!n) {
        fprintf(stderr, "ERROR: Memory allocation failed.\n");
        return NULL;
    }
    n->data = s;
    n->next = NULL;
    return n;
}

/* ================================================================
 *  Add student to end of list — returns (possibly new) head
 * ================================================================ */
Node* add_student(Node *head, Student s) {
    Node *n = create_node(s);
    if (!n) return head;

    if (!head) return n;          /* first element */

    Node *cur = head;
    while (cur->next)
        cur = cur->next;
    cur->next = n;
    return head;
}

/* ================================================================
 *  Print all students in a formatted table
 * ================================================================ */
void print_all(Node *head) {
    if (!head) {
        printf("\n  (No students in the system.)\n");
        return;
    }

    printf("\n  %-6s %-20s %-5s %-7s %-5s\n",
           "Roll", "Name", "Age", "Marks", "Grade");
    printf("  %-6s %-20s %-5s %-7s %-5s\n",
           "------", "--------------------", "---", "-------", "-----");

    Node *cur = head;
    while (cur) {
        printf("  %-6d %-20s %-5d %-7.2f %-5c\n",
               cur->data.roll, cur->data.name, cur->data.age,
               cur->data.marks, cur->data.grade);
        cur = cur->next;
    }
    printf("\n");
}

/* ================================================================
 *  Free entire linked list
 * ================================================================ */
void free_list(Node *head) {
    Node *cur = head;
    while (cur) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

/* ================================================================
 *  Search by exact roll number — returns node pointer or NULL
 * ================================================================ */
Node* search_by_roll(Node *head, int roll) {
    Node *cur = head;
    while (cur) {
        if (cur->data.roll == roll)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

/* ================================================================
 *  Delete student by roll — returns (possibly new) head
 * ================================================================ */
Node* delete_student(Node *head, int roll) {
    if (!head) return NULL;

    /* head node matches */
    if (head->data.roll == roll) {
        Node *new_head = head->next;
        free(head);
        printf("  Student (Roll #%d) deleted.\n", roll);
        return new_head;
    }

    /* traverse */
    Node *cur = head;
    while (cur->next && cur->next->data.roll != roll)
        cur = cur->next;

    if (cur->next) {
        Node *tmp = cur->next;
        cur->next = tmp->next;
        free(tmp);
        printf("  Student (Roll #%d) deleted.\n", roll);
    } else {
        printf("  Student not found.\n");
    }
    return head;
}

/* ================================================================
 *  Update an existing student (matched by roll)
 *  Returns 1 on success, 0 on failure
 * ================================================================ */
int update_student(Node *head, int roll, Student new_data) {
    Node *node = search_by_roll(head, roll);
    if (!node) return 0;

    node->data = new_data;
    return 1;
}

/* ================================================================
 *  Sort by name (ascending, case-insensitive) — insertion sort
 * ================================================================ */
Node* sort_by_name(Node *head) {
    if (!head || !head->next) return head;

    Node *sorted = NULL;
    Node *cur    = head;

    while (cur) {
        Node *next = cur->next;
        /* insert cur into sorted list */
        if (!sorted ||
            stricmp_custom(cur->data.name, sorted->data.name) < 0) {
            cur->next = sorted;
            sorted = cur;
        } else {
            Node *s = sorted;
            while (s->next &&
                   stricmp_custom(cur->data.name, s->next->data.name) >= 0)
                s = s->next;
            cur->next = s->next;
            s->next = cur;
        }
        cur = next;
    }
    return sorted;
}

/* ================================================================
 *  Sort by marks (descending) — insertion sort
 * ================================================================ */
Node* sort_by_marks(Node *head) {
    if (!head || !head->next) return head;

    Node *sorted = NULL;
    Node *cur    = head;

    while (cur) {
        Node *next = cur->next;
        if (!sorted || cur->data.marks > sorted->data.marks) {
            cur->next = sorted;
            sorted = cur;
        } else {
            Node *s = sorted;
            while (s->next && cur->data.marks <= s->next->data.marks)
                s = s->next;
            cur->next = s->next;
            s->next = cur;
        }
        cur = next;
    }
    return sorted;
}

/* ================================================================
 *  Count number of students
 * ================================================================ */
int count_students(Node *head) {
    int c = 0;
    while (head) { c++; head = head->next; }
    return c;
}

/* ================================================================
 *  Search by partial name (case-insensitive)
 * ================================================================ */
void search_by_name(Node *head, const char *partial) {
    if (!head) {
        printf("  (No students to search.)\n");
        return;
    }

    /* Lower-case copy of partial */
    char lower_partial[MAX_NAME];
    size_t len = strlen(partial);
    for (size_t i = 0; i < len; i++)
        lower_partial[i] = (char)tolower((unsigned char)partial[i]);
    lower_partial[len] = '\0';

    int found = 0;
    printf("\n  %-6s %-20s %-5s %-7s %-5s\n",
           "Roll", "Name", "Age", "Marks", "Grade");
    printf("  %-6s %-20s %-5s %-7s %-5s\n",
           "------", "--------------------", "---", "-------", "-----");

    Node *cur = head;
    while (cur) {
        /* lower-case copy of name */
        char lower_name[MAX_NAME];
        size_t nlen = strlen(cur->data.name);
        for (size_t i = 0; i < nlen; i++)
            lower_name[i] = (char)tolower((unsigned char)cur->data.name[i]);
        lower_name[nlen] = '\0';

        if (strstr(lower_name, lower_partial)) {
            printf("  %-6d %-20s %-5d %-7.2f %-5c\n",
                   cur->data.roll, cur->data.name, cur->data.age,
                   cur->data.marks, cur->data.grade);
            found = 1;
        }
        cur = cur->next;
    }

    if (!found)
        printf("  No results for \"%s\".\n", partial);
    printf("\n");
}
