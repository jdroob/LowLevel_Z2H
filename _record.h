#ifndef _RECORDS_H
#define _RECORDS_H

#define HRS_UNIT 8
#define INIT_SIZE 16
#define BUFFSIZE 512

__attribute__((__packed__)) struct record {
    char *name;
    char *addr;
    unsigned int hours;
};

__attribute__((__packed__)) struct record_list {
    unsigned int count;
    unsigned int max_size;
    record *records;
};

#endif // _RECORDS_H
