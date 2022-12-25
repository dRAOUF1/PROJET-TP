#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE_LEN 50
#define MAX_CONTENT_LEN 1000

// Struct to represent a chapter
struct Chapter
{
    int id;
    char title[MAX_TITLE_LEN];
    int num_pages;
    char content[MAX_CONTENT_LEN];
    struct Chapter *next;
};
