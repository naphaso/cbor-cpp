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

#include <stdio.h>

#include "cbor.h"

int main() {
    cbor::output_dynamic output;

    { //encoding
        cbor::encoder encoder(output);
        encoder.write_array(5);
        {
            encoder.write_int(123);
            encoder.write_string("bar");
            encoder.write_int(321);
            encoder.write_int(321);
            encoder.write_string("foo");
        }
    }

    { // decoding
        cbor::input input(output.data(), output.size());
        cbor::listener_debug listener;
        cbor::decoder decoder(input, listener);
        decoder.run();
    }

    return 0;
}
