#include <stdio.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/TestDB.h>
#include <CUnit/CUError.h>

#include "test_queue.h"

int main(int argc, char **argv)
{
    CU_ErrorCode result;
    CU_BasicRunMode run_mode = CU_BRM_VERBOSE;
    
    if (CU_initialize_registry() != CUE_SUCCESS) {
        fprintf(stderr, "E: test framework init faild\n");
        return -2;
    }

    /* Test suites */
    t_init_queue_tests();
    
    CU_basic_set_mode(run_mode);
    result = CU_basic_run_tests();
    CU_cleanup_registry();
    return result;
}