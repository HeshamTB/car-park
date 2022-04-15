#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>
#include <time.h>

time_t tm;
atomic_long atm_long;
int cnt;

int f(void* thr_data)
{
    (void)thr_data;
    for(int n = 0; n < 1000; ++n) {
        tm += 10;
        atm_long += 10;
        // for this example, relaxed memory order is sufficient, e.g.
        // atomic_fetch_add_explicit(&acnt, 1, memory_order_relaxed);
    }
    return 0;
}

int main(void)
{
    thrd_t thr[10];
    tm = time(NULL);
    atm_long = tm;
    for(int n = 0; n < 10; ++n)
        thrd_create(&thr[n], f, NULL);
    for(int n = 0; n < 10; ++n)
        thrd_join(thr[n], NULL);

    printf("The atomic counter is %lu\n", atm_long);
    printf("The non-atomic counter is %lu\n", tm);

    tm = atm_long;
    printf("The time counter is %lu\n", tm);

}