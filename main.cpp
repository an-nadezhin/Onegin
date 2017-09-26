//! @file main.cpp
//! @mainpage
//! @date 26.09.2017
//! @author Andrew Nadezhin
//! Sorting of Onegin by alphabet and

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctype.h>
#include <cassert>

enum {ALPHA_SORT =  1, REVERSE_SORT = 0, MISTAKE = -5};

using namespace std;

int File_Read(FILE *f, char *buffer, long *size);

int Amount_of_string(const char *buffer, const long size);

void Array_of_string(struct str pline[], char *buffer, const long *size);

void File_Output(const int *nlines, struct str pline[], const char *filename);

void Sorting(struct str pline[], const int *nlines, int (*compare)(const struct str *pline1, const struct str *pline2));

int recomp(const struct str *pline1, const struct str *pline2);

int strcmp_helper(const struct str *pline1, const struct str *pline2);

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
    int sort_type = 0;
    if (argc == 3) {
        if (strcmp(argv[2], "-n") == 0)
            sort_type = ALPHA_SORT;
        else
            sort_type = REVERSE_SORT;
    }
    int nlines = 0;
    char *buffer = NULL;
    long size = NULL;
    nlines = File_Read(f, buffer, &size);
    if (nlines < 0) {
        cout << "Error : fileopen error number: " << nlines << endl;
        return nlines;
    }
    struct str *pline = (struct str*) calloc(nlines + 1, sizeof(struct str));
    Array_of_string(pline, buffer, &size);
    Sorting(pline, &nlines, sort_type ? recomp : strcmp_helper);
    File_Output(&nlines, pline, "output.txt");

    free(pline);
    free(buffer);
    pline = NULL;
    buffer = NULL;
    return 0;
}

//---------------------------------------------------------
//! Reading a file into the array of string
//!
//!@param [in] f f-text file
//!@param [out] buffer pointer to the array of string
//!@param [out] size pointer to the size of file
//!
//!@return amount of lines
//!
//!@note in case of error return code of mistakes
//!
//!
//---------------------------------------------------------


int File_Read(FILE *f, char *buffer, long *size) {
    assert(f);
    assert(buffer == NULL);
    assert(size == NULL);
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = (char *) calloc(*size + 1, sizeof(char));
    if (buffer == 0) {
        fprintf(stderr, "error : FILE__FILE : LINE__LINE : problems with allocation for buffer");
        return MISTAKE;
    }

    size_t result = fread(*buffer, 1, *size, f);
    if (result != *size) {
        fprintf(stderr, "error : FILE__FILE : LINE__LINE : problems with reading text from file to buffer");
        return MISTAKE;
    }

    fclose(f);
    int nlines = Amount_of_string(buffer, *size);
    return nlines;
}

//---------------------------------------------------------
//! counting amount of string
//!
//!@param [in] size size of file
//!@param [out] buffer pointer to the array of string
//!
//!@return amount of lines
//!
//---------------------------------------------------------


int Amount_of_string(const char *buffer, const long size) {
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


void Array_of_string(struct str pline[], char *buffer, const long *size) {
    assert(buffer);
    assert(size);
    assert(pline == NULL);//??????????????????????
    int pos = 0;
    int prev = 0;
    int amount = 0;
    for (long i = 0; i < *size; i++) {
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

//-------------------------------------------------------------------------
//! sorting
//!
//!@param [out] pline array of pointer on structures
//!@param [in] nlines amount of strings
//!@param [in] compare pointer on function which choose methods of sorting
//!
//!@return nothing
//!
//-------------------------------------------------------------------------

void Sorting(struct str pline[], const int *nlines, int (*compare)(const struct str *pline1, const struct str *pline2)) {
    assert(pline);
    assert(nlines);
    for (int i = 1; i <= *nlines - 1; i++) {
        for (int j = *nlines - 1; j >= i; j--) {
            if (((*compare)((&pline[j - 1]), (&pline[j]))) > 0) {
                str x = pline[j - 1];
                pline[j - 1] = pline[j];
                pline[j] = x;
            }
        }
    }
}

//-------------------------------------------------------------------
//! function which sort strings from end to begin
//!
//! @param [in]pline1 pointer on the first string which we compare
//! @param [in]pline2 pointer on the second string which we compare
//!
//! @return return 1 if string1 > string2; -1 if string2 > string1; 0 if string1 = string 2;
//!
//-------------------------------------------------------------------

int recomp(const struct str *pline1, const struct str *pline2) {
    assert(pline1);
    assert(pline2);
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

//-------------------------------------------------------------------
//! function which sort strings from begin to end
//!
//! @param [in] pline1 pointer on the first string which we compare
//! @param [in] pline2 pointer on the second string which we compare
//!
//! @return return 1 if string1 > string2; -1 if string2 > string1; 0 if string1 = string 2;
//!
//-------------------------------------------------------------------

int strcmp_helper(const struct str *pline1, const struct str *pline2) {
    assert(pline1);
    assert(pline2);
    return strcmp(pline1->str, pline2->str);
}


//-------------------------------------------------------------------------
//!function which return result of sorting in file
//!
//! @param [in] nlines amount of string
//! @param [in]pline
//! @param [in]filename
//!
//! return file with sorted text
//!
//------------------------------------------------------------------------


void File_Output(const int *nlines, struct str pline[], const char *filename) {
    assert(nlines);
    assert(pline);
    assert(filename);

    FILE *d = fopen(filename, "w");

    for (int i = 0; i < *nlines; i++) {
        fputs(pline[i].str, d);
        fputs("\n", d);
    }
    fclose(d);
}


