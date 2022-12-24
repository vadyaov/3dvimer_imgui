#include "test.h"

int main() {
  int no_failed1 = 0;

  Suite *s1;

  SRunner *r1;

  s1 = VimerSuite();
  r1 = srunner_create(s1);
  srunner_set_fork_status(r1, CK_NOFORK);
  srunner_run_all(r1, CK_NORMAL);
  no_failed1 = srunner_ntests_failed(r1);
  srunner_free(r1);

  int failures = (!no_failed1) ? 0 : 1;

  return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
