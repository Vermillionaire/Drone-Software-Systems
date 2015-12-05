#ifndef CONSTANTS_H
#define CONSTANTS_H

class Constants{

  public:
        const int NUM_CORES = 16;
        const int HOST_BASE_ADDRESS = 0x3E000000;
        const int CO_BASE_ADDRESS = 0x8E000000;
        const int FRAME_WIDTH = 640;
        const int FRAME_HEIGHT = 480;
        const int FRAME_SIZE = FRAME_WIDTH * FRAME_HEIGHT;
        const int IR_BIRGHTNESS = 50;
        const int BASE_FRAME_LIMITER = 10;
        const int MAX_GET_SIZE = 10000;
        const int BUFFER_SIZE = FRAME_WIDTH * FRAME_HEIGHT * 60;

};

#endif
