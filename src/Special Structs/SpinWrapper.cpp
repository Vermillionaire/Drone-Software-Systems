#include "SpinWrapper.h"
#include "Constants.h"

SpinWrapper::SpinWrapper() {
  Constants c;
  array1 = new SpinArray(c.BUFFER_SIZE/2);
  array2 = new SpinArray(c.BUFFER_SIZE/2);
  }

SpinWrapper::~SpinWrapper() {
  delete array1;
  delete array2;
  }

SpinArray* SpinWrapper::getterArray() {
  if (!toggle)
    return array1;
  else
    return array2;
}

SpinArray* SpinWrapper::putterArray(){
  if (toggle) {
    toggle = !toggle;
    return array1;
  }
  else {
    toggle = !toggle;
    return array2;
  }
}

SpinArray* SpinWrapper::getOne() {
  return array1;
}

SpinArray* SpinWrapper::getTwo(){
  return array2;
}
