#include <iostream>
#include "hyperloglog.h"
#include <random>
#include <ctime>
#include <set>
#include "bloomfilter.h"

int main() {
  std::mt19937 mt_rand(time(0));
  std::set<int> set;
  //std::auto_ptr<HyperLogLog> hll = CreateHLL(16);
  std::auto_ptr<BloomFilter> bf = CreateBF(1000000, 0.1);
  for (int i = 0; i < 1000000; ++i) {
    int random = mt_rand() % 500000;
    //hll->insert(random);
    set.insert(random);
    bf->insert(random);
  }


  bf->printStatus();
  /*int realSize = set.size();
  int estimateSize = hll->estimateCount();
  long double error = realSize / estimateSize;
  std::cout << "Real Count (std::set): " << realSize << std::endl;
  std::cout << "Estimated Count (HyperLogLog): " << estimateSize << " with error rate: " << error << std::endl;*/


  int false_positive_count=0;
  for (int i = 0; i < 10000; ++i) {
    int random = mt_rand() % 1000000;
    if (!(set.find(random)!=set.end()) && (bf->query(random))) {
    ++false_positive_count;
    }
  }
  
  std::cout << "False Positive Count: " << false_positive_count << std::endl; 

}