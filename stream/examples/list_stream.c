#include "../stream.h"
#include "../../list/list.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct ListItem ListItem;

typedef struct ListItem {
    int value;
    ListItem* next;
} ListItem;

typedef struct {
    int length;
    ListItem* head;
} List;

typedef struct {
    ListItem* data;
    DefineStream;
} Stream;

void print_stream(Stream* stream, ListItem* each)
{
    printf("Stream item: %d\n", each->value);
}
bool filter_list(Stream* stream, ListItem* each)
{
    return each->value % 2 == 0;
}
void print_list(List* list)
{
    ListItem* item = list->head;
    printf("List: ");
    for (int i = 0; i < list->length; i++) {
        printf("%d", item->value);
        if (item->next) {
            printf(" -> ");
            item = item->next;
        } else {
            printf(" -> NULL");
        }
    }
    printf("\n");
}

int main(int argc, char** argv)
{
    List* list = NULL;
    create_list(list);

    for (int i = 0; i < 10; i++) {
        list_push(list, (ListItem) { .value = i + 1 });
    }

    ListItem* data = NULL;
    list_to_array(list, data);

    Stream* stream = NULL;
    create_stream(stream, list->length, data);

    filter(stream, filter_list);
    for_each(stream, print_stream);
    collect(stream);

    print_list(list);

    close_stream(stream);
    free_list_array(data);
    free_list(list);

    return 0;
}
