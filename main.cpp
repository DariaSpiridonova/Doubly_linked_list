#include <stdio.h>
#include "list.h"

int main()
{
    run_test_1();
    run_test_2();
    run_test_3();
    
    return 0;
}

bool run_test_1()
{
    doubly_linked_list list;

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListInit(&list, 6, "logfile1.htm")))
        print_error(err);
    
    printf("%zd", list_insert(&list, 0, 5, "logfile1.htm"));
    printf("%zd", list_insert(&list, 1, 0, "logfile1.htm"));
    printf("%zd", list_insert(&list, 2, 51, "logfile1.htm"));   

    printf("%d", list_delete(&list, 2, "logfile1.htm"));
    printf("%d", list_delete(&list, 1, "logfile1.htm"));  
    printf("%d", list_delete(&list, 3, "logfile1.htm"));  

    LIST_DUMP(&list, "logfile1.htm");
    if ((err = ListDestroy(&list, "logfile1.htm")))
        print_error(err);

    return 0;
}

bool run_test_2()
{
    doubly_linked_list list;

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListInit(&list, 3, "logfile2.htm")))
        print_error(err);
    
    printf("%zd", list_insert(&list, 0, 7, "logfile2.htm"));
    printf("%zd", list_insert(&list, 1, 3, "logfile2.htm"));
    printf("%zd", list_insert(&list, 2, 5, "logfile2.htm"));   

    printf("%d", list_delete(&list, 3, "logfile2.htm"));  

    LIST_DUMP(&list, "logfile2.htm");
    if ((err = ListDestroy(&list, "logfile2.htm")))
        print_error(err);

    return 0;
}

bool run_test_3()
{
    doubly_linked_list list;

    List_Errors err = NO_LIST_ERROR;

    if ((err = ListInit(&list, 2, "logfile3.htm")))
        print_error(err);
    
    printf("%zd", list_insert(&list, 0, 5, "logfile3.htm"));
    printf("%zd", list_insert(&list, 1, 5, "logfile3.htm"));
    printf("%zd", list_insert(&list, 2, 8, "logfile3.htm")); 
    printf("%zd", list_insert(&list, 1, 5, "logfile3.htm"));
    printf("%zd", list_insert(&list, 4, 30, "logfile3.htm"));
    printf("%zd", list_insert(&list, 1, 2, "logfile3.htm"));
    printf("%zd", list_insert(&list, 6, -5, "logfile3.htm"));  

    printf("%d", list_delete(&list, 4, "logfile3.htm"));  
    printf("%d", list_delete(&list, 3, "logfile3.htm"));  
    printf("%d", list_delete(&list, 6, "logfile3.htm"));  

    LIST_DUMP(&list, "logfile3.htm");
    if ((err = ListDestroy(&list, "logfile3.htm")))
        print_error(err);

    return 0;
}