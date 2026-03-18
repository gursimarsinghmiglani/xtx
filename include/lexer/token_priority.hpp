#pragma once
#include "token.hpp"
inline Token max(Token t1, Token t2) {
  return static_cast<int>(t1) < static_cast<int>(t2) ? t1 : t2;
}
