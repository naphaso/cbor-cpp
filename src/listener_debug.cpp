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
    p("i", value);
}

void listener_debug::on_bytes(unsigned char *data, int size) const noexcept {
    printf("bytes with size: %d", size);
}

void listener_debug::on_string(std::string &str) const noexcept {
    p("s", str);
}

void listener_debug::on_array(int size) const noexcept {
    printf("array: %d\n", size);
}

void listener_debug::on_map(int size) const noexcept {
    printf("map: %d\n", size);
}

void listener_debug::on_tag(unsigned int tag) const noexcept {
    p("t", tag);
}

void listener_debug::on_special(unsigned int code) const noexcept {
    p("spc", code);
}

void listener_debug::on_bool(bool value) const noexcept {
    p("b", value);
}

void listener_debug::on_null() const noexcept {
    p("spc", "null");
}

void listener_debug::on_undefined() const noexcept {
    p("spc", "undef");
}

void listener_debug::on_error(const char *error) const noexcept {
    p("err", error);
}

void listener_debug::on_extra_integer(unsigned long long value, int sign) const noexcept {
    if(sign >= 0) {
        printf("extra integer: %llu\n", value);
    } else {
        printf("extra integer: -%llu\n", value);
    }
}

void listener_debug::on_extra_tag(unsigned long long tag) const noexcept {
    p("ext", tag);
}

void listener_debug::on_extra_special(unsigned long long tag) const noexcept {
    p("extspc", tag);
}
