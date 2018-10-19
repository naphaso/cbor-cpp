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

#include "cborcpp/input.h"
#include "cborcpp/cbor_object.h"

namespace cbor {
    typedef enum {
        STATE_TYPE,
        STATE_PINT,
        STATE_NINT,
        STATE_BYTES_SIZE,
        STATE_BYTES_DATA,
        STATE_STRING_SIZE,
        STATE_STRING_DATA,
        STATE_ARRAY,
        STATE_MAP,
        STATE_TAG,
        STATE_SPECIAL,
        STATE_ERROR
    } decoder_state;

    class decoder {
    private:
        // listener *_listener;
        input *_in;
        decoder_state _state;
        int _currentLength;
    public:
        decoder(input &in);
        ~decoder();
		CborObjectP run();
        //void set_listener(listener &listener_instance);
    };
}
