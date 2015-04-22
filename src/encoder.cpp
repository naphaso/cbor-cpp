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

#include "encoder.h"

using namespace cbor;


encoder::encoder(output &out) {
    _out = &out;
}

encoder::~encoder() {

}

void encoder::writeTypeAndValue(int majorType, unsigned int value) {
    majorType <<= 5;
    if(value < 24) {
        _out->putByte(majorType | value);
    } else if(value < 256) {
        _out->putByte(majorType | 24);
        _out->putByte(value);
    } else if(value < 65536) {
        _out->putByte(majorType | 25);
        _out->putByte(value >> 8);
        _out->putByte(value);
    } else {
        _out->putByte(majorType | 26);
        _out->putByte(value >> 24);
        _out->putByte(value >> 16);
        _out->putByte(value >> 8);
        _out->putByte(value);
    }
}

void encoder::writeTypeAndValue(int majorType, unsigned long long value) {
    majorType <<= 5;
    if(value < 24ULL) {
        _out->putByte(majorType | value);
    } else if(value < 256ULL) {
        _out->putByte(majorType | 24);
        _out->putByte(value);
    } else if(value < 65536ULL) {
        _out->putByte(majorType | 25);
        _out->putByte(value >> 8);
    } else if(value < 4294967296ULL) {
        _out->putByte(majorType | 26);
        _out->putByte(value >> 24);
        _out->putByte(value >> 16);
        _out->putByte(value >> 8);
        _out->putByte(value);
    } else {
        _out->putByte(majorType | 27);
        _out->putByte(value >> 56);
        _out->putByte(value >> 48);
        _out->putByte(value >> 40);
        _out->putByte(value >> 32);
        _out->putByte(value >> 24);
        _out->putByte(value >> 16);
        _out->putByte(value >> 8);
        _out->putByte(value);
    }
}

void encoder::writeInt(unsigned int value) {
    writeTypeAndValue(0, value);
}

void encoder::writeInt(unsigned long long value) {
    writeTypeAndValue(0, value);
}

void encoder::writeInt(long long value) {
    if(value < 0) {
        writeTypeAndValue(1, (unsigned long long) -value);
    } else {
        writeTypeAndValue(0, (unsigned long long) value);
    }
}

void encoder::writeInt(int value) {
    if(value < 0) {
        writeTypeAndValue(1, (unsigned int) -value);
    } else {
        writeTypeAndValue(0, (unsigned int) value);
    }
}

void encoder::writeBytes(const unsigned char *data, unsigned int size) {
    writeTypeAndValue(2, size);
    _out->putBytes(data, size);
}

void encoder::writeString(const char *data, unsigned int size) {
    writeTypeAndValue(3, size);
    _out->putBytes((const unsigned char *)data, size);
}

void encoder::writeString(const std::string str) {
    writeTypeAndValue(3, (unsigned int)str.size());
    _out->putBytes((const unsigned char *)str.c_str(), str.size());
}


void encoder::writeArray(int size) {
    writeTypeAndValue(4, (unsigned int)size);
}

void encoder::writeMap(int size) {
    writeTypeAndValue(5, (unsigned int)size);
}

void encoder::writeTag(const unsigned int tag) {
    writeTypeAndValue(6, tag);
}

void encoder::writeSpecial(int special) {
    writeTypeAndValue(7, (unsigned int)special);
}

