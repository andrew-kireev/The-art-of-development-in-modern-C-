//
//  main.cpp
//  Josephus permutation
//
//  Created by Andrew Kireev on 06.06.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "test_runner.h"

#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include <list>
#include <queue>

#include "profile.h"

using namespace std;


template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
    list<typename RandomIt::value_type> pool;
//        pool.reserve(last - first);
    for (auto it = first; it != last; ++it) {
        pool.push_back(move(*it));
    }
    
    auto cur_pos = pool.begin();
    
    while (!pool.empty()) {
        *(first++) = move(*cur_pos);
        
        if (pool.size() == 1) {
            break;
        }
        
        auto next_pos = next(cur_pos) == pool.end() ? pool.begin() :
        next(cur_pos);
        
        pool.erase(cur_pos);
        
        cur_pos = next_pos;
        for (size_t i = 0; i < step_size - 1; ++i) {
          cur_pos = next(cur_pos) == pool.end() ? pool.begin() :
          next(cur_pos);
        }
    }
}

vector<int> MakeTestVector() {
  vector<int> numbers(10);
  iota(begin(numbers), end(numbers), 0);
  return numbers;
}

void TestIntVector() {
  const vector<int> numbers = MakeTestVector();
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
    ASSERT_EQUAL(numbers_copy, numbers);
  }
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
    ASSERT_EQUAL(numbers_copy, vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
  }
}

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем,
// почему он устроен именно так, далее в блоке про move-семантику —
// в видео «Некопируемые типы»

struct NoncopyableInt {
  int value;

  NoncopyableInt(const NoncopyableInt&) = delete;
  NoncopyableInt& operator=(const NoncopyableInt&) = delete;

  NoncopyableInt(NoncopyableInt&&) = default;
  NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator == (const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
  return lhs.value == rhs.value;
}

ostream& operator << (ostream& os, const NoncopyableInt& v) {
  return os << v.value;
}

void TestAvoidsCopying() {
  vector<NoncopyableInt> numbers;
    for(int i = 0; i < 10; ++i) {
        numbers.push_back({i});
    }
    {
        LOG_DURATION("test");

        for (int i = 0; i < 10'000'000; ++i) {
            MakeJosephusPermutation(begin(numbers), end(numbers), 2);
        }
    }

//  vector<NoncopyableInt> expected;
//  expected.push_back({1});
//  expected.push_back({3});
//  expected.push_back({5});
//  expected.push_back({4});
//  expected.push_back({2});

//  ASSERT_EQUAL(numbers, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  RUN_TEST(tr, TestAvoidsCopying);
  return 0;
}
