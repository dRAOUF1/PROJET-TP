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


// Linked list to store the chapters of the book
struct ChapterList
{
    struct Chapter *head;
    struct Chapter *tail;
};

// Function to create a new chapter
struct Chapter *create_chapter(int id, char *title, int num_pages, char *content)
{
    struct Chapter *new_chapter = malloc(sizeof(struct Chapter));
    new_chapter->id = id;
    strcpy(new_chapter->title, title);
    new_chapter->num_pages = num_pages;
    strcpy(new_chapter->content, content);
    new_chapter->next = NULL;
    return new_chapter;
}

// Function to add a chapter to the linked list
void add_chapter(struct ChapterList *book, struct Chapter *new_chapter)
{
    if (book->head == NULL)
    {
        // If the list is empty, set the head and tail pointers to the new chapter
        book->head = new_chapter;
        book->tail = new_chapter;
    }
    else
    {
        // Otherwise, add the new chapter to the end of the list
        book->tail->next = new_chapter;
        book->tail = new_chapter;
    }
}

// Recursive function to display the chapters in the linked list in order
void display_chapters(struct Chapter *current)
{
    if (current == NULL)
    {
        // Base case: reached the end of the list, so stop
        return;
    }
    // Print the current chapter
    printf("Chapter %d: %s\n", current->id, current->title);
    // Recursively call the function for the next chapter
    display_chapters(current->next);
}

// Recursive function to delete a chapter from the linked list
struct Chapter *delete_chapter(struct Chapter *current, int id)
{
    if (current == NULL)
    {
        // Base case: reached the end of the list, so return NULL
        return NULL;
    }
    if (current->id == id)
    {
        // Found the chapter to delete, so remove it from the list
        struct Chapter *temp = current->next;
        free(current);
        return temp;
    }
    // Chapter to delete not found yet, so keep searching
    current->next = delete_chapter(current->next, id);
    return current;
}

// Recursive function to modify the content of a chapter in the linked list
void modify_chapter(struct Chapter *current, int id, char *new_content)
{
    if (current == NULL)
    {
        // Base case: reached the end of the list, so stop
        return;
    }
    if (current->id == id)
    {
        // Found the chapter to modify, so update its content
        strcpy(current->content, new_content);
    }
    else
    {
        // Chapter to modify not found yet, so keep searching
        modify_chapter(current->next, id, new_content);
    }
}

// Recursive function to count the total number of pages in the linked list
int count_pages(struct Chapter *current, int total_pages)
{
    if (current == NULL)
    {
        // Base case: reached the end of the list, so return the total number of pages
        return total_pages;
    }
    // Add the number of pages in the current chapter to the total
    total_pages += current->num_pages;
    // Recursively call the function for the next chapter
    return count_pages(current->next, total_pages);
}

// Recursive function to find the chapter with the maximum and minimum number of pages
void find_max_min_chapters(struct Chapter *current, struct Chapter **max_chapter, struct Chapter **min_chapter)
{
    if (current == NULL)
    {
        // Base case: reached the end of the list, so stop
        return;
    }
    if (*max_chapter == NULL || current->num_pages > (*max_chapter)->num_pages)
    {
        // Current chapter has more pages than the current maximum, so update the maximum
        *max_chapter = current;
    }
    if (*min_chapter == NULL || current->num_pages < (*min_chapter)->num_pages)
    {
        // Current chapter has fewer pages than the current minimum, so update the minimum
        *min_chapter = current;
    }
    // Recursively call the function for the next chapter
    find_max_min_chapters(current->next, max_chapter, min_chapter);
}

int main(void)
{
    // Create an empty linked list to store the chapters of the book
    struct ChapterList book = {NULL, NULL};

    // Add some chapters to the list
    add_chapter(&book, create_chapter(1, "Chapter 1", 20, "Content of chapter 1"));
    add_chapter(&book, create_chapter(2, "Chapter 2", 30, "Content of chapter 2"));
    add_chapter(&book, create_chapter(3, "Chapter 3", 40, "Content of chapter 3"));

    // Display the chapters of the book
    printf("Chapters in the book:\n");
    display_chapters(book.head);

    // Delete the second chapter
    book.head = delete_chapter(book.head, 2);

    // Display the chapters of the book again
    printf("\nChapters in the book after deleting chapter 2:\n");
    display_chapters(book.head);

    // Modify the content of the first chapter
    modify_chapter(book.head, 1, "New content of chapter 1");

    // Display the chapters of the book again
    printf("\nChapters in the book after modifying chapter 1:\n");
    display_chapters(book.head);

    // Count the total number of pages in the book
    int total_pages = count_pages(book.head, 0);
    printf("\nTotal number of pages is %d", total_pages);
    // Find the chapter with the maximum and minimum number of pages
    struct Chapter *max_chapter = NULL;
    struct Chapter *min_chapter = NULL;
    find_max_min_chapters(book.head, &max_chapter, &min_chapter);

    // Print the information about the maximum and minimum chapters
    printf("\nChapter with maximum number of pages: %d pages (%s)\n", max_chapter->num_pages, max_chapter->title);
    printf("Chapter with minimum number of pages: %d pages (%s)\n", min_chapter->num_pages, min_chapter->title);

    return 0;
}
