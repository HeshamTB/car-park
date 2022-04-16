
#include <CUnit/CUnit.h>

#define INIT_CAR(car_num)      \
    car##car_num = (Car) {     \
        .atm = time(NULL),     \
        .cid = 4,              \
    };                         \
    sprintf(car##car_num.pn, "Car number %d", car_num);

#define QUEUE_TEST(s,t)                                        \
    if (!CU_ADD_TEST(s,t)) {                                   \
        fprintf(stderr, "W: cannot add array queue test (%s)", \
        CU_get_error_msg()); \
    }   

CU_pSuite t_init_queue_tests(void);