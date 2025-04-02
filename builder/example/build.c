#define BUILD_IMPLEMENTATION
#include "../builder.h"

int main(int argc, char** argv)
{
    Builder builder = { .output_dir = "./" };

    builder_add_executable(&builder, "main.out");
    builder_add_source_file(&builder, "source/main.c");
	builder_build(&builder);
    builder_free(&builder);

    return 0;
}