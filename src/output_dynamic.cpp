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

#include "output_dynamic.h"

#include <string.h>
#include <stdlib.h>

using namespace cbor;


void output_dynamic::init(unsigned int initalCapacity) {
    this->m_capacity = initalCapacity;
    this->m_buffer = new unsigned char[initalCapacity];
    this->m_offset = 0;
}

output_dynamic::output_dynamic() {
    init(256);
}

output_dynamic::output_dynamic(unsigned int inital_capacity) {
    init(inital_capacity);
}

output_dynamic::~output_dynamic() {
    delete[] m_buffer;
}

unsigned char *output_dynamic::data() {
    return m_buffer;
}

unsigned int output_dynamic::size() {
    return m_offset;
}

void output_dynamic::put_byte(unsigned char value) {
    if(m_offset < m_capacity) {
        m_buffer[m_offset++] = value;
    } else {
        m_capacity *= 2;
        m_buffer = (unsigned char *) realloc(m_buffer, m_capacity);
        m_buffer[m_offset++] = value;
    }
}

void output_dynamic::put_bytes(const unsigned char *data, int size) {
    while(m_offset + size > m_capacity) {
        m_capacity *= 2;
        m_buffer = (unsigned char *) realloc(m_buffer, m_capacity);
    }

    memcpy(m_buffer + m_offset, data, size);
    m_offset += size;
}
