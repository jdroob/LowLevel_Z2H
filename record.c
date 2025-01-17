#include "record.h"
#include "_record.h"

static void clean_up(record_list *records) {
    for (unsigned i=0; i<records->count; ++i) {
        free(records->records[i].firstname);
        free(records->records[i].lastname);
        free(records->records[i].addr);
    }
    free(records->records);
    free(records);
}

static void init_records(unsigned start, unsigned end, record_list *records) {
    for (unsigned i=start; i<end; ++i) {
        records->records[i].firstname = NULL;
        records->records[i].lastname = NULL;
        records->records[i].addr = NULL;
        records->records[i].hours = 0;
    }
}

static void grow_records(record_list *records) {
    record *tmp = (record*)realloc(records->records, (records->max_size * 2) * sizeof(record));
    if (!tmp) {
        fprintf(stderr, "Error! Unable to grow record list!\n");
        clean_up(records);
        exit(EXIT_FAILURE);
    }
    records->max_size = records->max_size * 2;
    records->records = tmp;
    init_records(records->count, records->max_size, records);
}

static int match(record a, record b) {
    return (((strcmp(a.firstname, b.firstname) == 0 &&
              strcmp(a.lastname, b.lastname) == 0) &&
            strcmp(a.addr, b.addr) == 0) &&
            a.hours == b.hours);
}

record_list *create_record_list(void) {
    record_list *records = (record_list*)malloc(sizeof(record_list));
    if (!records) {
        fprintf(stderr, "Error! Unable to allocate record list\n");
        exit(EXIT_FAILURE);
    }
    records->records = (record*)malloc(sizeof(struct record) * INIT_SIZE);
    if (!(records->records)) {
        fprintf(stderr, "Error! Unable to allocate record array\n");
        exit(EXIT_FAILURE);
    }
    records->count = 0;
    records->max_size = INIT_SIZE;
    init_records(0, INIT_SIZE, records);
    return records;
}

record_list *add_record(char *firstname, char *lastname, char *address, hours_t hrs, record_list *records) {
    if (records->count == records->max_size) {
        grow_records(records);
    }

    size_t str_size = strlen(firstname);
    records->records[records->count].firstname = (char*)malloc(str_size+1);
    if (!(records->records[records->count].firstname)) {
        fprintf(stderr, "Error! Unable to allocate record firstname\n");
        free(records);
        exit(EXIT_FAILURE);
    }
    strncpy(records->records[records->count].firstname, firstname, str_size);
    records->records[records->count].firstname[str_size] = '\0';
    
    str_size = strlen(lastname);
    records->records[records->count].lastname = (char*)malloc(str_size+1);
    if (!(records->records[records->count].lastname)) {
        fprintf(stderr, "Error! Unable to allocate record name\n");
        free(records);
        exit(EXIT_FAILURE);
    }
    strncpy(records->records[records->count].lastname, lastname, str_size);
    records->records[records->count].lastname[str_size] = '\0';

    str_size = strlen(address);
    records->records[records->count].addr = (char*)malloc(str_size+1);
    if (!(records->records[records->count].addr)) {
        fprintf(stderr, "Error! Unable to allocate record address\n");
        free(records);
        exit(EXIT_FAILURE);
    }
    strncpy(records->records[records->count].addr, address, str_size);
    records->records[records->count].addr[str_size] = '\0';

    records->records[records->count].hours = hrs;
    records->count++;

    return records;
}

record_list *add_records(record_list *additions, record_list *records) {
    for (unsigned i=0; i<additions->count; ++i) {
        add_record(additions->records[i].firstname, additions->records[i].lastname, additions->records[i].addr, additions->records[i].hours, records);
    }
    clean_up(additions);
    return records;
}

record_list *find_record_full(char *firstname, char *lastname, char *address, hours_t hours, record_list *records) {
    record_list *found_records = create_record_list();
    for (unsigned i=0; i<records->count; ++i) {
        if (((strcmp(records->records[i].firstname, firstname) == 0 &&
             strcmp(records->records[i].lastname, lastname) == 0) &&
             strcmp(records->records[i].addr, address) == 0) &&
            records->records[i].hours == hours) {
                add_record(firstname, lastname, address, hours, found_records);
        }
    }
    return found_records;
}

record_list *find_record_fname(char *firstname, record_list *records) {
    record_list *found_records = create_record_list();
    for (unsigned i=0; i<records->count; ++i) {
        if (strcmp(records->records[i].firstname, firstname) == 0) {
            add_record(firstname, records->records[i].lastname, records->records[i].addr, \
             records->records[i].hours, found_records);
        }
    }
    return found_records;
}

record_list *find_record_lname(char *lastname, record_list *records) {
    record_list *found_records = create_record_list();
    for (unsigned i=0; i<records->count; ++i) {
        if (strcmp(records->records[i].lastname, lastname) == 0) {
            add_record(records->records[i].firstname, lastname, records->records[i].addr, \
             records->records[i].hours, found_records);
        }
    }
    return found_records;
}

record_list *find_record_addr(char *address, record_list *records) {
    record_list *found_records = create_record_list();
    for (unsigned i=0; i<records->count; ++i) {
        if (strcmp(records->records[i].addr, address) == 0) {
                add_record(records->records[i].firstname, records->records[i].lastname, address, records->records[i].hours, found_records);
        }
    }
    return found_records;
}

record_list *find_record_hrs(hours_t hrs, record_list *records) {
    record_list *found_records = create_record_list();
    for (unsigned i=0; i<records->count; ++i) {
        if (records->records[i].hours == hrs) {
                add_record(records->records[i].firstname, records->records[i].lastname, records->records[i].addr, hrs, found_records);
        }
    }
    return found_records;
}

record_list *remove_records(record_list *removals, record_list *records) {
    /**
     * To keep rest of code simple, whenever one or more records are removed, 
     * a new array of size max_size is alloc'd and all valid
     * entries from previous record array are copied over.
     */
    unsigned remove_count = 0;
    
    // Step 1: Flag records to be removed
    for (unsigned i=0; i<removals->count; ++i) {
        for (unsigned j=0; j<records->count; ++j) {
            if (records->records[j].firstname != NULL &&  
                records->records[j].lastname != NULL &&  
                records->records[j].addr != NULL &&
                records->records[j].hours != -1) {  // if records->records[i] hasn't already been flagged for removal
                if ((strcmp(records->records[j].firstname, removals->records[i].firstname) == 0 &&
                     strcmp(records->records[j].lastname, removals->records[i].lastname) == 0) &&
                     strcmp(records->records[j].addr, removals->records[i].addr) == 0) {   // Removal condition
                        free(records->records[j].firstname); records->records[j].firstname = NULL;
                        free(records->records[j].lastname); records->records[j].lastname = NULL;
                        free(records->records[j].addr); records->records[j].addr = NULL;
                        records->records[j].hours = -1;
                        ++remove_count;
                }
            }
        }
    }
    free_recs(removals);
    records->count -= remove_count;

    // Step 2: Alloc a new records array and copy over non-NULL elements
    record *fresh_array = (record*)malloc(sizeof(record) * records->max_size);
    if (!fresh_array) {
        fprintf(stderr, "Error! Unable to allocate record array\n");
        clean_up(records);
        exit(EXIT_FAILURE);
    }
    unsigned i = 0;
    record *curr = records->records;
    while (i < records->count) {
        if (curr->firstname == NULL || curr->lastname == NULL || curr->addr == NULL || curr->hours == -1) {   // NULL record
            curr++;
        } else {
            fresh_array[i++] = *curr;   // Note to self: derefing a struct just means all the bytes in that struct are accessed (i.e. all members)
            curr++;
        }
    }

    // Step 3: Deallocate records->records
    free(records->records);

    // Step 4: Re-assign records->records
    records->records = fresh_array;
    return records;
}

record_list *update_records(char *filename, record_list *records) {
    // Step 1: Open filename
    FILE *fh = fopen(filename, "r");

    // Step 2: Create 2 record_list objects: old_records, new_records
    record_list *old_records = create_record_list();
    record_list *new_records = create_record_list();

    // Step 3: Read in file format as follows:
    //      <old record>, <new record>
    char old_fname[BUFFSIZE], new_fname[BUFFSIZE];   
    char old_lname[BUFFSIZE], new_lname[BUFFSIZE];   
    char old_addr[BUFFSIZE], new_addr[BUFFSIZE]; 
    unsigned old_hrs, new_hrs;
    int result = fscanf(fh, "%[^,]%*c%[^,]%*c%[^,]%*c%u%*c%[^,]%*c%[^,]%*c%[^,]%*c%u%*c%*c",
                        old_fname, old_lname, old_addr, &old_hrs,
                        new_fname, new_lname, new_addr, &new_hrs);
    while (result != EOF) {
        if (result != 6) {
            fprintf(stderr, "Error! Incorrect input file format: %s", filename);
            exit(EXIT_FAILURE);
        }
        add_record(old_fname, old_lname, old_addr, old_hrs, old_records);
        add_record(new_fname, new_lname, new_addr, new_hrs, new_records);
        result = fscanf(fh, "%[^,]%*c%[^,]%*c%[^,]%*c%u%*c%[^,]%*c%[^,]%*c%[^,]%*c%u%*c%*c",
                        old_fname, old_lname, old_addr, &old_hrs,
                        new_fname, new_lname, new_addr, &new_hrs);
    }

    // Step 4: For each record in records that matches a record in old_records,
    //          update to corresponding record in new_records
    for (unsigned i=0; i<records->count; ++i) {
        for (unsigned j=0; j<old_records->count; ++j) {
            if (match(old_records->records[j], records->records[i])) {
                free(records->records[i].firstname);
                free(records->records[i].lastname);
                free(records->records[i].addr);
                records->records[i].firstname = (char*)malloc(strlen(new_records->records[j].firstname)+1);
                strncpy(records->records[i].firstname, new_records->records[j].firstname, strlen(new_records->records[j].firstname)+1);
                records->records[i].lastname = (char*)malloc(strlen(new_records->records[j].lastname)+1);
                strncpy(records->records[i].lastname, new_records->records[j].lastname, strlen(new_records->records[j].lastname)+1);
                records->records[i].addr = (char*)malloc(strlen(new_records->records[j].addr)+1);
                strncpy(records->records[i].addr, new_records->records[j].addr, strlen(new_records->records[j].addr)+1);
                records->records[i].hours = new_records->records[j].hours;
            }
        }
    }

    // Step 5: Clean up old_records, new_records
    clean_up(old_records);
    clean_up(new_records);

    // Step 6: Close file
    fclose(fh);

    // Step 7: Return records
    return records;
}

// record_list *update_hours_special(char *name, char *address, hours_t hours, record_list *records) {
//     for (unsigned i=0; i<records->count; ++i) {
//         if (strcmp(records->records[i].name, name) == 0 &&
//             strcmp(records->records[i].addr, address) == 0) {
//                 records->records[i].hours = hours;
//         }
//     }
//     return records;
// }

// record_list *update_hours(record_list *records) {
//     for (unsigned i=0; i<records->count; ++i) {
//         records->records[i].hours += HRS_UNIT;
//     }
//     return records;
// }

// record_list *update_name(char *old_name, char *new_name, char *address, record_list *records) {
//     for (unsigned i=0; i<records->count; ++i) {
//         if (strcmp(records->records[i].name, old_name) == 0 &&
//             strcmp(records->records[i].addr, address) == 0) {
//                 free(records->records[i].name);
//                 size_t str_size = strlen(new_name);
//                 records->records[i].name = (char*)malloc(str_size+1);
//                 if (!(records->records[i].name)) {
//                     fprintf(stderr, "Error! Unable to allocate record name\n");
//                     clean_up(records);
//                     exit(EXIT_FAILURE);
//                 }
//                 strcpy(records->records[i].name, new_name);
//                 records->records[i].name[str_size] = '\0';
//         }
//     }
//     return records;
// }

// record_list *update_address(char *old_addr, char *new_addr, char *name, record_list *records) {
//     for (unsigned i=0; i<records->count; ++i) {
//         if (strcmp(records->records[i].name, name) == 0 &&
//             strcmp(records->records[i].addr, old_addr) == 0) {
//                 free(records->records[i].addr);
//                 size_t str_size = strlen(new_addr);
//                 records->records[i].addr = (char*)malloc(str_size+1);
//                 if (!(records->records[i].addr)) {
//                     fprintf(stderr, "Error! Unable to allocate record address\n");
//                     clean_up(records);
//                     exit(EXIT_FAILURE);
//                 }
//                 strcpy(records->records[i].addr, new_addr);
//                 records->records[i].addr[str_size] = '\0';
//         }
//     }
//     return records;
// }

record_list *read_records(char *filename) {
    // Step 1: Open file
    FILE *fh = fopen(filename, "r");
    if (!fh) {
        fprintf(stderr, "Error! Unable to read file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Step 2: Create record list
    record_list *input_records = create_record_list();

    // Step 3: Iterate through lines in file and add records to record list
    char fname[BUFFSIZE], lname[BUFFSIZE], addr[BUFFSIZE];
    unsigned hrs;
    int result = fscanf(fh, "%[^,]%*c%[^,]%*c%[^,]%*c%u%*c%*c", fname, lname, addr, &hrs);
    while (result != EOF) {
        if (result != 3) {
            fprintf(stderr, "Error! Incorrect input file format: %s", filename);
            exit(EXIT_FAILURE);
        }
        add_record(fname, lname, addr, hrs, input_records);
        result = fscanf(fh, "%[^,]%*c%[^,]%*c%[^,]%*c%u%*c%*c", fname, lname, addr, &hrs);
    }

    // Step 4: Close file
    fclose(fh);

    // Step 5: Return pointer to record list
    return input_records;
}

void write_records(char *filename, record_list *records) {
    // Step 1: Open file for writing (if it doesn't exist - create file)
    FILE *fh = fopen(filename, "w");
    if (!fh) {
        fprintf(stderr, "Error! Unable to open file: %s\n", filename);
        clean_up(records);
        exit(EXIT_FAILURE);
    }

    // Step 2: Clear the contents of the file
    fflush(fh);

    // Step 3: Iterate through records list and write each reacord in CSV format
    unsigned i=0;
    for (; i<records->count-1; ++i) {
        fprintf(fh, "%s,%s,%s,%u,\n", records->records[i].firstname, records->records[i].lastname, \
         records->records[i].addr, records->records[i].hours);
    }
    fprintf(fh, "%s,%s,%s,%u,", records->records[i].firstname, records->records[i].lastname, \
     records->records[i].addr, records->records[i].hours);

    // Step 4: Close file
    fclose(fh);
}

void get_count(record_list *records) {
    printf("%d records\n", records->count);
}

void print_records(record_list *records) {
    for (unsigned i=0; i<records->count; ++i) {
        printf("Name: %s %s,\tAddress: %s, Hours: %d\n", records->records[i].firstname, \
        records->records[i].lastname, records->records[i].addr, records->records[i].hours);
    }
}

void free_recs(record_list *records) {
    clean_up(records);
}
