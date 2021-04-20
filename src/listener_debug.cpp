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

#include "listener_debug.h"

using namespace cbor;

void listener_debug::on_integer(int value) const noexcept {
    printf("integer: %d\n", value);
}

void listener_debug::on_bytes(unsigned char *data, int size) const noexcept {
    printf("bytes with size: %d", size);
}

void listener_debug::on_string(std::string &str) const noexcept {
    printf("string: '%.*s'\n", (int)str.size(), str.c_str());
}

void listener_debug::on_array(int size) const noexcept {
    printf("array: %d\n", size);
}

void listener_debug::on_map(int size) const noexcept {
    printf("map: %d\n", size);
}

void listener_debug::on_tag(unsigned int tag) const noexcept {
    printf("tag: %d\n", tag);
}

void listener_debug::on_special(unsigned int code) const noexcept {
    printf("special: %d\n", code);
}

void listener_debug::on_bool(bool value) const noexcept {
    printf("bool: %s\n", value ? "true" : "false");
}

void listener_debug::on_null() const noexcept {
    printf("special: null\n");
}

void listener_debug::on_undefined() const noexcept {
    printf("special: undefined\n");
}

void listener_debug::on_error(const char *error) const noexcept {
    printf("error: %s\n", error);
}

void listener_debug::on_extra_integer(unsigned long long value, int sign) const noexcept {
    if(sign >= 0) {
        printf("extra integer: %llu\n", value);
    } else {
        printf("extra integer: -%llu\n", value);
    }
}

void listener_debug::on_extra_tag(unsigned long long tag) const noexcept {
    printf("extra tag: %llu\n", tag);
}

void listener_debug::on_extra_special(unsigned long long tag) const noexcept {
    printf("extra special: %llu\n", tag);
}
