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

#ifndef __CborStaticOutput_H_
#define __CborStaticOutput_H_

#include "output.h"

namespace cbor {
    class output_static : public output {
    public:
        output_static(unsigned int capacity);

        ~output_static();

        virtual unsigned char *getData();

        virtual unsigned int getSize();

        virtual void putByte(unsigned char value);

        virtual void putBytes(const unsigned char *data, int size);

    private:
        unsigned char *buffer;
        unsigned int capacity;
        unsigned int offset;
    };
}


#endif //__CborStaticOutput_H_