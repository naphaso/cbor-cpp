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

#include "output.h"

namespace cbor {
    const int default_capacity = 256;

    struct output {
        explicit output(unsigned int inital_capacity = default_capacity);

         uint8_t *data();

         unsigned int size();

         void put_byte(uint8_t value);

         void put_bytes(const uint8_t *data, int size);

    private:
        std::vector<uint8_t> m_buffer;
    };
}
