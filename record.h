#ifndef RECORDS_H
#define RECORDS_H

#include "common.h"

typedef struct record record;
typedef struct record_list record_list;
typedef unsigned int hours_t;

record_list *create_record_list(void);
record_list *read_records(char *filename);
void write_records(char *filename, record_list *records);
record_list *add_record(char *firstname, char *lastname, char *address, hours_t hrs, record_list *records);
record_list *add_records(record_list *additions, record_list *records);
record_list *remove_records(record_list *removals, record_list *records);
record_list *update_records(char *filename, record_list *records);
// record_list *update_hours(record_list *records);
// record_list *update_hours_special(char *name, char *address, hours_t hours, record_list *records);
// record_list *update_name(char *old_name, char *new_name, char *address, record_list *records);
// record_list *update_address(char *old_addr, char *new_addr, char *name, record_list *records);
record_list *find_record_full(char *first, char *last, char *address, hours_t hours, record_list *records);
record_list *find_record_fname(char *firstname, record_list *records);
record_list *find_record_lname(char *lastname, record_list *records);
record_list *find_record_addr(char *address, record_list *records);
record_list *find_record_hrs(hours_t hrs, record_list *records);
void get_count(record_list *records);
void print_records(record_list *records);
void free_recs(record_list *records);

#endif // RECORDS_H
