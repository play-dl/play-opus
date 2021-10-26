#include "deps/opus/include/opus.h"
#include "iostream"

int main(){
  int error;
  std::cout << opus_encoder_create(48000, 2, OPUS_APPLICATION_AUDIO, &error);
  return 0;
}