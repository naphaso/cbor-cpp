/*
   Copyright 2014-2015 Stanislav Ovsyannikov

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

	   Unless required by applicable law or agreed to in writing, software
	   distributed under the License is distributed on an "AS IS" BASIS,
	   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	   See the License for the specific language governing permissions and
	   limitations under the License.
*/

#include "input.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iostream>

using namespace cbor;

bool input::has_bytes(int count) {
    return m_data.size() - m_offset >= count;
}

uint8_t input::get_byte() {
    return m_data[m_offset++];
}

unsigned short input::get_short() {
    unsigned short value = ((unsigned short) m_data[m_offset] << 8) | ((unsigned short) m_data[m_offset + 1]);
    m_offset += 2;
    return value;
}

unsigned int input::get_int() {
    unsigned int value = \
            ((unsigned int) m_data[m_offset    ] << 24) |
            ((unsigned int) m_data[m_offset + 1] << 16) |
            ((unsigned int) m_data[m_offset + 2] << 8 ) |
            ((unsigned int) m_data[m_offset + 3]);
    m_offset += 4;
    return value;
}

float input::get_float() {
    uint8_t value[4] = {
      m_data[m_offset + 3],
      m_data[m_offset + 2],
      m_data[m_offset + 1],
      m_data[m_offset + 0]
    };
    m_offset += 4;
    return *((float*) (&value[0]));
}

double input::get_double() {
  double ret;
  uint8_t* ptr = (uint8_t*)(void*) &ret;
  *(ptr + 0) = m_data[m_offset + 7];
  *(ptr + 1) = m_data[m_offset + 6];
  *(ptr + 2) = m_data[m_offset + 5];
  *(ptr + 3) = m_data[m_offset + 4];
  *(ptr + 4) = m_data[m_offset + 3];
  *(ptr + 5) = m_data[m_offset + 2];
  *(ptr + 6) = m_data[m_offset + 1];
  *(ptr + 7) = m_data[m_offset + 0];
  m_offset += 8;
  return ret;
}

unsigned long long input::get_long() {
    unsigned long long value = ((unsigned long long) m_data[m_offset] << 56) |
            ((unsigned long long) m_data[m_offset +1] << 48) | ((unsigned long long) m_data[m_offset +2] << 40) |
            ((unsigned long long) m_data[m_offset +3] << 32) | ((unsigned long long) m_data[m_offset +4] << 24) |
            ((unsigned long long) m_data[m_offset +5] << 16) | ((unsigned long long) m_data[m_offset +6] << 8 ) |
            ((unsigned long long) m_data[m_offset +7]);
    m_offset += 8;
    return value;
}

std::vector<uint8_t> input::get_bytes(int count) {
    auto start = m_data.begin() + m_offset;
    return std::vector<uint8_t>(start, start + count);
}

std::string input::get_str(int count) {
    std::stringstream ss;
    auto begin = m_data.begin() + m_offset;
    auto end = m_data.begin() + m_offset + count;
    for(auto it = begin; it != m_data.end() && it != end; it++) {
        ss << *it;
        m_offset++;
    }

    return ss.str();
}