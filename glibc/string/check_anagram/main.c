
// gcc -o main main.c
// ./main test tset

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defining comparator function as per the requirement
static int char_compare(const void* a, const void* b)
{
    return strcmp((char*)a, (char*)b);
}

// Function to sort the array
void sort(const char* query, int n)
{
    // calling qsort function to sort the array
    // with the help of Comparator
    qsort((void*)query, n, sizeof(const char), char_compare);
}


// is_anagram returns 1  fi the given word and query is the anagram
int is_anagram(char* word, char* query) {
    sort(word, strlen(word));
    sort(query, strlen(query));

    printf("sort: %s, %s\n", word, query);

    if (strcmp(word, query) == 0) {
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    int ret;

    ret = is_anagram(argv[1], argv[2]);
    if (ret == 1) {
        printf("This is anagram\n");
    }
    else {
        printf("This is not anagram\n");
    }
}
