#define ASYNC_FS_IMPLEMENTATION
#include "../../async_fs.h"
#include <stdio.h>

int main()
{
    AsyncState* state = async_init();
    Task* read_file_task = async_read_file_data(state, "test.bin");
    Task* write_file_task = async_write_file_data(state, "output.bin", "Hello", 5);

    if (read_file_task == NULL) {
        printf("FILEIO: File name provided is not valid\n");
        return 1;
    }

    if (write_file_task == NULL) {
        printf("FILEIO: File name provided is not valid\n");
        return 1;
    }

    while (!async_is_finished(state)) {
        async_update(state);
    }

    FsDataDa* resolve = (FsDataDa*)await(read_file_task).resolve;
    printf("File len: %d\n", resolve->length);
    printf("File data: ");
    for (int i = 0; i < resolve->length; i++) {
        printf("%02x", resolve->items[i]);
    }
    printf("\n");

    async_fs_close(read_file_task);
    async_fs_close(write_file_task);
    async_close(state);
    return 0;
}