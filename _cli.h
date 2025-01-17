#ifndef _CLI_H
#define _CLI_H

__attribute__((__packed__)) struct opt {
    char *db;
    char *additions;
    char *removals;
    char *updates;
    char *output;
    char *name;
    char *address;
    int hours;
    int get_count;
};

#endif  // CLI_H
