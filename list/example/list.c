#include <stdio.h>

#include "../list.h"

typedef struct ListItem ListItem;

typedef struct ListItem {
    int value;
    int count;
    ListItem* next;
} ListItem;

typedef struct {
    int length;
    ListItem* head;
} List;

int main (int argc, char** argv)
{
    printf("__________________List in c__________________\n");

    List* list;
    create_list(list);

    list_push(list, ((ListItem) {
        .value = 1,
        .count = 23
    }));

    ListItem l = {
        .value = 23,
        .count = 2
    };
    list_append(list, l);

    {
        ListItem lop = {
                    .value = 23,
                    .count = 2
            };
    }

    for (int i = 0; i < 10; i++) {
        list_append(list, ((ListItem) {
            .value = 1+i,
            .count = 2+i
        }));
    }

    printf("List length: %d\n", list->length);
    ListItem* item = list->head;
    for (int i = 0; i < list->length; i++) {
        printf("Item[%d]\n", i);
        printf("  Value: %d\n", item->value);
        printf("  Count: %d\n", item->count);
        item = item->next;
    }

    free_list(list);

    printf("_____________________________________________\n");

    return 0;
}
