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

#include "output_static.h"
#include "log.h"

#include <string.h>

using namespace cbor;

output_static::output_static(unsigned int capacity) {
    this->_capacity = capacity;
    this->_buffer = new unsigned char[capacity];
    this->_offset = 0;
}

output_static::~output_static() {
    delete _buffer;
}

void output_static::put_byte(unsigned char value) {
    if(_offset < _capacity) {
        _buffer[_offset++] = value;
    } else {
        logger("buffer overflow error");
    }
}

void output_static::put_bytes(const unsigned char *data, int size) {
    if(_offset + size - 1 < _capacity) {
        memcpy(_buffer + _offset, data, size);
        _offset += size;
    } else {
        logger("buffer overflow error");
    }
}

unsigned char *output_static::getData() {
    return _buffer;
}

unsigned int output_static::getSize() {
    return _offset;
}
