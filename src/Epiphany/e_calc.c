#include "e_lib.h"
#include "string.h"
typedef struct {
  int x;
  int y;
  int z;
  int hash;
} point;

typedef struct {
  int status;
  int size;
} header;

int BASE = 0x8E000000;
int BUFF_SIZE = 16000;
int DEBUG = 0x8E100000;
int DEBUG_OFFSET = 0x8;
//int HEADER_SIZE = 0x08;

const float minDistance = -10.0f;
const float scaleFactor = .0021f;
//half width and height
const float w2 = 320.0f;
const float h2 = 240.0f;

void writeStatus(int s) {
  e_dma_copy( (void*)(BASE), &s, sizeof(int));
}

void writeDebug(char *c) {
  int total = 0;
  int size = strlen(c);

  e_dma_copy( &total, (void*)(DEBUG), sizeof(int));
  total += size;
  e_dma_copy( (void*)(DEBUG), &total, sizeof(int));
  e_dma_copy( (void*)(DEBUG + DEBUG_OFFSET), c, sizeof(char)*size);
  DEBUG_OFFSET += sizeof(char)*size;
}


int main(void) {
  int core_num = 0;
  {
    //Start Debug size at 0
    int zero = 0;
    e_dma_copy( (void*)(DEBUG), &zero, sizeof(int));

    //Get Core information
    unsigned row, col;
  	e_coreid_t coreid = e_get_coreid();
    e_coords_from_coreid(coreid, &row, &col);
    core_num = row*4 + col;
    e_dma_copy( (void*)(DEBUG+0x4), &core_num, sizeof(int));
  }

  header h;
  h.status = 0;
  h.size = 0;

  e_dma_copy( &h, (void*)(BASE), 8);
  int status = h.status;
  int size = h.size;

  writeDebug("First copy done.\n");

  //e_dma_copy( (void*)(BASE), &DEBUG_OFFSET, sizeof(int));

  if (status > 5 || size > 2000 || size <= 0) {
    writeStatus(-1);
    return 1;
  }

  point p[size];
  if (status == 1) {
    e_dma_copy( &p, (void*)(BASE + sizeof(header)), sizeof(point)*size);
  }
  else {
    writeStatus(-2);
    return -1;
  }

//writeStatus((int)((((float)p[0].x - w2) * (p[0].z + minDistance)) * scaleFactor * 0.1f));
	// Who am I? Query the CoreID from hardware.
	//coreid = e_get_coreid();
	//e_coords_from_coreid(coreid, &row, &col);
  writeDebug("At Loop.\n");
  int i=0;
  for (i = 0; i < size; i++) {
    p[i].x = (int)((((float)p[i].x - w2) * ((float)p[i].z + minDistance)) * scaleFactor * 0.1f);
    p[i].y = (int)((((float)p[i].x - w2) * ((float)p[i].z + minDistance)) * scaleFactor * 0.1f);
    p[i].z = (int)((float)p[i].z * 0.1f);
  }


  e_dma_copy( (void*)(BASE + sizeof(header)), &p, sizeof(point)*size);
  writeStatus(2);

  return 0;

}
