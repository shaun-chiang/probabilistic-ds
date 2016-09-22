/**
@file     hyperloglog.cpp
@brief    HyperLogLog, implemented for WIN32.
More information is found in the header file.
@details  Copyright (c) 2001-2008 Acronis
*/

//includes
#include "hyperloglog.h"
#include <vector>
#include <math.h>
#include <iostream>
#include "MurmurHash3.h"

namespace {
  static const double two_32 = 4294967296.0; 
  static const double neg_two_32 = -4294967296.0;
  uint32_t seed = 313;

  //knuth multiplicative method hash. apparently, it's not very good.
  uint32_t knuthhash(uint32_t v)
  {
    return v * UINT32_C(2654435761);
  }

  //djb2. better, but not that good
  uint32_t hf(int d) {
    std::string s = std::to_string(d);
    unsigned long hash = 5381;
    for (auto c : s) {
      hash = (hash << 5) + hash + c; /* hash * 33 + c */
    }
    return hash;
  }

  int getIntLength(int d)
  {
    unsigned int number_of_digits = 0;
    do {
      ++number_of_digits;
      d /= 10;
    } while (d);
    return number_of_digits;
  }

  uint32_t murmur(int d) {
    uint32_t hash;
    MurmurHash3_x86_32(&(d), getIntLength(d), seed, (void*)&hash);
    return hash;
  }


}

class HyperLogLogImpl: public HyperLogLog {
  std::vector<uint8_t> buckets;
  int bucketWidth;
  double alpha;
  int b;
public:
  HyperLogLogImpl(int bitwidth) 
  {
    b = bitwidth;
    if ((b < 4) | (b > 16))
    {
      std::cout << "Invalid error value. Choose from 4-16.";
      return;
    }
    bucketWidth = 1 << b;
    alpha = getAlpha();
    buckets.resize(bucketWidth);
    long double error = 1.04 / sqrt(bucketWidth);
    std::cout << "Initialised HyperLogLog with standard error of 1.04/sqrt(2^" << bucketWidth << ") = " << error << std::endl;
  }
  
  virtual void insert(int data) 
  {
    //std::size_t hash = std::hash<int>{}(data); 
    std::size_t hash = murmur(data);
    std::size_t leadingZeroes = getLeadingZeroes(hash); 
    std::size_t bucketNumber = getBucketNumber(hash);
    //update the bucket, if applicable
    if (leadingZeroes > buckets[bucketNumber]) 
    {
      buckets[bucketNumber] = leadingZeroes;
    }
  }

  virtual std::size_t getLeadingZeroes(size_t hash) 
  {
    uint8_t count = 1;
    while (count<=(sizeof(hash)*8) && !(hash & 0x80000000)) {
      ++count;
      hash <<= 1; //shift hash to the next digit
    }
    return count;
  }

  virtual std::size_t getBucketNumber(size_t hash) 
  {
    return hash & ((1 << b) - 1);
  }

  virtual double getAlpha() 
  {
    if (b == 4) 
    {
      return 0.673;
    }
    else if (b == 5)
    {
      return 0.697;
    }
    else if (b == 6)
    {
      return 0.709;
    }
    return 0.7213 / (1 + 1.079 / (bucketWidth));
  }

  virtual int estimateCount()
  {
    double estimate;
    double sum = 0.0;
    for (uint32_t i = 0; i < bucketWidth; i++) {
      sum += 1.0 / (1 << buckets[i]);
    }
    estimate = (alpha * bucketWidth * bucketWidth) / sum; //Intermediate - no correction.
    if (estimate <= 2.5 * bucketWidth) 
    {
      uint32_t zeroes = 0;
      for (uint32_t i = 0; i < bucketWidth; ++i) {
        if (buckets[i] == 0) {
          ++zeroes;
        }
      }
      if (zeroes != 0) {
        estimate = bucketWidth * log(bucketWidth / zeroes); //Small Range Correction.
      }
    }
    else if (estimate > (1.0 / 30.0) * two_32) {
      estimate = neg_two_32 * log(1.0 - (estimate / two_32)); //Large Range Correction.
    }
    return estimate; 
  }

};

std::auto_ptr<HyperLogLog> CreateHLL(double countError)
{
  return std::auto_ptr<HyperLogLog>(new HyperLogLogImpl(countError));
}