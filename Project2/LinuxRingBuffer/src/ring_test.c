/*========================================================================
** Author: Sam Solondz
** Date: 3/18/19
** ring_test.c
** Circular buffer testing
** ECEN5813
**========================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CUnit/Basic.h"
#include "CUnit/TestDB.h"
#include "CUnit/TestRun.h"
#include "CUnit/CUError.h"
#include "CUnit/CUCurses.h"
#include "CUnit/Console.h"
#include "CUnit/Automated.h"
#include "ring.h"
int init_suite1(void)
{
    return 0;
}
int clean_suite1(void)
{
    return 0;
}
void init_test(void)
{
    ring_t * ring = init(4);
    CU_ASSERT_EQUAL(ring->Length, 4);
    free(ring);
}
void empty_remove_entry_test(void)
{
    ring_t * ring = init(4);
    char * removed = 0;
    CU_ASSERT_EQUAL(remove_entry(ring, removed), -1);
    CU_ASSERT_NOT_EQUAL(remove_entry(ring, removed), 0);
    free(ring);
}
void insert_test(void)
{
    ring_t * ring = init(4);
    CU_ASSERT_EQUAL(insert(ring, 'b'), 0);
    CU_ASSERT_NOT_EQUAL(insert(ring, 'b'), -1);
    free(ring);
}
void remove_entry_test(void)
{
    ring_t * ring = init(4);
    insert(ring, 'b');
    insert(ring, 'c');
    insert(ring, 'd');
    char removed;

    CU_ASSERT_EQUAL(remove_entry(ring, &removed), 0);
    CU_ASSERT_EQUAL(removed, 'b');


    CU_ASSERT_EQUAL(remove_entry(ring, &removed), 0);
    CU_ASSERT_EQUAL(removed, 'c');

    CU_ASSERT_EQUAL(remove_entry(ring, &removed), 0);
    CU_ASSERT_EQUAL(removed, 'd');

    CU_ASSERT_EQUAL(remove_entry(ring, &removed), -1);
    free(ring);

}

void entries_test(void)
{
    ring_t * ring = init(4);
    insert(ring, 'a');
    insert(ring, 'b');
    insert(ring, 'c');

    CU_ASSERT_EQUAL(entries(ring), 3);

    ring = init(20);
    insert(ring, 'a');
    insert(ring, 'b');
    insert(ring, 'c');
    insert(ring, 'd');
    insert(ring, 'e');
    insert(ring, 'f');
    insert(ring, 'g');


    CU_ASSERT_EQUAL(entries(ring), 7);
    free(ring);
}

void insert_full_test(void)
{
    ring_t * ring = init(4);
    CU_ASSERT_EQUAL(insert(ring, 'b'), 0);
    CU_ASSERT_EQUAL(insert(ring, 'c'), 0);
    CU_ASSERT_EQUAL(insert(ring, 'd'), 0);
    CU_ASSERT_EQUAL(insert(ring, 'e'), -1);
    free(ring);
}
/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;
   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();
   /* add a suite to the registry */
   pSuite = CU_add_suite("Ring test suite", init_suite1, clean_suite1);
   if (NULL == pSuite) {

      CU_cleanup_registry();
      return CU_get_error();
   }
   /* add the tests to the suite */
   if ((NULL == CU_add_test(pSuite, "Initialization test", init_test)) ||
      (NULL == CU_add_test(pSuite, "Remove entry from empty ring test", empty_remove_entry_test)) ||
      (NULL == CU_add_test(pSuite, "Insert test", insert_test)) ||
      (NULL == CU_add_test(pSuite, "Remove entry test", remove_entry_test)) ||
      (NULL == CU_add_test(pSuite, "Insert full test", insert_full_test)) ||
      (NULL == CU_add_test(pSuite, "Count entries test", entries_test)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_basic_show_failures(CU_get_failure_list());
   CU_cleanup_registry();
   return CU_get_error();
}
