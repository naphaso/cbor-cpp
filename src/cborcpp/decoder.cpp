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

#include "cborcpp/decoder.h"

#include <limits.h>

using namespace cbor;

decoder::decoder(input &in) {
    _in = &in;
    _state = STATE_TYPE;
}

decoder::~decoder() {

}

static CborObjectP cbor_object_error(const std::string& error_msg) {
	auto result = std::make_shared<CborObject>();
	result->type = CborObjectType::COT_ERROR;
	result->value = error_msg;
	return result;
}

static void put_decoded_value(CborObjectP& result, std::vector<CborObjectP>& structures_stack, bool& iter_in_map_key, CborObjectP& map_key_temp, CborObjectP value) {
	auto old_structures_stack_size = structures_stack.size();
	if (structures_stack.empty()) {
		if (result)
			throw cbor_decode_exception("multiple cbor object when decoding");
		result = value;

		if (value->type == COT_ARRAY || value->type == COT_MAP) {
			if (value->array_or_map_size > 0) {
				structures_stack.push_back(value);
			}
		}
		return;
	}
	auto last = structures_stack[old_structures_stack_size - 1];
	if (last->type == COT_ARRAY) {
		auto& array_value = boost::get<CborArrayValue>(last->value);
		array_value.push_back(value);
		if (array_value.size() >= last->array_or_map_size) {
			// full, pop from structure
			structures_stack.pop_back();
		}
	}
	else if (last->type == COT_MAP) {
		if (iter_in_map_key)
			map_key_temp = value;
		else {
			if (map_key_temp->type != COT_STRING) {
				throw cbor_decode_exception("invalid map key type");
			}
			const auto& key = boost::get<CborStringValue>(map_key_temp->value);
			auto& map_value = boost::get<CborMapValue>(last->value);
			map_value[key] = value;
			if (map_value.size() >= last->array_or_map_size) {
				// full, pop from structure
				structures_stack.pop_back();
			}
		}
		iter_in_map_key = !iter_in_map_key;
	}
	else {
		throw cbor_decode_exception("invalid structure type");
	}

	if (value->type == COT_ARRAY || value->type == COT_MAP) {
		if (value->array_or_map_size > 0) {
			structures_stack.push_back(value);
		}
	}
}

CborObjectP decoder::run() {
	CborObjectP result;
	std::vector<CborObjectP> structures_stack;
	bool iter_in_map_key = true;
	CborObjectP map_key_temp;
	
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
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_int(minorType));
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
                            throw cbor_decode_exception("invalid integer type");
                        }
                        break;
                    case 1: // negative integer
                        if(minorType < 24) {
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_int(-1 - minorType));
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
                            throw cbor_decode_exception("invalid integer type");
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
							throw cbor_decode_exception("invalid bytes type");
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
							throw cbor_decode_exception("invalid string type");
                        }
                        break;
                    case 4: // array
                        if(minorType < 24) {
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::create_array(minorType));
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
							throw cbor_decode_exception("invalid array type");
                        }
                        break;
                    case 5: // map
                        if(minorType < 24) {
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::create_map(minorType));
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
							throw cbor_decode_exception("invalid array type");
                        }
                        break;
                    case 6: // tag
                        if(minorType < 24) {
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_tag(minorType));
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
							throw cbor_decode_exception("invalid tag type");
                        }
                        break;
                    case 7: // special
                        if (minorType < 20) {
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_special(minorType));
                        } else if (minorType == 20) {
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_bool(false));
                        } else if (minorType == 21) {
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_bool(true));
                        } else if (minorType == 22) {
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::create_null());
                        } else if (minorType == 23) {
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::create_undefined());
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
							throw cbor_decode_exception("invalid special type");
                        }
                        break;
                }
            } else break;
        } else if(_state == STATE_PINT) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_int(_in->get_byte()));
                        _state = STATE_TYPE;
                        break;
                    case 2:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_int(_in->get_short()));
                        _state = STATE_TYPE;
                        break;
                    case 4:
                        temp = _in->get_int();
                        if(temp <= INT_MAX) {
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_int(temp));
                        } else {
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_extra_integer(temp, true));
                        }
                        _state = STATE_TYPE;
                        break;
                    case 8:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_extra_integer(_in->get_long(), true));
                        _state = STATE_TYPE;
                        break;
                }
            } else break;
        } else if(_state == STATE_NINT) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_int(-(int)_in->get_byte() - 1));
                        _state = STATE_TYPE;
                        break;
                    case 2:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_int(-(int) _in->get_short() - 1));
                        _state = STATE_TYPE;
                        break;
                    case 4:
                        temp = _in->get_int();
                        if(temp <= INT_MAX) {
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_int(-(int) temp - 1));
                        } else {
							put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_extra_integer(temp + 1, false));
                        }
                        _state = STATE_TYPE;
                        break;
                    case 8:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_extra_integer(_in->get_long() + 1, false));
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
						throw cbor_decode_exception("extra long bytes");
                        break;
                }
            } else break;
        } else if(_state == STATE_BYTES_DATA) {
            if(_in->has_bytes(_currentLength)) {
				std::vector<char> data(_currentLength);
                _in->get_bytes(data.data(), _currentLength);
                _state = STATE_TYPE;
				put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_bytes(data));
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
						throw cbor_decode_exception("extra long array");
                        break;
                }
            } else break;
        } else if(_state == STATE_STRING_DATA) {
            if(_in->has_bytes(_currentLength)) {
				std::vector<char> data(_currentLength);
                _in->get_bytes(data.data(), _currentLength);
                _state = STATE_TYPE;
                std::string str(data.data(), (size_t)_currentLength);
				put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_string(str));
            } else break;
        } else if(_state == STATE_ARRAY) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::create_array(_in->get_byte()));
                        _state = STATE_TYPE;
                        break;
                    case 2:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::create_array(_in->get_short()));
                        _state = STATE_TYPE;
                        break;
                    case 4:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::create_array(_in->get_int()));
                        _state = STATE_TYPE;
                        break;
                    case 8:
                        _state = STATE_ERROR;
						throw cbor_decode_exception("extra long array");
                        break;
                }
            } else break;
        } else if(_state == STATE_MAP) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::create_map(_in->get_byte()));
                        _state = STATE_TYPE;
                        break;
                    case 2:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::create_map(_currentLength = _in->get_short()));
                        _state = STATE_TYPE;
                        break;
                    case 4:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::create_map(_in->get_int()));
                        _state = STATE_TYPE;
                        break;
                    case 8:
                        _state = STATE_ERROR;
						throw cbor_decode_exception("extra long map");
                        break;
                }
            } else break;
        } else if(_state == STATE_TAG) {
            if(_in->has_bytes(_currentLength)) {
                switch(_currentLength) {
                    case 1:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_tag(_in->get_byte()));
                        _state = STATE_TYPE;
                        break;
                    case 2:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_tag(_in->get_short()));
                        _state = STATE_TYPE;
                        break;
                    case 4:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_tag(_in->get_int()));
                        _state = STATE_TYPE;
                        break;
                    case 8:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_extra_tag(_in->get_long()));
                        _state = STATE_TYPE;
                        break;
                }
            } else break;
        } else if(_state == STATE_SPECIAL) {
            if (_in->has_bytes(_currentLength)) {
                switch (_currentLength) {
                    case 1:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_special(_in->get_byte()));
                        _state = STATE_TYPE;
                        break;
                    case 2:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_special(_in->get_short()));
                        _state = STATE_TYPE;
                        break;
                    case 4:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_special(_in->get_int()));
                        _state = STATE_TYPE;
                        break;
                    case 8:
						put_decoded_value(result, structures_stack, iter_in_map_key, map_key_temp, CborObject::from_extra_special(_in->get_long()));
                        _state = STATE_TYPE;
                        break;
                }
            } else break;
        } else if(_state == STATE_ERROR) {
            break;
        } else {
			throw cbor_decode_exception("UNKNOWN STATE");
        }
    }
	if (!result)
		throw cbor_decode_exception("cbor decoded nothing");
	if (!structures_stack.empty())
		throw cbor_decode_exception("cbor decode fail with not finished structures");
	return result;
}

