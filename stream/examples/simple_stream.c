#include "../stream.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    int* data;
    DefineStream;
} Stream;

int* map_items(Stream* stream, int* each)
{
    *each *= *each;
    return each;
}
int* map_items2(Stream* stream, int* each)
{
    *each += 10;
    return each;
}
bool filter_items(Stream* stream, int* each)
{
    return *each % 2 == 0;
}
bool filter_items2(Stream* stream, int* each)
{
    return *each != 26;
}
void print_items(Stream* stream, int* each)
{
    printf("stream data: %d\n", *each);
}

int main(int argc, char** argv)
{
    int data[] = { 1, 7, 2, 3, 4, 5 };
    Stream* stream = NULL;
    create_stream(stream, array_len(data), data);

    filter(stream, filter_items);
    map(stream, map_items);
    map(stream, map_items2);
    // filter(stream, filter_items2);
    for_each(stream, print_items);

    collect(stream);

    for (int i = 0; i < stream->length; i++) {
        printf("stream->data[%d]: %d\n", i, stream->data[i]);
    }

    for (int i = 0; i < array_len(data); i++) {
        printf("data[%d]: %d\n", i, data[i]);
    }

    close_stream(stream);
    return 0;
}
