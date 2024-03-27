#include "dynarrandutils.h"

int main() {
  Dynamic_array table_of_everything;
  Dynamic_array ten(10000, &table_of_everything);
  Dynamic_array hundred(100000, &table_of_everything);
  Dynamic_array five_hundred(500000, &table_of_everything);
  Dynamic_array milion(1000000, &table_of_everything);
  
  return 0;
}
