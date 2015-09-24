#include "e_lib.h"
struct point {
  int x;
  int y;
  int z;
  int hash;
};

int BASE = 0x8E000000;
int BUFF_SIZE = 0x7EE0 + 0x0004;

int main(void) {
	e_coreid_t coreid;
  //int* data =
	//unsigned row, col, *done;



  int* test;
  test = BASE;

  int correct = 2;
  int fail = 3;

  if (*test) {
    e_dma_copy( (void*)(BASE+sizeof(int)) ,&correct,4);
  }
  else {
    e_dma_copy( (void*)(BASE+sizeof(int)),&fail,4);
  }

	// Who am I? Query the CoreID from hardware.
	coreid = e_get_coreid();
	//e_coords_from_coreid(coreid, &row, &col);

  return 0;

}
