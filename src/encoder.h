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


#ifndef __CborEncoder_H_
#define __CborEncoder_H_

#include "output.h"
#include <string>

namespace cbor {
    class encoder {
    private:
        output *_out;
    public:
        encoder(output &out);

        ~encoder();

        void writeInt(int value);

        void writeInt(long long value);

        void writeInt(unsigned int value);

        void writeInt(unsigned long long value);

        void writeBytes(const unsigned char *data, unsigned int size);

        void writeString(const char *data, unsigned int size);

        void writeString(const std::string str);

        void writeArray(int size);

        void writeMap(int size);

        void writeTag(const unsigned int tag);

        void writeSpecial(int special);

    private:
        void writeTypeAndValue(int majorType, unsigned int value);

        void writeTypeAndValue(int majorType, unsigned long long value);
    };
}

#endif //__CborEncoder_H_
