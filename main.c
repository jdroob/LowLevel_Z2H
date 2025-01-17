#include "record.h"
#include "cli.h"

// #include <gtest/gtest.h>

int main(int argc, char *argv[]) {
    opt *options = parse_opts(argc, argv);
    handle_opts(options);
    teardown(options);

    return EXIT_SUCCESS;
}
