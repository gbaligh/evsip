#include <stdlib.h>
#include <check.h>

#include <sofia-sip/su.h>
#include <sofia-sip/su_alloc.h>
#include <sofia-sip/su_alloc_stat.h>
#include <evsip_mem.h>


START_TEST (test1)
  
  su_home_t home[1];
  int *pInt = (int *)0;
  unsigned int nbRef = 0;

  ck_assert(evsip_mem_init(NULL) != 0);
  
  ck_assert(evsip_mem_init(home) == 0);

  pInt = evsip_mem_alloc(sizeof(int), NULL);
  
  ck_assert(pInt != (int *)0);
  
  ck_assert(evsip_mem_nrefs(pInt) == 1);

  for (nbRef == evsip_mem_nrefs(pInt);
      nbRef < 100;
      nbRef ++) {
    ck_assert(evsip_mem_ref(pInt) != (void *)0);
    ck_assert(evsip_mem_nrefs(pInt) > nbRef);
  }

  while (nbRef) {
    ck_assert(evsip_mem_deref(pInt) == pInt);
    --nbRef;
  }
  
  ck_assert(evsip_mem_deref(pInt) == (void *)0);

END_TEST

Suite *mem_suite(void)
{
  Suite *s = (Suite *)0;
  TCase *tc_core = (TCase *)0;

  s = suite_create("Memory Manager");
  if (s) {
    /* Core test */
    tc_core = tcase_create("Core");
    if (tc_core) {
      tcase_add_test(tc_core, test1);
      suite_add_tcase(s, tc_core);
    }
  }

  return s;
}


int main(int argc, char *argv[])
{
  Suite *s = (Suite *)0;
  SRunner *sr = (SRunner *)0;
  int number_failed = 0;

  s = mem_suite();
  if (s) {
    sr = srunner_create(s);
    if (sr) {
      srunner_set_fork_status(sr, CK_FORK);
      srunner_run_all(sr, CK_VERBOSE);
      number_failed = srunner_ntests_failed(sr);
      srunner_print(sr, CK_VERBOSE);
      srunner_free(sr);
    }
  }

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
