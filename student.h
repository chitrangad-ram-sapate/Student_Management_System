#ifndef STUDENT_H
#define STUDENT_H

//---------- Constants ---------- 
#define MAX_NAME      100
#define FILENAME      "records.dat"
#define EXPORT_FILE   "records_export.txt"

// ---------- Student Struct ---------- 
typedef struct {
    int   roll;
    char  name[MAX_NAME];
    int   age;
    float marks;
    char  grade;          /* 'A','B','C','D','F' — auto-calculated */
} Student;

//---------- Linked-List Node ----------
typedef struct Node {
    Student     data;
    struct Node *next;
} Node;

// ========== student.c ========== 

/* Create a new node (copies data).  Returns NULL on allocation failure. */
Node* create_node(Student s);

/* Add student to end of list.  Returns new head. */
Node* add_student(Node *head, Student s);

/* Print all students in a formatted table. */
void  print_all(Node *head);

/* Free entire list. */
void  free_list(Node *head);

/* Search by exact roll number.  Returns node pointer or NULL. */
Node* search_by_roll(Node *head, int roll);

/* Delete student by roll.  Returns (possibly new) head. */
Node* delete_student(Node *head, int roll);

/* Update an existing student (matched by roll).  Returns 1 on success, 0 on failure. */
int   update_student(Node *head, int roll, Student new_data);

/* Sort by name (ascending, case-insensitive).  Returns new head. */
Node* sort_by_name(Node *head);

/* Sort by marks (descending).  Returns new head. */
Node* sort_by_marks(Node *head);

/* Count nodes in list. */
int   count_students(Node *head);

/* Search by partial name (case-insensitive).  Prints all matches. */
void  search_by_name(Node *head, const char *partial);

// ========== file_io.c ========== 

/* Save entire linked list to binary file.  Returns 1 on success. */
int   save_to_file(Node *head, const char *filename);

/* Load linked list from binary file.  Returns head of list (NULL if empty or error). */
Node* load_from_file(const char *filename);

/* Export all records to a readable text file.  Returns 1 on success. */
int   export_to_text(Node *head, const char *filename);

/* Auto-compute grade from marks (0‑100). */
char  compute_grade(float marks);

/* Validate student data.  Returns 0 if valid, or negative error code:
 *   -1 = duplicate roll
 *   -2 = empty name
 *   -3 = marks out of range (0‑100)
 *   -4 = age out of range (1‑99)
 *   -5 = roll <= 0
 *
 *  NOTE: duplicate-roll check needs list head + current roll.  Pass head=NULL to skip.
 */
int   validate_student(Node *head, Student s, int is_update);

/* Print statistics report. */
void  print_statistics(Node *head);

/* Safely read an integer from stdin.  Returns 1 on success. */
int   read_int(int *value);

/* Safely read a float from stdin.  Returns 1 on success. */
int   read_float(float *value);

/* Clear input buffer. */
void  clear_input(void);

#endif /* STUDENT_H */
