/**
@file     bloomgilter.cpp
@brief    Bloom Filter, implemented for WIN32.
@details  Copyright (c) 2001-2008 Acronis
*/
#include "bloomfilter.h"
#include <vector>
#include <iostream>
#include <math.h>
#include "MurmurHash3.h"

namespace {
  int getIntLength(int d)
  {
    unsigned int number_of_digits = 0;
    do {
      ++number_of_digits;
      d /= 10;
    } while (d);
    return number_of_digits;
  }
  
  uint32_t murmur(int d, int seed) {
    uint32_t hash;
    MurmurHash3_x86_32(&(d), getIntLength(d), seed, (void*)&hash);

    return hash;
  }
}

class BloomFilterImpl : public BloomFilter
{
  std::vector<bool> bitArray;
  int m; //bit array of m bits
  int k; //number of hash functions
  int n; //number of elements expected
  double p;
public:

  BloomFilterImpl(int capacity, double error_rate) {
    p = error_rate;
    n = capacity;
    m = ceil((n * log(p)) / log(1.0 / (pow(2.0, log(2.0)))));
    k = round(log(2.0) * m / n);
    bitArray.resize(m);
  }
  virtual void insert(int data)
  {
    for (int i = 0; i < k; ++i)
    {
      bitArray[murmur(data, i)%m] = true;
    }
  }

  virtual bool query(int data)
  {
    for (int i = 0; i < k; ++i)
    {
      if (!(bitArray[murmur(data, i)%m])) {
        return false;
      }
    }
    return true;
  }

  void printStatus()
  {
    int currentNumElements = 0;
    for (int i = 0; i < bitArray.size(); ++i) {
      if (bitArray[i])
        ++currentNumElements;
    }
    std::cout << "Bit array of " << m << " bits, with " << k << " hash functions, " << currentNumElements << " elements inserted." <<std::endl;
  }
};

std::auto_ptr<BloomFilter> CreateBF(int capacity, double error_rate)
{
  return std::auto_ptr<BloomFilter>(new BloomFilterImpl(capacity, error_rate));
}
