#include <stdio.h>
#include "list.h"

int main()
{
    run_test_1();
    run_test_2();
    run_test_3();
    run_test_4();
    
    return 0;
}

bool run_test_1()
{
    doubly_linked_list list;

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListInit(&list, 6, "logfile1.htm")))
        print_error(err);
    
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 0, 5));
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 1, 0));
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 2, 51));   
    LIST_DUMP(&list);

    printf("%d", list_delete(&list, 2));
    LIST_DUMP(&list);
    printf("%d", list_delete(&list, 1));  
    LIST_DUMP(&list);
    printf("%d", list_delete(&list, 3));  

    LIST_DUMP(&list);
    if ((err = ListDestroy(&list)))
        print_error(err);

    return 0;
}

bool run_test_2()
{
    doubly_linked_list list;

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListInit(&list, 3, "logfile2.htm")))
        print_error(err);
    
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 0, 7));
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 1, 3));
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 2, 5));   

    LIST_DUMP(&list);
    printf("%d", list_delete(&list, 3));  

    LIST_DUMP(&list);
    if ((err = ListDestroy(&list)))
        print_error(err);

    return 0;
}

bool run_test_3()
{
    doubly_linked_list list;

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListInit(&list, 2, "logfile3.htm")))
        print_error(err);
    
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 0, 5));
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 1, 5));
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 2, 8)); 
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 1, 5));
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 4, 30));
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 1, 2));
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 6, -5));  

    LIST_DUMP(&list);
    printf("%d", list_delete(&list, 4));  
    LIST_DUMP(&list);
    printf("%d", list_delete(&list, 3));  
    LIST_DUMP(&list);
    printf("%d", list_delete(&list, 6));  

    LIST_DUMP(&list);
    if ((err = ListDestroy(&list)))
        print_error(err);

    return 0;
}

bool run_test_4()
{
    doubly_linked_list list;

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListInit(&list, 6, "logfile4.htm")))
        print_error(err);
    
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 0, 5));
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 1, 0));
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 2, 51));   
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 3, 43));   
    LIST_DUMP(&list);
    printf("%zd", list_insert(&list, 2, 8));   
    LIST_DUMP(&list);

    printf("%d", list_delete(&list, 2));
    LIST_DUMP(&list);
    printf("%d", list_delete(&list, 1));   

    LIST_DUMP(&list);

    list_linearization(&list);
    LIST_DUMP(&list);

    if ((err = ListDestroy(&list)))
        print_error(err);

    return 0;
}