/**
    Test suite fot Array Queue implementaion
    Make sure that all operations and edge cases
    run correctly
*/
#include "test_queue.h"

#include "../Queue.h"

#include <CUnit/CUError.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestRun.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


static void t_empty_queue(void)
{
    Qinit(5);
    CU_ASSERT_TRUE(QisEmpty());
    CU_ASSERT_EQUAL(Qsize(), 0);
    Qfree();
}

static void t_enqueue_one_then_check_size(void)
{
    Qinit(5);
    CU_ASSERT_TRUE(QisEmpty());
    CU_ASSERT_FALSE(QisFull());
    Car car1;
    car1.atm = 44;
    car1.ltm = 54;
    strcpy(car1.pnf, "test string");
    Qenqueue(&car1);
    CU_ASSERT_EQUAL(Qsize(), 1);
    Qfree();
}

static void t_enqueue_then_serve(void)
{
    Qinit(5);
    Car car1;
    car1.atm = 44;
    car1.ltm = 54;
    strcpy(car1.pnf, "test string");
    Qenqueue(&car1);
    CU_ASSERT_EQUAL(Qsize(), 1);
    Car *car_served = Qserve();
    CU_ASSERT_EQUAL(car_served, &car1);
    CU_ASSERT_EQUAL(Qsize(), 0);
    CU_ASSERT_TRUE(QisEmpty());
    CU_ASSERT_FALSE(QisFull());
    Qfree();
}

static void t_enqueue_to_full(void)
{
    int q_size = 5;
    Car cars[q_size];
    Qinit(q_size);

    for (int i = 0; i < q_size; i++) {
        cars[i].atm = time(NULL);
        cars[i].cid = 4;
        strcpy(cars[i].pn," const char *restrict __src");
        Qenqueue(&cars[i]);
        CU_ASSERT_EQUAL(Qsize(), i + 1); // Check size on each enqueue
    }
    CU_ASSERT_EQUAL(Qsize(), q_size);
    CU_ASSERT_TRUE(QisFull());
    CU_ASSERT_FALSE(QisEmpty());
    Qfree();
}

static void t_serve_correct_order(void)
{
    Qinit(4);
    Car car1, car2, car3, car4;

#define INIT_CAR(car_num)      \
    car##car_num = (Car) {     \
        .atm = time(NULL),     \
        .cid = 4,              \
    };                         \
    sprintf(car##car_num.pn, "Car number %d", car_num);

    INIT_CAR(1);
    INIT_CAR(2);
    INIT_CAR(3);
    INIT_CAR(4);
    Qenqueue(&car1);
    Qenqueue(&car2);
    CU_ASSERT_PTR_EQUAL(Qpeek(), &car1);
    Qenqueue(&car3);
    CU_ASSERT_PTR_EQUAL(Qpeek(), &car1);
    CU_ASSERT_EQUAL(Qsize(), 3);
    
    Car *car_served = Qserve();
    CU_ASSERT_EQUAL(Qsize(), 2);
    CU_ASSERT_PTR_EQUAL(car_served, &car1);

    car_served = Qserve();
    CU_ASSERT_EQUAL(Qsize(), 1);
    CU_ASSERT_PTR_EQUAL(car_served, &car2);

    car_served = Qserve();
    CU_ASSERT_EQUAL(Qsize(), 0);
    CU_ASSERT_PTR_EQUAL(car_served, &car3);
    Qfree();
}

static void t_overfill(void)
{
    Qinit(4);
    Car car1, car2, car3, car4, car5;
    INIT_CAR(1);
    INIT_CAR(2);
    INIT_CAR(3);
    INIT_CAR(4);
    INIT_CAR(5);
    CU_ASSERT_TRUE(QisEmpty());
    Qenqueue(&car1);
    Qenqueue(&car2);
    Qenqueue(&car3);
    CU_ASSERT_FALSE(QisFull());
    Qenqueue(&car4);
    CU_ASSERT_TRUE(QisFull());
    Qenqueue(&car5);
    CU_ASSERT_TRUE(QisFull());
    CU_ASSERT_EQUAL(Qsize(), 4);
    CU_ASSERT_PTR_EQUAL(Qpeek(), &car1);
    Qfree();
}

CU_pSuite t_init_queue_tests(void)
{
    CU_pSuite suite;
    suite = CU_add_suite("array queue", NULL, NULL); 

    if (!suite) {
        fprintf(stderr,
                "W: cannot add array queue test suite (%s)\n",
                CU_get_error_msg());
        return NULL;
    }

    // Do tests
#define QUEUE_TEST(s,t)                                        \
    if (!CU_ADD_TEST(s,t)) {                                   \
        fprintf(stderr, "W: cannot add array queue test (%s)", \
        CU_get_error_msg()); \
    }                       

    QUEUE_TEST(suite, t_empty_queue);
    QUEUE_TEST(suite, t_enqueue_one_then_check_size);
    QUEUE_TEST(suite, t_enqueue_then_serve);
    QUEUE_TEST(suite, t_enqueue_to_full);
    QUEUE_TEST(suite, t_serve_correct_order);
    QUEUE_TEST(suite, t_overfill);
    
    return suite;
}