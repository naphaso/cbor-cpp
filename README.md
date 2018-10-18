cbor-cpp
========

[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/naphaso/cbor-cpp?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

CBOR C++ serialization library

Just a simple SAX-like Concise Binary Object Representation (CBOR).

[http://tools.ietf.org/html/rfc7049](http://tools.ietf.org/html/rfc7049)

#### Dependencies

* Boost 1.55+

#### Examples

```C++
    { //encoding
        cbor::encoder encoder(output);
		// [123, "bar", 321, 321, "foo", true, false, null, undefined, [123], [], {"hello": "world", "age": 18}, b"abcde"]
        encoder.write_array(13);
        {
            encoder.write_int(123);
            encoder.write_string("bar");
            encoder.write_int(321);
            encoder.write_int(321);
            encoder.write_string("foo");
            encoder.write_bool(true);
            encoder.write_bool(false);
            encoder.write_null();
            encoder.write_undefined();

			encoder.write_array(1);
			{
				encoder.write_int(123);
			}
			encoder.write_array(0);
			encoder.write_map(2);
			{
				encoder.write_string("hello");
				encoder.write_string("world");
				encoder.write_string("age");
				encoder.write_int(18);
			}
			encoder.write_bytes((const unsigned char*)"abcde", 5);
        }
    }

    { // decoding
        cbor::input input(output.data(), output.size());
        cbor::decoder decoder(input);
        auto result = decoder.run();
		assert(result->type == COT_ARRAY & result->array_or_map_size == 13);
		const auto& array_value = result->as<CborArrayValue>();
		auto obj1 = array_value[0]->as_int();
		auto obj2 = array_value[1]->as_string();
		auto obj3 = array_value[2]->as_int();
		auto obj4 = array_value[3]->as_int();
		auto obj5 = array_value[4]->as_string();
		auto obj6 = array_value[5]->as_bool();
		auto obj7 = array_value[6]->as_bool();
		auto obj8 = array_value[7]->is_null();
		auto obj9 = array_value[8]->is_undefined();
		auto obj10 = array_value[9]->as_array();
		auto obj11 = array_value[10]->as_array();
		auto obj12 = array_value[11]->as_map();
		auto obj13 = array_value[12]->as_bytes();
		assert(obj1 == 123);
		assert(obj2 == "bar");
		assert(obj3 == 321 && obj4 == 321);
		assert(obj5 == "foo");
		assert(obj6 == true && obj7 == false);
		assert(obj8 && obj9);
		assert(obj10.size() == 1 && obj10[0]->as_int() == 123);
		assert(obj11.empty());
		assert(obj12.size() == 2 && obj12["hello"]->as_string() == "world" && obj12["age"]->as_int() == 18);
		assert(obj13.size() == 5 && memcmp(obj13.data(), "abcde", 5) == 0);
    }
```
