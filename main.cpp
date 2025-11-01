#include <stdio.h>
#include "list.h"

int main()
{
    doubly_linked_list list;

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListInit(&list, 6)))
        print_error(err);
    
    printf("%zd", list_insert(&list, 0, 5));
    printf("%zd", list_insert(&list, 1, 5));
    printf("%zd", list_insert(&list, 2, 5));
    printf("%zd", list_insert(&list, 1, 5));
    printf("%zd", list_insert(&list, 1, 5));
    printf("%zd", list_insert(&list, 1, 5));   
    printf("%zd", list_insert(&list, 1, 5));   

    printf("%d", list_delete(&list, 2));

    LIST_DUMP(&list);
    if ((err = ListDestroy(&list)))
        print_error(err);

    return 0;
}