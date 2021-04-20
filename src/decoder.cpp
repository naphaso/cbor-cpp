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
    _state = decoder_state::type;
}

decoder::decoder(input &in, listener &listener) {
    _in = &in;
    m_listener = &listener;
    _state = decoder_state::type;
}

decoder::~decoder() {

}

void decoder::set_listener(listener &listener_instance) {
    m_listener = &listener_instance;
}

void decoder::run() {
    unsigned int temp;
    while(1) {
        if(_state == decoder_state::type) {
            if(_in->has_bytes(1)) {
                unsigned char type = _in->get_byte();
                unsigned char majorType = type >> 5;
                unsigned char minorType = (unsigned char) (type & 31);

                switch(majorType) {
                    case 0: // positive integer
                        if(minorType < 24) {
                            m_listener->on_integer(minorType);
                        } else if(minorType == 24) { // 1 byte
                            _currentLength = 1;
                            _state = decoder_state::pint;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = decoder_state::pint;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = decoder_state::pint;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = decoder_state::pint;
                        } else {
                            _state = decoder_state::error;
                            m_listener->on_error("invalid integer type");
                        }
                        break;
                    case 1: // negative integer
                        if(minorType < 24) {
                            m_listener->on_integer(-1 -minorType);
                        } else if(minorType == 24) { // 1 byte
                            _currentLength = 1;
                            _state = decoder_state::nint;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = decoder_state::nint;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = decoder_state::nint;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = decoder_state::nint;
                        } else {
                            _state = decoder_state::error;
                            m_listener->on_error("invalid integer type");
                        }
                        break;
                    case 2: // bytes
                        if(minorType < 24) {
                            _state = decoder_state::bytes_data;
                            _currentLength = minorType;
                        } else if(minorType == 24) {
                            _state = decoder_state::bytes_size;
                            _currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = decoder_state::bytes_size;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = decoder_state::bytes_size;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = decoder_state::bytes_size;
                        } else {
                            _state = decoder_state::error;
                            m_listener->on_error("invalid bytes type");
                        }
                        break;
                    case 3: // string
                        if(minorType < 24) {
                            _state = decoder_state::string_data;
                            _currentLength = minorType;
                        } else if(minorType == 24) {
                            _state = decoder_state::string_size;
                            _currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = decoder_state::string_size;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = decoder_state::string_size;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = decoder_state::string_size;
                        } else {
                            _state = decoder_state::error;
                            m_listener->on_error("invalid string type");
                        }
                        break;
                    case 4: // array
                        if(minorType < 24) {
                            m_listener->on_array(minorType);
                        } else if(minorType == 24) {
                            _state = decoder_state::array;
                            _currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = decoder_state::array;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = decoder_state::array;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = decoder_state::array;
                        } else {
                            _state = decoder_state::error;
                            m_listener->on_error("invalid array type");
                        }
                        break;
                    case 5: // map
                        if(minorType < 24) {
                            m_listener->on_map(minorType);
                        } else if(minorType == 24) {
                            _state = decoder_state::map;
                            _currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = decoder_state::map;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = decoder_state::map;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = decoder_state::map;
                        } else {
                            _state = decoder_state::error;
                            m_listener->on_error("invalid array type");
                        }
                        break;
                    case 6: // tag
                        if(minorType < 24) {
                            m_listener->on_tag(minorType);
                        } else if(minorType == 24) {
                            _state = decoder_state::tag;
                            _currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = decoder_state::tag;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = decoder_state::tag;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = decoder_state::tag;
                        } else {
                            _state = decoder_state::error;
                            m_listener->on_error("invalid tag type");
                        }
                        break;
                    case 7: // special
                        if (minorType < 20) {
                            m_listener->on_special(minorType);
                        } else if (minorType == 20) {
                            m_listener->on_bool(false);
                        } else if (minorType == 21) {
                            m_listener->on_bool(true);
                        } else if (minorType == 22) {
                            m_listener->on_null();
                        } else if (minorType == 23) {
                            m_listener->on_undefined();
                        } else if(minorType == 24) {
                            _state = decoder_state::special;
                            _currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            _currentLength = 2;
                            _state = decoder_state::special;
                        } else if(minorType == 26) { // 4 byte
                            _currentLength = 4;
                            _state = decoder_state::special;
                        } else if(minorType == 27) { // 8 byte
                            _currentLength = 8;
                            _state = decoder_state::special;
                        } else {
                            _state = decoder_state::error;
                            m_listener->on_error("invalid special type");
                        }
                        break;
                }
            } else break;
        } else if(_state == decoder_state::pint) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        m_listener->on_integer(_in->get_byte());
                        _state = decoder_state::type;
                        break;
                    case 2:
                        m_listener->on_integer(_in->get_short());
                        _state = decoder_state::type;
                        break;
                    case 4:
                        temp = _in->get_int();
                        if(temp <= INT_MAX) {
                            m_listener->on_integer(temp);
                        } else {
                            m_listener->on_extra_integer(temp, 1);
                        }
                        _state = decoder_state::type;
                        break;
                    case 8:
                        m_listener->on_extra_integer(_in->get_long(), 1);
                        _state = decoder_state::type;
                        break;
                }
            } else break;
        } else if(_state == decoder_state::nint) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        m_listener->on_integer(-(int) _in->get_byte() - 1);
                        _state = decoder_state::type;
                        break;
                    case 2:
                        m_listener->on_integer(-(int) _in->get_short() - 1);
                        _state = decoder_state::type;
                        break;
                    case 4:
                        temp = _in->get_int();
                        if(temp <= INT_MAX) {
                            m_listener->on_integer(-(int) temp - 1);
                        } else {
                            m_listener->on_extra_integer(temp + 1, -1);
                        }
                        _state = decoder_state::type;
                        break;
                    case 8:
                        m_listener->on_extra_integer(_in->get_long() + 1, -1);
                        break;
                }
            } else break;
        } else if(_state == decoder_state::bytes_size) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        _currentLength = _in->get_byte();
                        _state = decoder_state::bytes_data;
                        break;
                    case 2:
                        _currentLength = _in->get_short();
                        _state = decoder_state::bytes_data;
                        break;
                    case 4:
                        _currentLength = _in->get_int();
                        _state = decoder_state::bytes_data;
                        break;
                    case 8:
                        _state = decoder_state::error;
                        m_listener->on_error("extra long bytes");
                        break;
                }
            } else break;
        } else if(_state == decoder_state::bytes_data) {
            if(_in->has_bytes(_currentLength)) {
                unsigned char *data = new unsigned char[_currentLength];
                _in->get_bytes(data, _currentLength);
                _state = decoder_state::type;
                m_listener->on_bytes(data, _currentLength);
            } else break;
        } else if(_state == decoder_state::string_size) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        _currentLength = _in->get_byte();
                        _state = decoder_state::string_data;
                        break;
                    case 2:
                        _currentLength = _in->get_short();
                        _state = decoder_state::string_data;
                        break;
                    case 4:
                        _currentLength = _in->get_int();
                        _state = decoder_state::string_data;
                        break;
                    case 8:
                        _state = decoder_state::error;
                        m_listener->on_error("extra long array");
                        break;
                }
            } else break;
        } else if(_state == decoder_state::string_data) {
            if(_in->has_bytes(_currentLength)) {
                unsigned char *data = new unsigned char[_currentLength];
                _in->get_bytes(data, _currentLength);
                _state = decoder_state::type;
                std::string str((const char *)data, (size_t)_currentLength);
                m_listener->on_string(str);
            } else break;
        } else if(_state == decoder_state::array) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        m_listener->on_array(_in->get_byte());
                        _state = decoder_state::type;
                        break;
                    case 2:
                        m_listener->on_array(_currentLength = _in->get_short());
                        _state = decoder_state::type;
                        break;
                    case 4:
                        m_listener->on_array(_in->get_int());
                        _state = decoder_state::type;
                        break;
                    case 8:
                        _state = decoder_state::error;
                        m_listener->on_error("extra long array");
                        break;
                }
            } else break;
        } else if(_state == decoder_state::map) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        m_listener->on_map(_in->get_byte());
                        _state = decoder_state::type;
                        break;
                    case 2:
                        m_listener->on_map(_currentLength = _in->get_short());
                        _state = decoder_state::type;
                        break;
                    case 4:
                        m_listener->on_map(_in->get_int());
                        _state = decoder_state::type;
                        break;
                    case 8:
                        _state = decoder_state::error;
                        m_listener->on_error("extra long map");
                        break;
                }
            } else break;
        } else if(_state == decoder_state::tag) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
                        m_listener->on_tag(_in->get_byte());
                        _state = decoder_state::type;
                        break;
                    case 2:
                        m_listener->on_tag(_in->get_short());
                        _state = decoder_state::type;
                        break;
                    case 4:
                        m_listener->on_tag(_in->get_int());
                        _state = decoder_state::type;
                        break;
                    case 8:
                        m_listener->on_extra_tag(_in->get_long());
                        _state = decoder_state::type;
                        break;
                }
            } else break;
        } else if(_state == decoder_state::special) {
            if (_in->has_bytes(_currentLength)) {
                switch (_currentLength) {
                    case 1:
                        m_listener->on_special(_in->get_byte());
                        _state = decoder_state::type;
                        break;
                    case 2:
                        m_listener->on_special(_in->get_short());
                        _state = decoder_state::type;
                        break;
                    case 4:
                        m_listener->on_special(_in->get_int());
                        _state = decoder_state::type;
                        break;
                    case 8:
                        m_listener->on_extra_special(_in->get_long());
                        _state = decoder_state::type;
                        break;
                }
            } else break;
        } else if(_state == decoder_state::error) {
            break;
        } else {
            logger("UNKNOWN STATE");
        }
    }
}

