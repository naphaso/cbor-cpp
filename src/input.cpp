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

#include <stdlib.h>
#include <string.h>

using namespace cbor;

input::input(void *data, int size) {
    this->data = (unsigned char *)data;
    this->size = size;
    this->offset = 0;
}

input::~input() {

}

bool input::hasBytes(int count) {
    return size - offset >= count;
}

unsigned char input::getByte() {
    return data[offset++];
}

unsigned short input::getShort() {
    unsigned short value = ((unsigned short)data[offset] << 8) | ((unsigned short)data[offset + 1]);
    offset += 2;
    return value;
}

unsigned int input::getInt() {
    unsigned int value = ((unsigned int)data[offset] << 24) | ((unsigned int)data[offset + 1] << 16) | ((unsigned int)data[offset + 2] << 8) | ((unsigned int)data[offset + 3]);
    offset += 4;
    return value;
}

unsigned long long input::getLong() {
    unsigned long long value = ((unsigned long long)data[offset] << 56) | ((unsigned long long)data[offset+1] << 48) | ((unsigned long long)data[offset+2] << 40) | ((unsigned long long)data[offset+3] << 32) | ((unsigned long long)data[offset+4] << 24) | ((unsigned long long)data[offset+5] << 16) | ((unsigned long long)data[offset+6] << 8) | ((unsigned long long)data[offset+7]);
    offset += 8;
    return value;
}

void input::getBytes(void *to, int count) {
    memcpy(to, data + offset, count);
    offset += count;
}
