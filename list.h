#ifndef LIST
#define LIST

#include <stdio.h>
#include <assert.h>

#define LIST_DUMP(list)\
        ListDump(list,__FILE__, __LINE__)

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
    assert(list->free <= list->capacity);\

const size_t CANARY = 43685;
const size_t POIZON = 765418;
const size_t INCREASE_IN = 2;

typedef int used_type;

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
    TOO_MUCH_FREE,
    NO_CURRENT_ELEMENT,
    ERROR_DURING_OPENING_LOGFILE
};

List_Errors ListInit(doubly_linked_list *list, ssize_t capacity);
List_Errors ListVerify(doubly_linked_list *list);
List_Errors ListDestroy(doubly_linked_list *list);
void ListDump(doubly_linked_list *list, const char *file, int line);
bool open_file_success(FILE *fp);
bool print_error(List_Errors err);

ssize_t list_insert(doubly_linked_list *list, ssize_t index, used_type value);
List_Errors list_delete(doubly_linked_list *list, ssize_t index);

#endif