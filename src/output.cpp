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

#include "output.h"

#include <string.h>
#include <stdlib.h>

using namespace cbor;

output::output(unsigned int initalCapacity) : m_buffer(initalCapacity) {
}

uint8_t *output::data() {
    return m_buffer.data();
}

unsigned int output::size() {
    return m_buffer.size();
}

void output::put_byte(uint8_t value) {
    m_buffer.push_back(value);
}

void output::put_bytes(const uint8_t *data, int size) {
    for(auto i = 0; i < size; i++) {
        m_buffer.push_back(data[i]);
    }
}
