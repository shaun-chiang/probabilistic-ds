/**
@file     bloomfilter.h
@brief    Streaming Quotient Filter, implemented for WIN32.
@details  Copyright (c) 2001-2008 Acronis
*/

//includes
#pragma once
#include <memory>
#include <string>

class BloomFilter
{
public:
  virtual ~BloomFilter(){}
  virtual void insert(int data) = 0;
  virtual bool query(int data) = 0;
  virtual void printStatus() = 0;
};

std::auto_ptr<BloomFilter> CreateBF(int capacity, double error_rate);