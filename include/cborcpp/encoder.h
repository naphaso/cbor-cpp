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

#pragma once

#include "cborcpp/output.h"
#include "cborcpp/cbor_object.h"
#include <string>
#include <cstdint>

namespace cbor {
    class encoder {
    private:
        output *_out;
    public:
        encoder(output &out);

        ~encoder();

        void write_bool(bool value);
        
        void write_int(int32_t value);

        void write_int(int64_t value);

        void write_int(uint32_t value);

        void write_int(uint64_t value);

        void write_bytes(const unsigned char *data, unsigned int size);

        void write_string(const char *data, unsigned int size);

        void write_string(const std::string str);

        void write_array(int size);

        void write_map(int size);

        void write_tag(const unsigned int tag);

        void write_special(int special);
        
        void write_null();
        
        void write_undefined();

		void write_cbor_object(CborObjectP value);

    private:
        void write_type_value(int major_type, uint32_t value);

        void write_type_value(int major_type, uint64_t value);
    };
}
