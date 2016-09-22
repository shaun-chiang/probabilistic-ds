///**
//@file     streamingquotientfilter.cpp
//@brief    Streaming Quotient Filter, implemented for WIN32.
//@details  Copyright (c) 2001-2008 Acronis
//*/
//#include "streamingquotientfilter.h"
//#include <unordered_map>
//#include <vector>
//
//class StreamingQuotientFilterImpl : public StreamingQuotientFilter
//{
//  std::unordered_map<uint8_t, std::vector<uint8_t>> htable;
//  int r = 2;
//  int k = 4;
//  int rprime = r / 2;
//  //r=2, k=4, r’=r/2 = 1
//public:
//  StreamingQuotientFilterImpl(){
//
//  }
//
//  virtual void insert(int data)
//  {
//
//  }
//  virtual bool query()
//  {
//    return true;
//  }
//};
//
//std::auto_ptr<StreamingQuotientFilter> CreateSQF()
//{
//  return std::auto_ptr<StreamingQuotientFilter>(new StreamingQuotientFilterImpl());
//}