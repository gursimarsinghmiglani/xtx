#pragma once
#include <cstddef>
#include <functional>
#include <unordered_set>
struct SetHash {
  size_t operator()(const std::unordered_set<int> &set) const {
    size_t hash = 0;
    for (int x : set) {
      size_t h = std::hash<int>()(x);
      h ^= h >> 16;
      h *= 0x85ebca6b;
      h ^= h >> 13;
      h *= 0xc2b2ae35;
      h ^= h >> 16;
      hash ^= h;
    }
    return hash;
  }
};
