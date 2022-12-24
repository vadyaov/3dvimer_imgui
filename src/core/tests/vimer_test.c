#include "test.h"

START_TEST(parser_t1) {

}
END_TEST;

Suite *VimerSuite() {
  Suite *vimer = suite_create("[Vimer Unit Test]");
  TCase *tc = tcase_create("vimer");

  tcase_add_test(tc, parser_t1);

  suite_add_tcase(vimer, tc);

  return vimer;
}
