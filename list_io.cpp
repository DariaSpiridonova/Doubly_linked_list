#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

ssize_t list_insert(doubly_linked_list *list, ssize_t index, used_type value)
{
    ASSERT(list);
    assert(index >= 0);

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListVerify(list)))
    {
        LIST_DUMP(list);
        return err;
    }

    if (list->data[index] == POIZON)
    {
        return NO_CURRENT_ELEMENT;
    }

    if (list->size == list->capacity)
    {
        list->data = (used_type *)realloc(list->data, (INCREASE_IN*(size_t)(list->capacity) + 1)*sizeof(used_type));
        if (list->data == NULL) return ALLOCATE_MEMORY_ERROR;

        list->next = (ssize_t *)realloc(list->next, (INCREASE_IN*(size_t)(list->capacity) + 1)*sizeof(ssize_t));
        if (list->prev == NULL) 
        {
            free(list->data);
            return ALLOCATE_MEMORY_ERROR;
        }

        list->prev = (ssize_t *)realloc(list->prev, (INCREASE_IN*(size_t)(list->capacity) + 1)*sizeof(ssize_t));
        if (list->prev == NULL) 
        {
            free(list->data);
            free(list->next);
            return ALLOCATE_MEMORY_ERROR;
        }
        
        for (ssize_t i = list->capacity + 1; i < INCREASE_IN*list->capacity + 1; i++)
        {
            list->data[i] = POIZON;
            list->next[i] = i + 1;
            list->prev[i] = -1;
        }
        list->capacity *= INCREASE_IN;
        
        err = NO_LIST_ERROR;
        if ((err = ListVerify(list)))
        {
            LIST_DUMP(list);
            return err;
        }
    }

    ssize_t occupied = list->free;
    list->free = list->next[list->free];

    list->data[occupied] = value;
    list->next[occupied] = list->next[index];
    list->prev[occupied] = index;
    list->next[index] = occupied;
    list->prev[list->next[occupied]] = occupied;

    if (list->prev[occupied] == 0)
    {
        list->head = occupied;
        list->next[0] = occupied;
    }

    if (list->next[occupied] == 0) 
    {
        list->tail = occupied;
        list->prev[0] = occupied;
    }
    
    list->size++;

    if ((err = ListVerify(list)))
    {
        LIST_DUMP(list);
        return err;
    }


    return occupied;
}

List_Errors list_delete(doubly_linked_list *list, ssize_t index)
{
    ASSERT(list);
    assert(index >= 0);

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListVerify(list)))
    {
        LIST_DUMP(list);
        return err;
    }

    list->data[index] = POIZON;
    if (list->prev[index] == 0)
    {
        list->head = list->next[index];
        list->next[0] = list->next[index];
    }
    if (list->next[index] == 0)
    {
        list->tail = list->prev[index];
        list->prev[0] = list->prev[index];
    }

    list->prev[list->next[index]] = list->prev[index];
    list->next[list->prev[index]] = list->next[index];
    list->prev[index] = -1;
    list->next[index] = list->free;
    list->free = index;

    list->size--;

    if ((err = ListVerify(list)))
    {
        LIST_DUMP(list);
        return err;
    }

    return err;
}