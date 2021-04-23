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

#pragma once

#include <stdint.h>
#include <vector>
#include <string>

namespace cbor {
  

    struct input {
        explicit input(const std::vector<uint8_t>& data) : m_data(data), m_offset(0) {}

        bool has_bytes(int count);

        uint8_t get_byte();

        unsigned short get_short();

        unsigned int get_int();
        float get_float();
        double get_double();

        unsigned long long get_long();

        std::vector<uint8_t> get_bytes(int count);

        std::string get_str(int count);
    private:
        std::vector<uint8_t> m_data{};
        size_t m_offset{};
    };
}
