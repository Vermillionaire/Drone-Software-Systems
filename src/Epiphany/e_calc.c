#include "e_lib.h"
//#include "pal.h"
#include <math.h>
#include <stdlib.h>

typedef struct {
  int x;
  int y;
  int z;
  int angle;
} point;

typedef struct {
  int host_status;
  int core_status;
  int size;
} header;

int BASE = 0x8E000000;
int MULT = 100;
//int BUFF_SIZE = 16000;
int DEBUG = 0;
int DEBUG_OFFSET = 0x8;
int HEADER_SIZE = 0x0C;

const float minDistance = -10.0f;
const float scaleFactor = .0021f;
//half width and height
const float w2 = 320.0f;
const float h2 = 240.0f;
const int MAX_SIZE = 100;
const int SLEEP_TIME = 10; //1 cycle ~ 1 ns

//Core Status numbers
//9 sleeping
//8 size error
//7 At calculations phase
//2 Finished calculations
//1 Default Loaded
//0 Normal Exit
//3 Debug printing

void writeCoreStatus(int s) {
  e_dma_copy( (void*)(BASE+4), &s, sizeof(int));
}

header readHeader(void* base) {
  header h;
  e_dma_copy( &h, base, sizeof(header));
  return h;
}

void writeDebug(char *c, int size) {
  writeCoreStatus(5);
  int total = 0;

  e_dma_copy( &total, (void*)(DEBUG), sizeof(int));
  total += size;
  e_dma_copy( (void*)(DEBUG), &total, sizeof(int));
  e_dma_copy( (void*)(DEBUG + DEBUG_OFFSET), c, sizeof(char)*size);
  DEBUG_OFFSET += sizeof(char)*size;
}

void printNum(int num) {
  char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
  char *characters = malloc(sizeof(char)*10);

  int i = 9;
  for (; i>=0; i--) {
    int d = num % 10;
    num = num / 10;
    characters[i] = digits[d];
  }

  free(characters);
  writeDebug(characters, 10);
}

void calc(void* address) {
  int i;
  point p[MAX_SIZE];
  e_dma_copy( &p, address, sizeof(point)*MAX_SIZE);

  float angle = 0.0f;
  float c = 0.0f;
  float s = 0.0f;
  for (i = 0; i < MAX_SIZE; i++) {
    int z = p[i].z;
    if ( z <= 0 || z > 10000) {
      p[i].x = -1;
      p[i].y = -1;
      p[i].z = -1;
    }
    else {
      angle = ((float)p[i].angle)*M_PI/180.0f;
      c = cosf(angle);
      s = sinf(angle);
      float x = (((float)p[i].x - w2) * ((float)p[i].z + minDistance)) * scaleFactor;
      float y = (((float)p[i].y - w2) * ((float)p[i].z + minDistance)) * scaleFactor;
      float z = (float)p[i].z;
      //p[i].x = (int)(z*c - y*s);
      //p[i].y = (int)(y*c - z*s);
      p[i].x = (int) z;
      p[i].y = (int) y;
      p[i].z = (int) x;
    }
  }
  e_dma_copy( address, &p, sizeof(point)*MAX_SIZE);
}


int main(void) {
  int core_num = 0;
  int comp_num = 0;
  char *s;

  {
    //Gets the coordinates of the core and converts it to a core number
    unsigned row, col;
    e_coreid_t coreid = e_get_coreid();
    e_coords_from_coreid(coreid, &row, &col);
    core_num = (row*4 + col) + 1;

    //Sets the Debug text location based off of the core number
    DEBUG = BASE + 0x00100000 * core_num;

    //Start Debug size at 0
    int zero = 0;
    e_dma_copy( (void*)(DEBUG), &zero, 4);
    e_dma_copy( (void*)(DEBUG+0x4), &core_num, 4);
  }

  s = "Starting processing.\n";
  writeDebug(s, 21);

  header h;
  h.host_status = 2;
  h.core_status = 1;
  h.size = 500;

  //Changes the base address for the specific core data
  BASE += (HEADER_SIZE + sizeof(point)*MAX_SIZE*MULT)*(core_num - 1);

  writeCoreStatus(1);
  //h = readHeader((void*)BASE);
  //printNum(h.host_status);
  //printNum(h.core_status);
  //printNum(h.size);
  //printNum(HEADER_SIZE);

  s = "Entering loop.\n";
  writeDebug(s, 15);

  int num_cycles = 0;
  int num_sleeps = 0;
  while (h.host_status > 0) {
    num_cycles++;


    //Loop until it is time to compute (waits for a status of one)
    h = readHeader((void*)BASE);

    //Sleep if host flag is not set to compute
    if (h.host_status != 1) {
      num_sleeps++;

      if (h.core_status != 9)
        writeCoreStatus(9);

      e_wait(E_CTIMER_0, SLEEP_TIME);
    //  int i=0;
    //while (i<SLEEP_TIME) {i++;};

      continue;
    }

    //Sleep if the host says compute, but your status says you finished computing
    if (h.host_status == 1 && h.core_status == 2) {
      num_sleeps++;
      e_wait(E_CTIMER_0, SLEEP_TIME);
      continue;
    }


    int i=0;
    writeCoreStatus(7);

    /*
    //Allocate array for the data and copy it to the core
    e_dma_copy( &p, (void*)(BASE + 0x0C), sizeof(point)*MAX_SIZE);

    for (i = 0; i < h.size; i++) {
      int z = p[i].z;
      if ( z <= 0 || z > 10000) {
        p[i].x = -1;
        p[i].y = -1;
        p[i].z = -1;
      }
      else {

        p[i].x = (int)((((float)p[i].x - w2) * ((float)p[i].z + minDistance)) * scaleFactor * 0.1f);
        p[i].y = (int)((((float)p[i].y - w2) * ((float)p[i].z + minDistance)) * scaleFactor * 0.1f);
        p[i].z = (int)((float)p[i].z * 0.1f);

      }
    }

    e_dma_copy( (void*)(BASE + 0x0C), &p, sizeof(point)*rem);
    comp_num++;
    */
    for (i=0; i<MULT; i++) {
      calc((void*)(BASE + 0x0C + i*MAX_SIZE*sizeof(point)));
      comp_num++;
    }
    /*
    calc((void*)(BASE + 0x0C));
    calc((void*)(BASE + 0x0C + 1000*sizeof(point)));
    comp_num++;
    comp_num++;
    */

    writeCoreStatus(2);
  }

  writeCoreStatus(3);
  s = "\nTotal calculations: ";
  writeDebug(s, 21);
  printNum(comp_num);

  s = "\nSleeps: ";
  writeDebug(s, 9);
  printNum(num_sleeps);

  s = "\nCycles: ";
  writeDebug(s, 9);
  printNum(num_cycles);

  writeCoreStatus(0);
  return 0;

}
