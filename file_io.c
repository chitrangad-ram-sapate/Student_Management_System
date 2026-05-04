#include <stdio.h>
#include <stdlib.h>
#include "student.h"


// Save entire linked list to binary file
 
int save_to_file(Node *head, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "ERROR: Cannot open %s for writing.\n", filename);
        return 0;
    }

    int count = count_students(head);
    //write record count first 
    fwrite(&count, sizeof(int), 1, fp);

    Node *cur = head;
    while (cur) {
        fwrite(&cur->data, sizeof(Student), 1, fp);
        cur = cur->next;
    }

    fclose(fp);
    printf("  Saved %d record(s) to %s\n", count, filename);
    return 1;
}


//Load linked list from binary file — returns head
 
Node* load_from_file(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return NULL;   //no file yet — not an error 

    int count = 0;
    if (fread(&count, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return NULL;
    }

    Node *head = NULL;
    for (int i = 0; i < count; i++) {
        Student s;
        if (fread(&s, sizeof(Student), 1, fp) != 1) {
            fprintf(stderr, "WARNING: Corrupted file — stopped at record %d.\n", i);
            break;
        }
        head = add_student(head, s);
    }

    fclose(fp);
    printf("  Loaded %d record(s) from %s\n", count, filename);
    return head;
}


 //Export all records to a readable excel file
 
int export_to_csv(Node *head, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "ERROR: Cannot open %s for writing.\n", filename);
        return 0;
    }

    fprintf(fp, "%-6s %-20s %-5s %-7s %-5s\n",
            "Roll", "Name", "Age", "Marks", "Grade");
    fprintf(fp, "%-6s %-20s %-5s %-7s %-5s\n",
            "------", "--------------------", "---", "-------", "-----");

    Node *cur = head;
    while (cur) {
        fprintf(fp, "%-6d %-20s %-5d %-7.2f %-5c\n",
                cur->data.roll, cur->data.name, cur->data.age,
                cur->data.marks, cur->data.grade);
        cur = cur->next;
    }

    fprintf(fp, "\nTotal students: %d\n", count_students(head));
    fclose(fp);
    printf("  Exported to %s\n", filename);
    return 1;
}
