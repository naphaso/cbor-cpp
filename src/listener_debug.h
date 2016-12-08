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

#ifndef __listener_debug_H_
#define __listener_debug_H_

#include <string>

#include "listener.h"

namespace cbor {
    class listener_debug : public listener {
    public:
        virtual void on_integer(int value);

        virtual void on_bytes(unsigned char *data, int size);

        virtual void on_string(std::string &str);

        virtual void on_array(int size);

        virtual void on_map(int size);

        virtual void on_tag(unsigned int tag);

        virtual void on_special(unsigned int code);
        
        virtual void on_bool(bool);
        
        virtual void on_null();
        
        virtual void on_undefined();

        virtual void on_error(const char *error);

        virtual void on_extra_integer(unsigned long long value, int sign);

        virtual void on_extra_tag(unsigned long long tag);

        virtual void on_extra_special(unsigned long long tag);
    };
}

#endif //__listener_debug_H_
