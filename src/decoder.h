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

#include "listener.h"
#include "input.h"

namespace cbor {
    
    enum class decoder_state : uint8_t {
        type,
        pint,
        nint,
        bytes_size,
        bytes_data,
        string_size,
        string_data,
        array,
        map,
        tag,
        special,
        error
    };

    struct decoder {
        void run(input& input, listener& listener);
    private:
        decoder_state m_state;
    };
}
