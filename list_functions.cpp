#include <stdlib.h>
#include <string.h>
#include "list.h"

List_Errors ListInit(doubly_linked_list *list, ssize_t capacity, const char *logfile_name)
{
    assert(list != NULL);
    assert(capacity > 0);

    list->data = (used_type *)calloc((size_t)capacity + 1, sizeof(used_type));
    if (list->data == NULL) 
        return EMPTY_POINTER_ON_DATA;

    list->next = (ssize_t *)  calloc((size_t)capacity + 1, sizeof(ssize_t));
    if (list->next == NULL)
    {
        free(list->data);
        return EMPTY_POINTER_ON_NEXT;
    }

    list->prev = (ssize_t *)  calloc((size_t)capacity + 1, sizeof(ssize_t));
    if (list->prev == NULL) 
    {
        free(list->data);
        free(list->next);
        return EMPTY_POINTER_ON_PREV;
    }

    list->head = 0;
    list->tail = 0;
    list->free = 1;
    list->size = 0;
    list->capacity = capacity;

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListVerify(list)))
    {
        LIST_DUMP(list, logfile_name);
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
        LIST_DUMP(list, logfile_name);
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
    for (ssize_t i = list->head; i != 0; i = list->next[i])
    {
        if (list->next[list->prev[i]] != i)
        {
            printf("After element with index %zd follows element with index %zd, and not %zd", list->prev[i], list->next[list->prev[i]], i);
            return NEXT_FROM_PREV_IS_NOT_CURRENT;
        }
        if (list->prev[list->next[i]] != i) 
        {
            printf("the element with index %zd is preceded by with index %zd, and not %zd", list->next[i], list->prev[list->next[i]], i);
            return PREV_FROM_NEXT_IS_NOT_CURRENT;
        }
    }

    ASSERT(list);

    return NO_LIST_ERROR;
}

// TODO: Logfile with list image evolution + text description
// FIXME: !make list work!
// TODO: add more examples

List_Errors ListDestroy(doubly_linked_list *list, const char *logfile_name)
{
    ASSERT(list);

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListVerify(list)))
    {
        LIST_DUMP(list, logfile_name);
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

void ListDump(const doubly_linked_list *list, const char *file, int line, const char *logfile_name)
{
    ASSERT(list);

    dump_to_console(stdin, list, file, line);

    time_t rawtime;      
    struct tm *timeinfo; 
    char buffer[80];

    // Get the current calendar time
    time(&rawtime);

    // Convert the calendar time to local time
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%Y_%m_%d_%H_%M_%S", timeinfo);

    // Print the formatted time string
    printf("Current time: %s\n", buffer);

    char gvfile_name[SIZE_OF_NAME] = {0};
    sprintf(gvfile_name, "%s%s%s.gv", link_to_graphviz_file, logfile_name, buffer);

    dump_to_logfile(list, logfile_name, gvfile_name);

    create_graph(list, gvfile_name);

    ASSERT(list);
}

void dump_parameters(FILE * fp, const doubly_linked_list *list)
{
    fprintf(fp, "LIST[%p]\n", list);\
    fprintf(fp, "LIST_DATA[%p]\n", list->data);\
    fprintf(fp, "LIST_NEXT[%p]\n", list->next);\
    fprintf(fp, "LIST_PREV[%p]\n\n", list->prev);\
    fprintf(fp, "{\n");\
    fprintf(fp, "    capacity = %ld\n", list->capacity);\
    fprintf(fp, "    size     = %ld\n", list->size);\
    fprintf(fp, "    head     = %ld\n", list->head);\
    fprintf(fp, "    tail     = %ld\n", list->tail);\
    fprintf(fp, "    free     = %ld\n", list->free);\
    fprintf(fp, "    {\n");
}

void dump_to_console(FILE * fp, const doubly_linked_list *list, const char *file, int line)
{
    printf("ListDump called from %s :%d\n", file, line);
    dump_parameters(fp, list);

    printf("    capacity = %ld\n", list->capacity);
    printf("    size     = %ld\n", list->size);
    printf("    head     = %ld\n", list->head);
    printf("    tail     = %ld\n", list->tail);
    printf("    free     = %ld\n", list->free);
    print_content("List", list->head, 0, 0, -1, list->next, list->data);
    print_content("Next", 0, list->capacity + 1, 1, 1, list->next, list->next);
    print_content("Prev", 0, list->capacity  + 1, 1, 1, list->next, list->prev);
    
    printf("    }\n");
    printf("}\n");
}

void print_content(const char *str, ssize_t initial_el,  ssize_t limitation, ssize_t index, ssize_t index1, const ssize_t * next, const void * buffer)
{
    printf("        %s contents:\n", str);
    for (ssize_t i = initial_el; i * index1 < limitation; i = 1 * index + i * index + (index == 0) * next[i])
    {
        printf("            [%zd] = %d\n", i, (index == 0) ? ((int *)buffer)[i] : ((ssize_t *)buffer)[i]);
    }
}

void dump_to_logfile(const doubly_linked_list *list, const char *logfile_name, const char *gvfile_name)
{
    FILE *fp = fopen(logfile_name, "a");

    if (!open_file_success(fp, logfile_name))
    {
        return;
    }

    fprintf(fp, "<pre>\n");
    dump_parameters(stdin, list);
    fprintf(fp, "        List contents:\n");
    for (ssize_t i = 1; i < list->capacity + 1; i++)
    {
        fprintf(fp, "          data index = %ld\n", i);
        fprintf(fp, "          element = %d\n", list->data[i]);
        fprintf(fp, "          next = %ld\n", list->next[i]);
        fprintf(fp, "          prev = %ld\n\n\n", list->prev[i]);
    }
    fprintf(fp, "    }\n");
    fprintf(fp, "}\n");

    fprintf(fp, "<img src=\"%*s.png\" alt=\"Graphviz image\" width=\"1800\">", (int)strlen(gvfile_name) - 3, gvfile_name);

    if (!close_files_success(fp, logfile_name))
    {
        return;
    }
}

void create_graph(const doubly_linked_list *list, const char *gvfile_name)
{
    FILE *fp = fopen(gvfile_name, "w");

    if (!open_file_success(fp, gvfile_name))
    {
        return;
    }

    fprintf(fp, "digraph {\n");
    fprintf(fp, "   rankdir=LR;\n");
    fprintf(fp, "   node[shape=\"record\", style=\"filled\", fillcolor=\"grey\", fontsize=20, pin = true];\n");
    fprintf(fp, "splines = ortho;\n");
    fprintf(fp, "rank = same;\n");
    fprintf(fp, "bgcolor=\"LightBlue\";\n");
    fprintf(fp, "invis [shape = record, style = \"invis\", height = 6 , pos = \"2.0, 0.0!\"]");
    fprintf(fp, "   \"0\" [label = \"head = %ld | tail = %ld | free = %ld\", fillcolor = \"yellow\", pos = \"0.0, 0.0!\"];\n", list->head, list->tail, list->free);
    for (ssize_t i = 1; i < list->capacity + 1; i++)
    {
        printf("next = %zd\n", list->next[i]);
        fprintf(fp, "   \"%zd\" [label = \"data index = %zd | element = %d | {prev = %zd | next = %zd}\", fillcolor = \"aqua\", pos = \"%zd.0, 0.0!\"];\n", i, i, list->data[i], list->prev[i], list->next[i], 4 * i);
    }
    print_pointer_on_significant_el(fp, "head", list->head);
    print_pointer_on_significant_el(fp, "tail", list->tail);
    print_pointer_on_significant_el(fp, "free", list->free);

    fprintf(fp, "   \"0\" -> \"1\"");
    for (ssize_t i = 2; i < list->capacity + 1; i++)
    {
        fprintf(fp, "-> \"%zd\"", i);
    }
    fprintf(fp, "[style = invis];\n");
    fprintf(fp, "   ");

    fprintf(fp, "\"%zd\"", list->head);
    for (ssize_t i = list->next[list->head]; i != 0; i = list->next[i])
    {
        fprintf(fp, " -> \"%zd\"", i);
    }
    fprintf(fp, "[style = \"\", color = \"black\"];\n");

    link_with_el(fp, 'h', list->head);
    link_with_el(fp, 't', list->tail);
    link_with_el(fp, 'f', list->free);
    
    fprintf(fp, "\"%zd\"", list->tail);
    for (ssize_t i = list->prev[list->tail]; i != 0; i = list->prev[i])
    {
        fprintf(fp, " -> \"%zd\"", i);
    }
    fprintf(fp, "[style = \"\", color = \"red\"];\n");

    fprintf(fp, "}");
 
    if (!close_files_success(fp, gvfile_name))
    {
        return;
    }

    char command[SIZE_OF_NAME * 2] = {0};
    printf("gvfile_name = %s, gvfile_name = %*s\n", gvfile_name, (int)strlen(gvfile_name) - 3, gvfile_name);
    sprintf(command, "dot -Kfdp %s -Tpng -o %*s.png", gvfile_name, (int)strlen(gvfile_name) - 3, gvfile_name);
    
    system(command);
}

char *get_current_time()
{
    time_t rawtime;      
    struct tm *timeinfo; 
    char buffer[80];

    // Get the current calendar time
    time(&rawtime);

    // Convert the calendar time to local time
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%Y_%m_%d_%H_%M_%S", timeinfo);

    // Print the formatted time string
    printf("Current time: %s\n", buffer);

    return buffer;
}

void print_pointer_on_significant_el(FILE *fp, const char *str, ssize_t el)
{
    fprintf(fp, "\"%c\" [label = \"%s\", fillcolor = \"Pink\", pos = \"%zd.0, 3.5!\", height = 1, width = 1];\n", str[0], str, el * 4);
}

void link_with_el(FILE *fp, const char ch, ssize_t el)
{
    fprintf(fp, "\"%c\" -> \"%zd\";\n", ch, el);
}

bool open_file_success(FILE *fp, const char * file_name)
{
    if (fp == NULL)
    {
        printf("An error occurred when opening the file %s", file_name);
        return false;
    }

    return true;
}

bool close_files_success(FILE *fp, const char * file_name)
{
    if (fclose(fp))
    {
        printf("An error occurred when closing the file %s", file_name);
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

        case NEXT_FROM_PREV_IS_NOT_CURRENT:
            printf("Next element from prev element is not current");
            return true;

        case PREV_FROM_NEXT_IS_NOT_CURRENT:
            printf("Prev element from next element is not current");
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