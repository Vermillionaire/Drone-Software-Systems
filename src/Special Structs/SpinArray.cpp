#include "SpinArray.h"
#include <iostream>
#include <stdio.h>

SpinArray::SpinArray(const int arraySize) {
    length = arraySize;

    top = new PointKey[arraySize];
    head = 0;
    tail = 0;
    distance = 0;

    overwrite = false;
    overflow = false;
    lossCounter = 0;
    lastOverflow = false;

}

SpinArray::~SpinArray() {
    //for (int i=0; i<length; i++)
      //  if (top[i] != nullptr)
          //  delete top[i];

    delete[] top;
}

bool SpinArray::isOverwriting() {
    return overwrite;
}

bool SpinArray::isOverflowing() {
    return overflow;
}

long SpinArray::getLossCount() {
    return lossCounter;
}

bool SpinArray::didPutOverflow() {
  bool o = lastOverflow;
  lastOverflow = false;
  return o;
}

void SpinArray::setOverwrite(bool over) {
    overwrite = over;
}

void SpinArray::resetCount() {
    lossCounter = 0;
}


bool SpinArray::put(short depth, short i, short j) {
  //std::lock_guard<std::mutex> lock(mutex);

  /*
  if (length/2 < distance && DataControl::frameLimiter != 500)
    DataControl::frameLimiter++;
  else
    if (DataControl::frameLimiter != 0)
      DataControl::frameLimiter--;
      */

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

}

void SpinArray::put(short* frame, int width, int height, int angle) {
  std::lock_guard<std::mutex> lock(mutex); {


  //std::cout << "PUTTING" << std::endl;

  /*
  if (length>>1 < distance && DataControl::frameLimiter != 500)
    DataControl::frameLimiter++;
  else
    if (DataControl::frameLimiter != 0)
      DataControl::frameLimiter -= 3;
      */

  int row = 0;
  int add = head;
  //int dis = distance;
  int len = length;
  //int loss = 0;
  bool over_f = overflow;
  bool over_w = overwrite;

  PointKey p;
  int point_size = sizeof(PointKey);

  if( width * height > length - distance && !over_w) {
    lossCounter += width * height;
    lastOverflow = true;
    return;
  }

  for (int i=0; i<height; i++) {
    row = i * width;
    for (int j=0; j<width; j++) {

      /*
      if (over_f && !over_w) {
          loss++;
          if (dis < len)
            over_f = false;
          continue;
      }*/

      p.z = frame[row+j];
      p.x = j;
      p.y = i;
      p.angle = angle;

      //std::cout << point_size << "\t" << add << "\t" << distance << "\t" << tail   << "\t" << i << "\t" << j << "\n";
      memcpy( &(top[add]), &p, point_size);
      add++;

      /*
      top[add].z = frame[row+j];
      top[add].x = j;
      top[add].y = i;
      add++;
      */
      //dis++;


      if (add >= len)
          add = 0;

      //if (dis == len)
          //over_f = true;
    }


  }

  head = add;
  distance += width * height;
  overflow = over_f;
}
  //lossCounter += loss;
}


//Obsolete
/*
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
*/

int SpinArray::get(PointKey *array, int size) {
  //std::lock_guard<std::mutex> lock(mutex);
  std::unique_lock<std::mutex> mutexLock(mutex); {

  //std::cout << "Size requested: " << size << " Distance: " << distance << std::endl;

  if (size > distance) {
    size = distance;
    if (size == 0)
      return 0;
  }

  //int point_size = sizeof(PointKey);
  if ( size + tail <= length ) {
    //std::cout << sizeof(PointKey) << "\t" << head << "\t" << distance << "\t" << tail << "\t" << length << "\n";
    //printSize();
    //std::cout << array << "\t" << array+sizeof(PointKey)*size << "\t" << top << "\t" << top + sizeof(PointKey)*tail << "\t" << std::endl;

    memcpy(array, &top[tail], size);

    distance -= size;
    tail += size;

    return size;
  }
  else {
    //std::cout << "Alt 1 " << std::endl;
    int rem = length - tail;
    memcpy(array, &top[tail], rem);

    //std::cout << "Alt 2 " << std::endl;
    tail = 0;
    memcpy(&array[rem], top, size - rem);
    distance -= size;
    tail += size - rem;
    return size;
  }
}

  /*
  for (int i=0; i<size; i++) {
    array[i].key = 0;

    //Creates empty gaps, fix pls
    if (distance == 0)
      continue;

    array[i].x = top[tail].x;
    array[i].y = top[tail].y;
    array[i].z = top[tail].z;

    //if (item  != nullptr) {
      //top[tail] = nullptr;

    if (tail+1 >= length)
      tail = 0;
    else
      tail++;
    */
    //distance--;
  //}//

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
    int size = 75;
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
