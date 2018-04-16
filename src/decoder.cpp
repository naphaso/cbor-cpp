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

#include "decoder.h"
#include "log.h"

#include <limits.h>

using namespace cbor;

decoder::decoder(input &in) {
    _in = &in;
    _state = STATE_TYPE;
}

decoder::decoder(input &in, listener &listener) {
    _in = &in;
    _listener = &listener;
    _state = STATE_TYPE;
}

decoder::~decoder() {

}

void decoder::set_listener(listener &listener_instance) {
    _listener = &listener_instance;
}

void decoder::run() {
    unsigned int temp;
    while(1) {
        if(_state == STATE_TYPE) {
            if(_in->has_bytes(1)) {
                unsigned char type = _in->get_byte();
                unsigned char majorType = type >> 5;
                unsigned char minorType = (unsigned char) (type & 31);

                switch(majorType) {
                    case 0: // positive integer
                        if(minorType < 24) {
                            _listener->on_integer(minorType);
                        } else if(minorType == 24) { // 1 byte
                            _currentLength = 1;
                            _state = STATE_PINT;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = STATE_PINT;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = STATE_PINT;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = STATE_PINT;
                        } else {
                            _state = STATE_ERROR;
                            _listener->on_error("invalid integer type");
                        }
                        break;
                    case 1: // negative integer
                        if(minorType < 24) {
                            _listener->on_integer(-1 -minorType);
                        } else if(minorType == 24) { // 1 byte
                            _currentLength = 1;
                            _state = STATE_NINT;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = STATE_NINT;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = STATE_NINT;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = STATE_NINT;
                        } else {
                            _state = STATE_ERROR;
                            _listener->on_error("invalid integer type");
                        }
                        break;
                    case 2: // bytes
                        if(minorType < 24) {
                            _state = STATE_BYTES_DATA;
                            _currentLength = minorType;
                        } else if(minorType == 24) {
                            _state = STATE_BYTES_SIZE;
                            _currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = STATE_BYTES_SIZE;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = STATE_BYTES_SIZE;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = STATE_BYTES_SIZE;
                        } else {
                            _state = STATE_ERROR;
                            _listener->on_error("invalid bytes type");
                        }
                        break;
                    case 3: // string
                        if(minorType < 24) {
                            _state = STATE_STRING_DATA;
                            _currentLength = minorType;
                        } else if(minorType == 24) {
                            _state = STATE_STRING_SIZE;
                            _currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = STATE_STRING_SIZE;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = STATE_STRING_SIZE;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = STATE_STRING_SIZE;
                        } else {
                            _state = STATE_ERROR;
                            _listener->on_error("invalid string type");
                        }
                        break;
                    case 4: // array
                        if(minorType < 24) {
                            _listener->on_array(minorType);
                        } else if(minorType == 24) {
                            _state = STATE_ARRAY;
                            _currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = STATE_ARRAY;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = STATE_ARRAY;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = STATE_ARRAY;
                        } else {
                            _state = STATE_ERROR;
                            _listener->on_error("invalid array type");
                        }
                        break;
                    case 5: // map
                        if(minorType < 24) {
                            _listener->on_map(minorType);
                        } else if(minorType == 24) {
                            _state = STATE_MAP;
                            _currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = STATE_MAP;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = STATE_MAP;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = STATE_MAP;
                        } else {
                            _state = STATE_ERROR;
                            _listener->on_error("invalid array type");
                        }
                        break;
                    case 6: // tag
                        if(minorType < 24) {
                            _listener->on_tag(minorType);
                        } else if(minorType == 24) {
                            _state = STATE_TAG;
                            _currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = STATE_TAG;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = STATE_TAG;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = STATE_TAG;
                        } else {
                            _state = STATE_ERROR;
                            _listener->on_error("invalid tag type");
                        }
                        break;
                    case 7: // special
                        if (minorType < 20) {
                            _listener->on_special(minorType);
                        } else if (minorType == 20) {
                            _listener->on_bool(false);
                        } else if (minorType == 21) {
                            _listener->on_bool(true);
                        } else if (minorType == 22) {
                            _listener->on_null();
                        } else if (minorType == 23) {
                            _listener->on_undefined();
                        } else if(minorType == 24) {
                            _state = STATE_SPECIAL;
                            _currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = STATE_SPECIAL;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = STATE_SPECIAL;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = STATE_SPECIAL;
                        } else {
                            _state = STATE_ERROR;
                            _listener->on_error("invalid special type");
                        }
                        break;
                }
            } else break;
        } else if(_state == STATE_PINT) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        _listener->on_integer(_in->get_byte());
                        _state = STATE_TYPE;
                        break;
                    case 2:
                        _listener->on_integer(_in->get_short());
                        _state = STATE_TYPE;
                        break;
                    case 4:
                        temp = _in->get_int();
                        if(temp <= INT_MAX) {
                            _listener->on_integer(temp);
                        } else {
                            _listener->on_extra_integer(temp, 1);
                        }
                        _state = STATE_TYPE;
                        break;
                    case 8:
                        _listener->on_extra_integer(_in->get_long(), 1);
                        _state = STATE_TYPE;
                        break;
                }
            } else break;
        } else if(_state == STATE_NINT) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        _listener->on_integer(-(int) _in->get_byte() - 1);
                        _state = STATE_TYPE;
                        break;
                    case 2:
                        _listener->on_integer(-(int) _in->get_short() - 1);
                        _state = STATE_TYPE;
                        break;
                    case 4:
                        temp = _in->get_int();
                        if(temp <= INT_MAX) {
                            _listener->on_integer(-(int) temp - 1);
                        } else {
                            _listener->on_extra_integer(temp + 1, -1);
                        }
                        _state = STATE_TYPE;
                        break;
                    case 8:
                        _listener->on_extra_integer(_in->get_long() + 1, -1);
                        break;
                }
            } else break;
        } else if(_state == STATE_BYTES_SIZE) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        _currentLength = _in->get_byte();
                        _state = STATE_BYTES_DATA;
                        break;
                    case 2:
                        _currentLength = _in->get_short();
                        _state = STATE_BYTES_DATA;
                        break;
                    case 4:
                        _currentLength = _in->get_int();
                        _state = STATE_BYTES_DATA;
                        break;
                    case 8:
                        _state = STATE_ERROR;
                        _listener->on_error("extra long bytes");
                        break;
                }
            } else break;
        } else if(_state == STATE_BYTES_DATA) {
            if(_in->has_bytes(_currentLength)) {
                unsigned char *data = new unsigned char[_currentLength];
                _in->get_bytes(data, _currentLength);
                _state = STATE_TYPE;
                _listener->on_bytes(data, _currentLength);
            } else break;
        } else if(_state == STATE_STRING_SIZE) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        _currentLength = _in->get_byte();
                        _state = STATE_STRING_DATA;
                        break;
                    case 2:
                        _currentLength = _in->get_short();
                        _state = STATE_STRING_DATA;
                        break;
                    case 4:
                        _currentLength = _in->get_int();
                        _state = STATE_STRING_DATA;
                        break;
                    case 8:
                        _state = STATE_ERROR;
                        _listener->on_error("extra long array");
                        break;
                }
            } else break;
        } else if(_state == STATE_STRING_DATA) {
            if(_in->has_bytes(_currentLength)) {
                unsigned char *data = new unsigned char[_currentLength];
                _in->get_bytes(data, _currentLength);
                _state = STATE_TYPE;
                std::string str((const char *)data, (size_t)_currentLength);
                _listener->on_string(str);
            } else break;
        } else if(_state == STATE_ARRAY) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        _listener->on_array(_in->get_byte());
                        _state = STATE_TYPE;
                        break;
                    case 2:
                        _listener->on_array(_currentLength = _in->get_short());
                        _state = STATE_TYPE;
                        break;
                    case 4:
                        _listener->on_array(_in->get_int());
                        _state = STATE_TYPE;
                        break;
                    case 8:
                        _state = STATE_ERROR;
                        _listener->on_error("extra long array");
                        break;
                }
            } else break;
        } else if(_state == STATE_MAP) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        _listener->on_map(_in->get_byte());
                        _state = STATE_TYPE;
                        break;
                    case 2:
                        _listener->on_map(_currentLength = _in->get_short());
                        _state = STATE_TYPE;
                        break;
                    case 4:
                        _listener->on_map(_in->get_int());
                        _state = STATE_TYPE;
                        break;
                    case 8:
                        _state = STATE_ERROR;
                        _listener->on_error("extra long map");
                        break;
                }
            } else break;
        } else if(_state == STATE_TAG) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        _listener->on_tag(_in->get_byte());
                        _state = STATE_TYPE;
                        break;
                    case 2:
                        _listener->on_tag(_in->get_short());
                        _state = STATE_TYPE;
                        break;
                    case 4:
                        _listener->on_tag(_in->get_int());
                        _state = STATE_TYPE;
                        break;
                    case 8:
                        _listener->on_extra_tag(_in->get_long());
                        _state = STATE_TYPE;
                        break;
                }
            } else break;
        } else if(_state == STATE_SPECIAL) {
            if (_in->has_bytes(_currentLength)) {
                switch (_currentLength) {
                    case 1:
                        _listener->on_special(_in->get_byte());
                        _state = STATE_TYPE;
                        break;
                    case 2:
                        _listener->on_special(_in->get_short());
                        _state = STATE_TYPE;
                        break;
                    case 4:
                        _listener->on_special(_in->get_int());
                        _state = STATE_TYPE;
                        break;
                    case 8:
                        _listener->on_extra_special(_in->get_long());
                        _state = STATE_TYPE;
                        break;
                }
            } else break;
        } else if(_state == STATE_ERROR) {
            break;
        } else {
            logger("UNKNOWN STATE");
        }
    }
}

