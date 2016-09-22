/**
@file     hyperloglog.h
@brief    Hyperloglog, implemented for WIN32.
@details  Copyright (c) 2001-2008 Acronis
*/

//includes
#pragma once
#include <memory>
#include <string>

class HyperLogLog
{
public:
  virtual ~HyperLogLog(){}
  virtual void insert(int data) =0;
  virtual int estimateCount() = 0;
};

std::auto_ptr<HyperLogLog> CreateHLL(double countError);