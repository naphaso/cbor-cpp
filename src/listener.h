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
  virtual ~listener() = default;

  virtual void on_integer(int ) const noexcept {}

  virtual void on_float32(float ) const noexcept {}
  virtual void on_double(double ) const noexcept {}

  virtual void on_bytes(uint8_t *, int ) const noexcept {}

  virtual void on_string(const std::string &) const noexcept {}

  virtual void on_array(int ) const noexcept {}

  virtual void on_map(int ) const noexcept {}

  virtual void on_tag(unsigned int ) const noexcept {}

  virtual void on_special(unsigned int ) const noexcept {}

  virtual void on_bool(bool) const noexcept {}

  virtual void on_null() const noexcept {}

  virtual void on_undefined() const noexcept {}

  virtual void on_error(const char *) const noexcept {}

  virtual void on_extra_integer(unsigned long long ,
                                int ) const noexcept {}

  virtual void on_extra_tag(unsigned long long) const noexcept {}

  virtual void on_extra_special(unsigned long long) const noexcept {}

protected:
};

template <class T> inline void p(const std::string &tag, const T &value) {
  std::cout << "[" << tag << sizeof(T) << " = " << value << "]\n";
}

template <> inline void p(const std::string &tag, const std::string &value) {
  std::cout << "[" << tag << value.length() << " = '" << value << "']\n";
}

} // namespace cbor
