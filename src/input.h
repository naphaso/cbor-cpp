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

#ifndef CBOR_CPP_INPUT_H
#define CBOR_CPP_INPUT_H

namespace cbor {
    class input {
    public:
        input(void *data, int size);

        ~input();

        bool hasBytes(int count);

        unsigned char getByte();

        unsigned short getShort();

        unsigned int getInt();

        unsigned long long getLong();

        void getBytes(void *to, int count);

    private:
        unsigned char *data;
        int size;
        int offset;
    };
}

#endif // CBOR_CPP_INPUT_H
