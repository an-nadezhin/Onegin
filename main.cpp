#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctype.h>
#include <cassert>

using namespace std;

int File_Open(FILE *f, char **buffer, long *size);

int Amount_of_string(char *buffer, long size);

void Array_of_string(struct str pline[], char *buffer, long size);

void File_Output(int nlines, struct str pline[], const char *filename);

void Sorting(struct str pline[], int nlines, int (*compare)(const struct str *pline1, const struct str *pline2));

int recomp(const struct str *pline1, const struct str *pline2);

int strcmp_new(const struct str *pline1, const struct str *pline2);

struct str {
    int len;
    char *str;
};

int main(int argc, char *argv[]) {

    if (argc > 3) {
        cout << "error : usage: sorting <file_name>" << endl;
        return 1;
    }
    FILE *f = fopen(argv[1], "r");

    if (f == NULL) {
        cout << "error: file" << argv[1] << " not found" << endl;
        return 2;
    }
    assert(f != NULL);
    int choice = 0;
    if (argc == 3) {
        if (strcmp(argv[2], "-n") == 0)
            choice = 1;
        else
            choice = 0;
    }
    int nlines = 0;
    char *buffer = NULL;
    long size = 0;
    nlines = File_Open(f, &buffer, &size);
    if (nlines < 0) {
        cout << "Error : fileopen error number: " << nlines << endl;
        return nlines;
    }
    assert(buffer != NULL);
    struct str pline[nlines];
    Array_of_string(pline, buffer, size);
    Sorting(pline, nlines, choice ? recomp : strcmp_new);
    File_Output(nlines, pline, "output.txt");
    return 0;
}

//---------------------------------------------------------
//! Reading a file into the array of string
//!
//! @param [in] f f-text file
//! @param [out] buffer pointer to the array of string
//! @param [out] size pointer to the size of file
//!
//! @return amount of lines
//!
//! @note in case of error return code of mistakes
//!
//!
//---------------------------------------------------------


int File_Open(FILE *f, char **buffer, long *size) {
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);
    *buffer = (char *) calloc(*size + 1, sizeof(char));
    if (*buffer == 0) {
        fputs("error", stderr);
        return -4;
    }

    size_t result = fread(*buffer, 1, *size, f);
    if (result != *size) {
        fputs("error", stderr);
        return -5;
    }

    fclose(f);
    int nlines = Amount_of_string(*buffer, *size);
    return nlines;
}

//---------------------------------------------------------
//! counting amount of string
//!
//! @param [in] size size of file
//! @param [out] buffer pointer to the array of string
//!
//! @return amount of lines
//!
//---------------------------------------------------------


int Amount_of_string(char *buffer, long size) {
    int memory = 0;
    for (long i = 0; i < size; i++)
        if (buffer[i] == '\n')
            memory++;
    return memory;
}

//------------------------------------------------------------
//!function which make pointer on the string and create array of this pointer
//!
//! @param [out] pline array of pointers on structures
//! @param [in] buffer array of string
//! @param [in] size size of file
//!
//! return nothing
//!
//------------------------------------------------------------


void Array_of_string(struct str pline[], char *buffer, long size) {
    int pos = 0;
    int prev = 0;
    int amount = 0;
    for (long i = 0; i < size; i++) {
        amount++;
        if (buffer[i] == '\r') {
            if (buffer[i + 1] == '\n') {
                buffer[i] = '\0';
                pline[pos].str = buffer + prev;
                pline[pos].len = amount;
                prev = i + 1;
                pos++;
                i++;
                amount = 0;
            }
        }
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
            pline[pos].str = buffer + prev;
            pline[pos].len = amount;
            prev = i + 1;
            pos++;
            amount = 0;
        }
    }
}


void Sorting(struct str pline[], int nlines, int (*compare)(const struct str *pline1, const struct str *pline2)) {
    for (int i = 1; i <= nlines - 1; i++) {
        for (int j = nlines - 1; j >= i; j--) {
            if (((*compare)((&pline[j - 1]), (&pline[j]))) > 0) {
                str x = pline[j - 1];
                pline[j - 1] = pline[j];
                pline[j] = x;
            }
        }
    }
}

int recomp(const struct str *pline1, const struct str *pline2) {
    int l1 = pline1->len - 1;
    int l2 = pline2->len - 1;

    while (!isalpha(pline1->str[l1]) && l1 > 0)
        l1--;
    while (!isalpha(pline2->str[l2]) && l2 > 0)
        l2--;
    while (pline1->str[l1] == pline2->str[l2] && l1 > 0 && l2 > 0) {
        l1--;
        l2--;
    }
    if (l1 == 0 && l2 == 0)
        return 0;
    if ((int) pline1->str[l1] > (int) pline2->str[l2])
        return 1;
    else
        return -1;
}

int strcmp_new(const struct str *pline1, const struct str *pline2) {
    return strcmp(pline1->str, pline2->str);
}


void File_Output(int nlines, struct str pline[], const char *filename) {

    FILE *d = fopen(filename, "w");

    for (int i = 0; i < nlines; i++) {
        fputs(pline[i].str, d);
        fputs("\n", d);
    }
    fclose(d);
}


