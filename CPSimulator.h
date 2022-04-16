

extern int oc, nm, psize;
extern long nc, pk, rf, sqw, spt;

/**
    Print help in case of incorrect usage
    @author Hesham T. Banafa
    @date Apr 16th, 2022
*/
void usage();

/**
    Read in args from argv
    @author Hesham T. Banafa
    @date Apr 17th, 2022
*/
static void process_args(char *argv[], int argc, int *in_val, int *out_val, int *qsize, double *exp_cars);
