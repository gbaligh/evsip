#include <stdlib.h>
#include <check.h>


START_TEST (test1)
{
  ck_assert_str_eq("USD", "USD"); 
}
END_TEST

Suite *register_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("Register");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test1);
  suite_add_tcase(s, tc_core);

  return s;
}


int main()
{
  Suite *s;
  SRunner *sr;
  int number_failed;

  s = register_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
