/*
   Copyright 2014-2015 Stanislav Ovsyannikov

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

           Unless required by applicable law or agreed to in writing, software
           distributed under the License is distributed on an "AS IS" BASIS,
           WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
   implied. See the License for the specific language governing permissions and
           limitations under the License.
*/

#pragma once

#include <iostream>
#include <string>

namespace cbor {

struct listener {

  virtual void on_integer(int value) const noexcept {}

  virtual void on_float32(float value) const noexcept {}
  virtual void on_double(double value) const noexcept {}

  virtual void on_bytes(unsigned char *data, int size) const noexcept {}

  virtual void on_string(std::string &str) const noexcept {}

  virtual void on_array(int size) const noexcept {}

  virtual void on_map(int size) const noexcept {}

  virtual void on_tag(unsigned int tag) const noexcept {}

  virtual void on_special(unsigned int code) const noexcept {}

  virtual void on_bool(bool) const noexcept {}

  virtual void on_null() const noexcept {}

  virtual void on_undefined() const noexcept {}

  virtual void on_error(const char *error) const noexcept {}

  virtual void on_extra_integer(unsigned long long value,
                                int sign) const noexcept {}

  virtual void on_extra_tag(unsigned long long tag) const noexcept {}

  virtual void on_extra_special(unsigned long long tag) const noexcept {}

protected:
};

template <class T> inline void p(const std::string &tag, const T &value) {
  std::cout << "[" << tag << sizeof(T) << " = " << value << "]\n";
}

template <> inline void p(const std::string &tag, const std::string &value) {
  std::cout << "[" << tag << value.length() << " = '" << value << "']\n";
}

} // namespace cbor
