#pragma once
#include <vector>
using gpio_num_t = int;
constexpr int GPIO_MODE_OUTPUT=1;
inline std::vector<int> operations;
inline bool held=true, output=true, pad=true;
inline int gpio_set_level(int pin,int level) { assert(pin==2); operations.push_back(level ? 1 : 0); output=level; if(!held)pad=output; return 0; }
inline int gpio_set_direction(int pin,int mode) { assert(pin==2 && mode==1); operations.push_back(2); return 0; }
inline int gpio_hold_dis(int pin) { assert(pin==2); operations.push_back(3); held=false; pad=output; return 0; }
inline int gpio_hold_en(int pin) { assert(pin==2); operations.push_back(4); held=true; return 0; }
