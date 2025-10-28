#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

List_Errors ListInit(doubly_linked_list *list, ssize_t capacity)
{
    assert(list != NULL);
    assert(capacity > 0);

    list->data = (used_type *)calloc((size_t)capacity + 1, sizeof(used_type));
    list->next = (ssize_t *)calloc((size_t)capacity + 1, sizeof(ssize_t));
    list->prev = (ssize_t *)calloc((size_t)capacity + 1, sizeof(ssize_t));

    if (list->data == NULL) return EMPTY_POINTER_ON_DATA;
    if (list->next == NULL) return EMPTY_POINTER_ON_NEXT;
    if (list->prev == NULL) return EMPTY_POINTER_ON_PREV;

    list->head = 0;
    list->tail = 0;
    list->free = 1;
    list->size = 0;
    list->capacity = capacity;

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListVerify(list)))
    {
        LIST_DUMP(list);
        return err;
    }

    list->data[0] = CANARY;
    list->next[0] = 0;
    list->prev[0] = 0;

    for (ssize_t i = 1; i < capacity + 1; i++)
    {
        list->data[i] = POIZON;
        list->next[i] = i + 1;
        list->prev[i] = -1;
    }
    list->next[capacity] = 0;

    if ((err = ListVerify(list)))
    {
        LIST_DUMP(list);
        return err;
    }

    ASSERT(list);

    return err;
}

List_Errors ListVerify(doubly_linked_list *list)
{
    ASSERT(list);

    if (list == NULL)                     return EMPTY_POINTER_ON_LIST_STRUCTURE;
    else if (list->data     == NULL)      return EMPTY_POINTER_ON_DATA;
    else if (list->next     == NULL)      return EMPTY_POINTER_ON_NEXT;
    else if (list->prev     == NULL)      return EMPTY_POINTER_ON_PREV;
    else if (list->capacity <= 0)         return ERROR_IN_CAPACITY;
    else if (list->size < 0)              return ERROR_IN_CAPACITY;
    else if (list->head      < 0)         return TOO_SMALL_HEAD;
    else if (list->tail      < 0)         return TOO_SMALL_TAIL;
    else if (list->free      < 0)         return TOO_SMALL_FREE;
    else if (list->head > list->capacity) return TOO_MUCH_HEAD;
    else if (list->tail > list->capacity) return TOO_MUCH_TAIL;
    else if (list->free > list->capacity) return TOO_MUCH_FREE;

    ASSERT(list);

    return NO_LIST_ERROR;
}

List_Errors ListDestroy(doubly_linked_list *list)
{
    ASSERT(list);

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListVerify(list)))
    {
        LIST_DUMP(list);
        return err;
    }

    printf("FREE\n");

    free(list->data);
    free(list->next);
    free(list->prev);
    list->data = NULL;
    list->next = NULL;
    list->prev = NULL;
    list = NULL;

    return err;
}

// system
void ListDump(doubly_linked_list *list, const char *file, int line)
{
    ASSERT(list);

    printf("ListDump called from %s :%d\n", file, line);
    printf("LIST[%p]\n", list);
    printf("LIST_DATA[%p]\n", list->data);
    printf("LIST_NEXT[%p]\n", list->next);
    printf("LIST_PREV[%p]\n", list->prev);
    printf("{\n");
    printf("    capacity = %ld\n", list->capacity);
    printf("    size     = %ld\n", list->size);
    printf("    head     = %ld\n", list->head);
    printf("    tail     = %ld\n", list->tail);
    printf("    free     = %ld\n", list->free);
    printf("    {\n");
    printf("        List contents:\n");
    for (ssize_t i = list->head; i != 0; i = list->next[i])
    {
        printf("            [%zd] = %d\n", i, list->data[i]);
    }
    printf("        Next contents:\n");
    for (ssize_t i = 0; i < list->capacity; i++)
    {
        printf("            [%zd] = %d\n", i, list->next[i]);
    }
    printf("        Prev contents:\n");
    for (ssize_t i = 0; i < list->capacity; i++)
    {
        printf("            [%zd] = %d\n", i, list->prev[i]);
    }
    printf("    }\n");
    printf("}\n");

    FILE *logfile = fopen("logfile.htm", "w");

    if (!open_file_success(logfile))
    {
        return;
    }

    fprintf(logfile, "<pre>\n");
    fprintf(logfile, "LIST[%p]\n", list);
    fprintf(logfile, "LIST_DATA[%p]\n", list->data);
    fprintf(logfile, "LIST_NEXT[%p]\n", list->next);
    fprintf(logfile, "LIST_PREV[%p]\n\n", list->prev);
    fprintf(logfile, "{\n");
    fprintf(logfile, "    capacity = %ld\n", list->capacity);
    fprintf(logfile, "    size     = %ld\n\n", list->size);
    fprintf(logfile, "    head     = %ld\n", list->head);
    fprintf(logfile, "    tail     = %ld\n", list->tail);
    fprintf(logfile, "    free     = %ld\n", list->free);
    fprintf(logfile, "    {\n");
    fprintf(logfile, "        List contents:\n");
    for (ssize_t i = 1; i < list->capacity; i++)
    {
        fprintf(logfile, "          data index = %ld\n", i, i);
        fprintf(logfile, "          element = %ld\n", list->data[i]);
        fprintf(logfile, "          next = %ld\n", list->next[i]);
        fprintf(logfile, "          prev = %ld\n\n\n", list->prev[i]);
        //printf("           \033[1m.\033[0m" " [%zd] = %d\n", i, list->data[i]);
    }
    fprintf(logfile, "    }\n");
    fprintf(logfile, "}\n");

    ASSERT(list);
}

bool open_file_success(FILE *fp)
{
    if (fp == NULL)
    {
        printf("An error occurred when opening the file \"logfile.htm\"");
        return false;
    }

    return true;
}
bool print_error(List_Errors err)
{
    switch(err)
    {
        case NO_LIST_ERROR:
            return true;

        case EMPTY_POINTER_ON_LIST_STRUCTURE:
            printf("The pointer on the LIST STRUCTURE is empty");
            return true;

        case EMPTY_POINTER_ON_DATA:
            printf("The pointer to the DATA is empty");
            return true;

        case EMPTY_POINTER_ON_NEXT:
            printf("The pointer to the NEXT is empty");
            return true;

        case EMPTY_POINTER_ON_PREV:
            printf("The pointer to the PREV is empty");
            return true;

        case ERROR_IN_CAPACITY:
            printf("Capacity takes an invalid value");
            return true;

        case ALLOCATE_MEMORY_ERROR:
            printf("Couldn't allocate additional memory");
            return true;

        case TOO_SMALL_HEAD:
            printf("The head value is too small");
            return true;

        case TOO_SMALL_TAIL:
            printf("The tail value is too small");
            return true;

        case TOO_SMALL_FREE:
            printf("The free value is too small");
            return true;

        case TOO_MUCH_HEAD:
            printf("The head value is too much");
            return true;

        case TOO_MUCH_TAIL:
            printf("The tail value is too much");
            return true;

        case TOO_MUCH_FREE:
            printf("The free value is too much");
            return true;

        case NO_CURRENT_ELEMENT:
            printf("It is not possible to add an element after a non-existent index");
            return true;

        case ERROR_DURING_OPENING_LOGFILE:
            printf("An error occurred when opening the file \"logfile.htm\"");
            return true;

        default:
            return false;
    }

    return false;
}