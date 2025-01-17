#include "cli.h"
#include "_cli.h"
#include "record.h"

static void init_opts(opt *options) {
    options->db = NULL;
    options->additions = NULL;
    options->removals = NULL;
    options->updates = NULL;
    options->output = NULL;
    options->firstname = NULL;
    options->lastname = NULL;
    options->address = NULL;
    options->hours = -1;
    options->get_count = 0;
}

static void free_opts(opt *options) {
    free(options->db);
    options->db = NULL;
    free(options->additions);
    options->additions = NULL;
    free(options->removals);
    options->removals = NULL;
    free(options->updates);
    options->updates = NULL;
    free(options->output);
    options->output = NULL;
    free(options->firstname);
    options->firstname = NULL;
    free(options->lastname);
    options->lastname = NULL;
    free(options->address);
    options->address = NULL;
    free(options);
}

void teardown(opt *options) {
    free_opts(options);
}

void usage(void) {
    puts("\nUsage: <executable> -d <input> -a <additions> -r <removals> \
     -u <updates> -ffirstname \"<firstname>\" -flastname \"<lasstname>\" \
     -faddress \"<address>\" -fhours <hours> --count -o <output>\n");
}

opt *parse_opts(int argc, char *argv[]) {
    // Step 1: malloc opts
    opt *options = (opt*)malloc(sizeof(struct opt));
    init_opts(options);

    // Step 2: For each arg to be added to opts
    int i = 1;
    while (i<argc) {
        if ((strcmp(argv[i], "-d") == 0 ||
             strcmp(argv[i], "--database") == 0) && 
             i+1 < argc) {
            options->db = (char*)malloc(strlen(argv[i+1])+1);
            strncpy(options->db, argv[i+1], strlen(argv[i+1])+1);
            options->db[strlen(argv[i+1])] = '\0';
            i += 2;
        } else if ((strcmp(argv[i], "-a") == 0 ||
            strcmp(argv[i], "--additions") == 0) && 
            i+1 < argc) {
            options->additions = (char*)malloc(strlen(argv[i+1])+1);
            strncpy(options->additions, argv[i+1], strlen(argv[i+1])+1);
            options->additions[strlen(argv[i+1])] = '\0';
            i += 2;
        } else if ((strcmp(argv[i], "-r") == 0 ||
            strcmp(argv[i], "--removals") == 0) && 
            i+1 < argc) {
            options->removals = (char*)malloc(strlen(argv[i+1])+1);
            strncpy(options->removals, argv[i+1], strlen(argv[i+1])+1);
            options->removals[strlen(argv[i+1])] = '\0';
            i += 2;
        } else if ((strcmp(argv[i], "-u") == 0 ||
            strcmp(argv[i], "--updates") == 0) && 
            i+1 < argc) {
            options->updates = (char*)malloc(strlen(argv[i+1])+1);
            strncpy(options->updates, argv[i+1], strlen(argv[i+1])+1);
            options->updates[strlen(argv[i+1])] = '\0';
            i += 2;
        } else if ((strcmp(argv[i], "-ffirstname") == 0) &&
            i+1 < argc) {
            options->firstname = (char*)malloc(strlen(argv[i+1])+1);
            strncpy(options->firstname, argv[i+1], strlen(argv[i+1])+1);
            options->firstname[strlen(argv[i+1])] = '\0';
            i += 2;
        } else if ((strcmp(argv[i], "-flastname") == 0) &&
            i+1 < argc) {
            options->lastname = (char*)malloc(strlen(argv[i+1])+1);
            strncpy(options->lastname, argv[i+1], strlen(argv[i+1])+1);
            options->lastname[strlen(argv[i+1])] = '\0';
            i += 2;
        } else if ((strcmp(argv[i], "-faddress") == 0) &&
            i+1 < argc) {
            options->address = (char*)malloc(strlen(argv[i+1])+1);
            strncpy(options->address, argv[i+1], strlen(argv[i+1])+1);
            options->address[strlen(argv[i+1])] = '\0';
            i += 2;
        } else if ((strcmp(argv[i], "-fhours") == 0) && i+1 < argc) {
            options->hours = (unsigned)atoi(argv[i+1]);
            i += 2;
        } else if ((strcmp(argv[i], "-c") == 0) || 
            (strcmp(argv[i], "--count") == 0)) {
            options->get_count = 1;
            i += 1;
        } else if (strcmp(argv[i], "-o") == 0 && i+1 < argc) {
            options->output = (char*)malloc(strlen(argv[i+1])+1);
            strncpy(options->output, argv[i+1], strlen(argv[i+1])+1);
            options->output[strlen(argv[i+1])] = '\0';
            i += 2;
        } else {
            fprintf(stderr, "Error! Invalid options provided\n");
            usage();
            free_opts(options);
            exit(EXIT_FAILURE);
        }
    }

    // Default -d option
    if (!(options->db)) {
        options->db = (char*)malloc(7);
        strncpy(options->db, "db.csv", 7);
        options->db[6] = '\0';
    }
    
    // Default -o option
    if (!(options->output)) {
        options->output = (char*)malloc(11);
        strncpy(options->output, "output.csv", 11);
        options->output[10] = '\0';
    }

    return options;
}

void handle_opts(opt *options) {
    // Step 1: Read in database
    record_list *records = read_records(options->db);

    // Step 2: Read in additions, if any
    if (options->additions) {
        add_records(read_records(options->additions), records);
    }

    // Step 3: Read in removals, if any
    if (options->removals) {
        remove_records(read_records(options->removals), records);
    }

    // Step 4: Read in updates, if any
    if (options->updates) {
        update_records(options->updates, records);
    }

    // Step 5: Handle searches, if any
    if (options->firstname) {
        if (options->lastname) {
            if (options->address) {
                if (options->hours) {   // -ffirstname -flastname -faddress -fhours
                    record_list *found = find_record_full(options->firstname, options->lastname, options->address, options->hours, records);
                    print_records(found);
                    free_recs(found);
                } else {    // -ffirstname -flastname -faddress
                    record_list *tmp = find_record_fname(options->firstname, records);
                    record_list *tmp2 = find_record_lname(options->lastname, tmp);
                    record_list *found = find_record_addr(options->address, tmp2);
                    free_recs(tmp);
                    free_recs(tmp2);
                    print_records(found);
                    free_recs(found);
                }
            } else if (options->hours) {    // -ffirstname -flastname -fhours
                record_list *tmp = find_record_fname(options->firstname, records);
                record_list *tmp2 = find_record_lname(options->lastname, tmp);
                record_list *found = find_record_hrs(options->hours, tmp2);
                free_recs(tmp);
                free_recs(tmp2);
                print_records(found);
                free_recs(found);
            } else {    // -ffirstname -flastname
                record_list *tmp = find_record_fname(options->firstname, records);
                record_list *found = find_record_fname(options->lastname, tmp);
                free_recs(tmp);
                print_records(found);
                free_recs(found);
            }
        } else if (options->address) {
            if (options->hours) {   // -ffirstname -faddress -fhours
                record_list *tmp = find_record_fname(options->firstname, records);
                record_list *tmp2 = find_record_addr(options->address, tmp);
                record_list *found = find_record_hrs(options->hours, tmp2);
                free_recs(tmp);
                free_recs(tmp2);
                print_records(found);
                free_recs(found);
            } else {    // -ffirstname -faddress
                record_list *tmp = find_record_fname(options->firstname, records);
                record_list *found = find_record_addr(options->address, tmp);
                free_recs(tmp);
                print_records(found);
                free_recs(found);
            }
        } else if (options->hours) {    // -ffirstname -fhours
            record_list *tmp = find_record_fname(options->firstname, records);
            record_list *found = find_record_hrs(options->hours, tmp);
            free_recs(tmp);
            print_records(found);
            free_recs(found);
        } else {    // -ffirstname
            record_list *found = find_record_fname(options->firstname, records);
            print_records(found);
            free_recs(found);
        }
    } else if (options->lastname) {
        if (options->address) {
            if (options->hours) {   // -flastname -faddress -fhours
                record_list *tmp = find_record_lname(options->lastname, records);
                record_list *tmp2 = find_record_addr(options->address, tmp);
                record_list *found = find_record_hrs(options->hours, tmp2);
                free_recs(tmp);
                free_recs(tmp2);
                print_records(found);
                free_recs(found);
            } else {    // -flastname -faddress
                record_list *tmp = find_record_lname(options->lastname, records);
                record_list *found = find_record_addr(options->address, tmp);
                free_recs(tmp);
                print_records(found);
                free_recs(found);
            }
        } else if (options->hours) {    // -flastname -fhours
            record_list *tmp = find_record_lname(options->lastname, records);
            record_list *found = find_record_hrs(options->hours, tmp);
            free_recs(tmp);
            print_records(found);
            free_recs(found);
        } else {    // -flastname
            record_list *found = find_record_lname(options->lastname, records);
            print_records(found);
            free_recs(found);
        }
    } else if (options->address) {
        if (options->hours) {   // -faddress -fhours
            record_list *tmp = find_record_addr(options->address, records);
            record_list *found = find_record_hrs(options->hours, tmp);
            free_recs(tmp);
            print_records(found);
            free_recs(found);
        } else {    // -faddress
            record_list *found = find_record_addr(options->address, records);
            print_records(found);
            free_recs(found);
        }
    } else if (options->hours != -1) {  // -fhours
        record_list *found = find_record_hrs(options->hours, records);
        print_records(found);
        free_recs(found);
    }

    // Step 6: See if user requested record count
    if (options->get_count) {
        get_count(records);   
    }

    // Step 7: Write finalized database to file
    write_records(options->output, records);

    // Step 8: Free records
    free_recs(records);
}
