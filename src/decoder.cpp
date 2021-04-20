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
    m_in = &in;
    m_state = decoder_state::type;
}

decoder::decoder(input &in, listener &listener) {
    m_in = &in;
    m_listener = &listener;
    m_state = decoder_state::type;
}

decoder::~decoder() {

}

void decoder::set_listener(listener &listener_instance) {
    m_listener = &listener_instance;
}

void decoder::run() {
    unsigned int temp;
    while(1) {
        if(m_state == decoder_state::type) {
            if(m_in->has_bytes(1)) {
                unsigned char type = m_in->get_byte();
                unsigned char majorType = type >> 5;
                unsigned char minorType = (unsigned char) (type & 31);

                switch(majorType) {
                    case 0: // positive integer
                        if(minorType < 24) {
                            m_listener->on_integer(minorType);
                        } else if(minorType == 24) { // 1 byte
                            m_currentLength = 1;
                            m_state = decoder_state::pint;
                        } else if(minorType == 25) { // 2 byte
                            m_currentLength = 2;
                            m_state = decoder_state::pint;
                        } else if(minorType == 26) { // 4 byte
                            m_currentLength = 4;
                            m_state = decoder_state::pint;
                        } else if(minorType == 27) { // 8 byte
                            m_currentLength = 8;
                            m_state = decoder_state::pint;
                        } else {
                            m_state = decoder_state::error;
                            m_listener->on_error("invalid integer type");
                        }
                        break;
                    case 1: // negative integer
                        if(minorType < 24) {
                            m_listener->on_integer(-1 -minorType);
                        } else if(minorType == 24) { // 1 byte
                            m_currentLength = 1;
                            m_state = decoder_state::nint;
                        } else if(minorType == 25) { // 2 byte
                            m_currentLength = 2;
                            m_state = decoder_state::nint;
                        } else if(minorType == 26) { // 4 byte
                            m_currentLength = 4;
                            m_state = decoder_state::nint;
                        } else if(minorType == 27) { // 8 byte
                            m_currentLength = 8;
                            m_state = decoder_state::nint;
                        } else {
                            m_state = decoder_state::error;
                            m_listener->on_error("invalid integer type");
                        }
                        break;
                    case 2: // bytes
                        if(minorType < 24) {
                            m_state = decoder_state::bytes_data;
                            m_currentLength = minorType;
                        } else if(minorType == 24) {
                            m_state = decoder_state::bytes_size;
                            m_currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            m_currentLength = 2;
                            m_state = decoder_state::bytes_size;
                        } else if(minorType == 26) { // 4 byte
                            m_currentLength = 4;
                            m_state = decoder_state::bytes_size;
                        } else if(minorType == 27) { // 8 byte
                            m_currentLength = 8;
                            m_state = decoder_state::bytes_size;
                        } else {
                            m_state = decoder_state::error;
                            m_listener->on_error("invalid bytes type");
                        }
                        break;
                    case 3: // string
                        if(minorType < 24) {
                            m_state = decoder_state::string_data;
                            m_currentLength = minorType;
                        } else if(minorType == 24) {
                            m_state = decoder_state::string_size;
                            m_currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            m_currentLength = 2;
                            m_state = decoder_state::string_size;
                        } else if(minorType == 26) { // 4 byte
                            m_currentLength = 4;
                            m_state = decoder_state::string_size;
                        } else if(minorType == 27) { // 8 byte
                            m_currentLength = 8;
                            m_state = decoder_state::string_size;
                        } else {
                            m_state = decoder_state::error;
                            m_listener->on_error("invalid string type");
                        }
                        break;
                    case 4: // array
                        if(minorType < 24) {
                            m_listener->on_array(minorType);
                        } else if(minorType == 24) {
                            m_state = decoder_state::array;
                            m_currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            m_currentLength = 2;
                            m_state = decoder_state::array;
                        } else if(minorType == 26) { // 4 byte
                            m_currentLength = 4;
                            m_state = decoder_state::array;
                        } else if(minorType == 27) { // 8 byte
                            m_currentLength = 8;
                            m_state = decoder_state::array;
                        } else {
                            m_state = decoder_state::error;
                            m_listener->on_error("invalid array type");
                        }
                        break;
                    case 5: // map
                        if(minorType < 24) {
                            m_listener->on_map(minorType);
                        } else if(minorType == 24) {
                            m_state = decoder_state::map;
                            m_currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            m_currentLength = 2;
                            m_state = decoder_state::map;
                        } else if(minorType == 26) { // 4 byte
                            m_currentLength = 4;
                            m_state = decoder_state::map;
                        } else if(minorType == 27) { // 8 byte
                            m_currentLength = 8;
                            m_state = decoder_state::map;
                        } else {
                            m_state = decoder_state::error;
                            m_listener->on_error("invalid array type");
                        }
                        break;
                    case 6: // tag
                        if(minorType < 24) {
                            m_listener->on_tag(minorType);
                        } else if(minorType == 24) {
                            m_state = decoder_state::tag;
                            m_currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            m_currentLength = 2;
                            m_state = decoder_state::tag;
                        } else if(minorType == 26) { // 4 byte
                            m_currentLength = 4;
                            m_state = decoder_state::tag;
                        } else if(minorType == 27) { // 8 byte
                            m_currentLength = 8;
                            m_state = decoder_state::tag;
                        } else {
                            m_state = decoder_state::error;
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
                            m_state = decoder_state::special;
                            m_currentLength = 1;
                        } else if(minorType == 25) { // 2 byte
                            m_currentLength = 2;
                            m_state = decoder_state::special;
                        } else if(minorType == 26) { // 4 byte
                            m_currentLength = 4;
                            m_state = decoder_state::special;
                        } else if(minorType == 27) { // 8 byte
                            m_currentLength = 8;
                            m_state = decoder_state::special;
                        } else {
                            m_state = decoder_state::error;
                            m_listener->on_error("invalid special type");
                        }
                        break;
                }
            } else break;
        } else if(m_state == decoder_state::pint) {
            if(m_in->has_bytes(m_currentLength)) {
                switch(m_currentLength) {
                    case 1:
                        m_listener->on_integer(m_in->get_byte());
                        m_state = decoder_state::type;
                        break;
                    case 2:
                        m_listener->on_integer(m_in->get_short());
                        m_state = decoder_state::type;
                        break;
                    case 4:
                        temp = m_in->get_int();
                        if(temp <= INT_MAX) {
                            m_listener->on_integer(temp);
                        } else {
                            m_listener->on_extra_integer(temp, 1);
                        }
                        m_state = decoder_state::type;
                        break;
                    case 8:
                        m_listener->on_extra_integer(m_in->get_long(), 1);
                        m_state = decoder_state::type;
                        break;
                }
            } else break;
        } else if(m_state == decoder_state::nint) {
            if(m_in->has_bytes(m_currentLength)) {
                switch(m_currentLength) {
                    case 1:
                        m_listener->on_integer(-(int) m_in->get_byte() - 1);
                        m_state = decoder_state::type;
                        break;
                    case 2:
                        m_listener->on_integer(-(int) m_in->get_short() - 1);
                        m_state = decoder_state::type;
                        break;
                    case 4:
                        temp = m_in->get_int();
                        if(temp <= INT_MAX) {
                            m_listener->on_integer(-(int) temp - 1);
                        } else {
                            m_listener->on_extra_integer(temp + 1, -1);
                        }
                        m_state = decoder_state::type;
                        break;
                    case 8:
                        m_listener->on_extra_integer(m_in->get_long() + 1, -1);
                        break;
                }
            } else break;
        } else if(m_state == decoder_state::bytes_size) {
            if(m_in->has_bytes(m_currentLength)) {
                switch(m_currentLength) {
                    case 1:
                        m_currentLength = m_in->get_byte();
                        m_state = decoder_state::bytes_data;
                        break;
                    case 2:
                        m_currentLength = m_in->get_short();
                        m_state = decoder_state::bytes_data;
                        break;
                    case 4:
                        m_currentLength = m_in->get_int();
                        m_state = decoder_state::bytes_data;
                        break;
                    case 8:
                        m_state = decoder_state::error;
                        m_listener->on_error("extra long bytes");
                        break;
                }
            } else break;
        } else if(m_state == decoder_state::bytes_data) {
            if(m_in->has_bytes(m_currentLength)) {
                unsigned char *data = new unsigned char[m_currentLength];
                m_in->get_bytes(data, m_currentLength);
                m_state = decoder_state::type;
                m_listener->on_bytes(data, m_currentLength);
            } else break;
        } else if(m_state == decoder_state::string_size) {
            if(m_in->has_bytes(m_currentLength)) {
                switch(m_currentLength) {
                    case 1:
                        m_currentLength = m_in->get_byte();
                        m_state = decoder_state::string_data;
                        break;
                    case 2:
                        m_currentLength = m_in->get_short();
                        m_state = decoder_state::string_data;
                        break;
                    case 4:
                        m_currentLength = m_in->get_int();
                        m_state = decoder_state::string_data;
                        break;
                    case 8:
                        m_state = decoder_state::error;
                        m_listener->on_error("extra long array");
                        break;
                }
            } else break;
        } else if(m_state == decoder_state::string_data) {
            if(m_in->has_bytes(m_currentLength)) {
                unsigned char *data = new unsigned char[m_currentLength];
                m_in->get_bytes(data, m_currentLength);
                m_state = decoder_state::type;
                std::string str((const char *)data, (size_t)m_currentLength);
                m_listener->on_string(str);
            } else break;
        } else if(m_state == decoder_state::array) {
            if(m_in->has_bytes(m_currentLength)) {
                switch(m_currentLength) {
                    case 1:
                        m_listener->on_array(m_in->get_byte());
                        m_state = decoder_state::type;
                        break;
                    case 2:
                        m_listener->on_array(m_currentLength = m_in->get_short());
                        m_state = decoder_state::type;
                        break;
                    case 4:
                        m_listener->on_array(m_in->get_int());
                        m_state = decoder_state::type;
                        break;
                    case 8:
                        m_state = decoder_state::error;
                        m_listener->on_error("extra long array");
                        break;
                }
            } else break;
        } else if(m_state == decoder_state::map) {
            if(m_in->has_bytes(m_currentLength)) {
                switch(m_currentLength) {
                    case 1:
                        m_listener->on_map(m_in->get_byte());
                        m_state = decoder_state::type;
                        break;
                    case 2:
                        m_listener->on_map(m_currentLength = m_in->get_short());
                        m_state = decoder_state::type;
                        break;
                    case 4:
                        m_listener->on_map(m_in->get_int());
                        m_state = decoder_state::type;
                        break;
                    case 8:
                        m_state = decoder_state::error;
                        m_listener->on_error("extra long map");
                        break;
                }
            } else break;
        } else if(m_state == decoder_state::tag) {
            if(m_in->has_bytes(m_currentLength)) {
                switch(m_currentLength) {
                    case 1:
                        m_listener->on_tag(m_in->get_byte());
                        m_state = decoder_state::type;
                        break;
                    case 2:
                        m_listener->on_tag(m_in->get_short());
                        m_state = decoder_state::type;
                        break;
                    case 4:
                        m_listener->on_tag(m_in->get_int());
                        m_state = decoder_state::type;
                        break;
                    case 8:
                        m_listener->on_extra_tag(m_in->get_long());
                        m_state = decoder_state::type;
                        break;
                }
            } else break;
        } else if(m_state == decoder_state::special) {
            if (m_in->has_bytes(m_currentLength)) {
                switch (m_currentLength) {
                    case 1:
                        m_listener->on_special(m_in->get_byte());
                        m_state = decoder_state::type;
                        break;
                    case 2:
                        m_listener->on_special(m_in->get_short());
                        m_state = decoder_state::type;
                        break;
                    case 4:
                        m_listener->on_special(m_in->get_int());
                        m_state = decoder_state::type;
                        break;
                    case 8:
                        m_listener->on_extra_special(m_in->get_long());
                        m_state = decoder_state::type;
                        break;
                }
            } else break;
        } else if(m_state == decoder_state::error) {
            break;
        } else {
            logger("UNKNOWN STATE");
        }
    }
}

