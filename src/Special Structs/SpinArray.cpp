#include "SpinArray.h"
#include <iostream>

SpinArray::SpinArray(const int arraySize) {
    length = arraySize;

    top = new Point[arraySize];
    head = 0;
    tail = 0;
    distance = 0;

    overwrite = false;
    overflow = false;
    lossCounter = 0;
};

SpinArray::~SpinArray() {
    //for (int i=0; i<length; i++)
      //  if (top[i] != nullptr)
          //  delete top[i];

    delete[] top;
};

bool SpinArray::isOverwriting() {
    return overwrite;
};

bool SpinArray::isOverflowing() {
    return overflow;
};

long SpinArray::getLossCount() {
    return lossCounter;
};

void SpinArray::setOverwrite(bool over) {
    overwrite = over;
};

void SpinArray::resetCount() {
    lossCounter = 0;
};


bool SpinArray::put(short depth, short i, short j) {
  //std::lock_guard<std::mutex> lock(mutex);

  if (length/2 < distance && DataControl::frameLimiter != 500)
    DataControl::frameLimiter++;
  else
    if (DataControl::frameLimiter != 0)
      DataControl::frameLimiter--;

  if (overflow && !overwrite) {
      lossCounter++;
      if (distance < length)
        overflow = false;
      return false;
  }

  top[head].z = depth;
  top[head].x = i;
  top[head].y = j;
  head++;
  distance++;

  if (head >= length)
      head = 0;

  //if (head == tail || top[head] != nullptr)
  if (distance == length)
      overflow = true;

  return true;

};

void SpinArray::put(short* frame, int width, int height) {
  std::lock_guard<std::mutex> lock(mutex);

  if (length/2 < distance && DataControl::frameLimiter != 500)
    DataControl::frameLimiter++;
  else
    if (DataControl::frameLimiter != 0)
      DataControl::frameLimiter--;

  for (int i=0; i<height; i++) {
    for (int j=0; j<width; j++) {
      int pos = width*i+j;

      if (overflow && !overwrite) {
          lossCounter++;
          if (distance < length)
            overflow = false;
          continue;
      }

      top[head].z = frame[pos];
      top[head].x = j;
      top[head].y = i;
      head++;
      distance++;

      if (head >= length)
          head = 0;

      //if (head == tail || top[head] != nullptr)
      if (distance == length)
          overflow = true;
    }
  }
}

//Obsolete
Point * SpinArray::get() {
  std::lock_guard<std::mutex> lock(mutex);

  if (distance == 0)
    return nullptr;

  Point* item = &top[tail];

  //if (item  != nullptr) {
      //top[tail] = nullptr;

  if (tail+1 >= length)
      tail = 0;
  else
      tail++;

  distance--;

  return item;
};


void SpinArray::get(Point *array, int size) {
  std::lock_guard<std::mutex> lock(mutex);

  for (int i=0; i<size; i++) {

    //Creates empty gaps, fix pls
    if (distance == 0)
      continue;

    array[i] = top[tail];

    //if (item  != nullptr) {
      //top[tail] = nullptr;

    if (tail+1 >= length)
      tail = 0;
    else
      tail++;

    distance--;
  }

};

long SpinArray::getDistance() {
  return distance;
}

void SpinArray::print() {
    std::cout << "Head = " << head << "  Tail = " << tail << "  Loss = " << lossCounter << std::endl;
    std::cout << "FLAGS: overwrite[" << overwrite << "]  overflow[" << overflow << "]" << std::endl;
    for (int i=0; i<length; i++) {
        std::cout << " [";
        if (i == head)
            std::cout << "H";
        if (i == tail)
            std::cout << "T";
        else
            std::cout << ".";

        std::cout << "]";
    }
    std::cout << std::endl;

}

void SpinArray::printSize() {
    int size = 100;
    int scale = length / size;

    std::cout << "[";

    for (int i=0; i<size; i++) {
        int pos = i*scale;
        if (head < tail) {
          if (pos < head)
            std::cout << "X";
          else if (pos >= tail)
            std::cout << "X";
          else
            std::cout << "-";
        }
        else if (head > tail) {
          if (pos < tail)
            std::cout << "-";
          else if (pos >= head)
            std::cout << "-";
          else
            std::cout << "X";
        }
        else {
            if (distance == 0)
              std::cout << "-";
            else
              std::cout << "X";
        }

    }

    std::cout << "] l[" << length << "] h[" << head << "] t[" << tail << "] d[" << distance << "] L[" << lossCounter << "]" ;
}

void SpinArray::lock() {
  mutex.lock();
}

void SpinArray::unlock() {
  mutex.unlock();
}
