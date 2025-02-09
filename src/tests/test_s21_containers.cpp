#include "test_s21_containers.h"

extern void AddVectorTests();
extern void AddMapTests();
extern void AddSetTests();
extern void AddMultisetTests();
extern void AddArrayTests();

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  AddVectorTests();
  AddMapTests();
  AddSetTests();
  AddMultisetTests();
  AddArrayTests();

  return RUN_ALL_TESTS();
}
