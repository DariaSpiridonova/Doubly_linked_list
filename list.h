#ifndef LIST
#define LIST

#include <stdio.h>
#include <assert.h>

#define PATH_TO_GRAPHVIZ "../Graphviz/list_graph1"
#define LIST_DUMP(list)\
        ListDump(list,__FILE__, __LINE__)
#define DUMP_PARAMETERS(fp, list)\
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

#define ASSERT(list) \
    assert(list != NULL);\
    assert(list->data != NULL);\
    assert(list->next != NULL);\
    assert(list->prev != NULL);\
    assert(list->capacity > 0);\
    assert(list->size >= 0);\
    assert(list->head >= 0);\
    assert(list->tail >= 0);\
    assert(list->free >= 0);\
    assert(list->head <= list->capacity);\
    assert(list->tail <= list->capacity);\

typedef int used_type;

const used_type CANARY = 43685;
const used_type POIZON = 765418;
const ssize_t INCREASE_IN = 2;

typedef struct
{
    used_type *data;
    ssize_t *next;
    ssize_t *prev;
    ssize_t capacity;
    ssize_t size;
    ssize_t head;
    ssize_t tail;
    ssize_t free;
} doubly_linked_list;

enum List_Errors
{
    NO_LIST_ERROR,
    EMPTY_POINTER_ON_LIST_STRUCTURE,
    EMPTY_POINTER_ON_DATA,
    EMPTY_POINTER_ON_NEXT,
    EMPTY_POINTER_ON_PREV,
    ALLOCATE_MEMORY_ERROR,
    ERROR_IN_CAPACITY,
    TOO_SMALL_HEAD,
    TOO_SMALL_TAIL,
    TOO_SMALL_FREE,
    TOO_MUCH_HEAD,
    TOO_MUCH_TAIL,
    NEXT_FROM_PREV_IS_NOT_CURRENT,
    PREV_FROM_NEXT_IS_NOT_CURRENT,
    TOO_MUCH_FREE,
    NO_CURRENT_ELEMENT,
    ERROR_DURING_OPENING_LOGFILE
};

List_Errors ListInit(doubly_linked_list *list, ssize_t capacity);
List_Errors ListVerify(doubly_linked_list *list);
List_Errors ListDestroy(doubly_linked_list *list);

void ListDump(const doubly_linked_list *list, const char *file, int line);
// dump functions:
void dump_to_console(FILE * fp, const doubly_linked_list *list, const char *file, int line);
void print_content(const char *str, ssize_t initial_el,  ssize_t limitation, ssize_t index, ssize_t index1, const ssize_t * next, const void * buffer);
void dump_to_logfile(const doubly_linked_list *list);
void create_graph(const doubly_linked_list *list);
void print_pointer_on_significant_el(FILE *fp, const char *str, ssize_t el);
void link_with_el(FILE *fp, const char ch, ssize_t el);

bool open_file_success(FILE *fp, const char * file_name);
bool close_files_success(FILE *fp, const char * file_name);

bool print_error(List_Errors err);

ssize_t list_insert(doubly_linked_list *list, ssize_t index, used_type value);
List_Errors list_delete(doubly_linked_list *list, ssize_t index);

#endif