#ifndef CLI_H
#define CLI_H

#include "common.h"

typedef struct opt opt;

void usage(void);
opt *parse_opts(int argc, char *argv[]);
void handle_opts(opt *options);
void teardown(opt *options);

#endif  // CLI_H
