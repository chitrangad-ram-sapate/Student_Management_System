#include <stdio.h>
#include <stdlib.h>
#include "student.h"

/* ================================================================
 *  Save entire linked list to binary file
 * ================================================================ */
int save_to_file(Node *head, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "ERROR: Cannot open %s for writing.\n", filename);
        return 0;
    }

    int count = count_students(head);
    /* write record count first */
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

/* ================================================================
 *  Load linked list from binary file — returns head
 * ================================================================ */
Node* load_from_file(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return NULL;   /* no file yet — not an error */

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

/* ================================================================
 *  Comparator for qsort — sorts Student array by roll (ascending)
 * ================================================================ */
static int cmp_by_roll(const void *a, const void *b) {
    const Student *s1 = (const Student *)a;
    const Student *s2 = (const Student *)b;
    return s1->roll - s2->roll;
}

/* ================================================================
 *  Export all records to a proper CSV file (sorted by roll number)
 *  — Opens cleanly as a table in Excel / Google Sheets
 * ================================================================ */
int export_to_excel(Node *head, const char *filename) {
    int count = count_students(head);

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "ERROR: Cannot open %s for writing.\n", filename);
        return 0;
    }

    /* ---- Write CSV header row ---- */
    fprintf(fp, "Roll,Name,Age,Marks,Grade\n");

    if (count == 0) {
        fclose(fp);
        printf("  Exported 0 records to %s\n", filename);
        return 1;
    }

    /* ---- Copy linked list into a temp array for sorting ---- */
    Student *arr = (Student *)malloc(count * sizeof(Student));
    if (!arr) {
        fprintf(stderr, "ERROR: Memory allocation failed during export.\n");
        fclose(fp);
        return 0;
    }

    Node *cur = head;
    for (int i = 0; i < count; i++, cur = cur->next)
        arr[i] = cur->data;

    /* ---- Sort by roll number (ascending) ---- */
    qsort(arr, count, sizeof(Student), cmp_by_roll);

    /* ---- Write each record as a CSV row ---- */
    for (int i = 0; i < count; i++) {
        /*
         * Wrap the Name field in double-quotes so commas or special
         * characters inside a name never break the column layout.
         */
        fprintf(fp, "%d,\"%s\",%d,%.2f,%c\n",
                arr[i].roll,
                arr[i].name,
                arr[i].age,
                arr[i].marks,
                arr[i].grade);
    }

    free(arr);
    fclose(fp);
    printf("  Exported %d record(s) (sorted by roll) to %s\n", count, filename);
    return 1;
}
