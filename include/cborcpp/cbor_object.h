#pragma once

#include <boost/variant.hpp>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include "cborcpp/exceptions.h"

namespace cbor {
	enum CborObjectType {
		COT_BOOL,
		COT_INT,
		COT_BYTES,
		COT_STRING,
		COT_ARRAY,
		COT_MAP,
		COT_TAG,
		COT_SPECIAL,
		COT_UNDEFINED,
		COT_NULL,
		COT_ERROR,
		COT_EXTRA_INT,
		COT_EXTRA_TAG,
		COT_EXTRA_SPECIAL
	};
	struct CborObject;
	typedef std::shared_ptr<CborObject> CborObjectP;
	typedef bool CborBoolValue;
	typedef int64_t CborIntValue;
	typedef uint32_t CborTagValue;
	typedef std::vector<char> CborBytesValue;
	typedef std::vector<CborObjectP> CborArrayValue;
	typedef std::map<std::string, CborObjectP, std::less<std::string>> CborMapValue;
	typedef std::string CborStringValue;
	typedef std::string CborErrorValue;
	typedef uint64_t CborExtraIntValue;
	typedef uint32_t CborSpecialValue;
	typedef uint64_t CborExtraSpecialValue;
	typedef boost::variant<CborBoolValue, CborIntValue, CborExtraIntValue, CborTagValue, CborStringValue, CborBytesValue, CborArrayValue, CborMapValue> CborObjectValue;

	struct CborObject {
		CborObjectType type;
		CborObjectValue value;
		uint32_t array_or_map_size = 0;
		bool is_positive_extra = false;

		template <typename T>
		const T& as() const {
			return boost::get<const T&>(value);
		}

		inline bool is_null() const {
			return COT_NULL == type;
		}
		inline bool is_undefined() const {
			return COT_UNDEFINED == type;
		}
		inline bool is_int() const {
			return COT_INT == type;
		}
		inline bool is_string() const {
			return COT_STRING == type;
		}
		inline bool is_bytes() const {
			return COT_BYTES == type;
		}
		inline bool is_bool() const {
			return COT_BOOL == type;
		}
		inline bool is_array() const {
			return COT_ARRAY == type;
		}
		inline bool is_map() const {
			return COT_MAP == type;
		}
		inline bool is_tag() const {
			return COT_TAG == type;
		}
		inline bool is_special() const {
			return COT_SPECIAL == type;
		}
		inline CborObjectType object_type() const {
			return type;
		}
		inline CborBoolValue as_bool() const {
			return as<CborBoolValue>();
		}
		inline CborIntValue as_int() const {
			return as<CborIntValue>();
		}
		inline CborTagValue as_tag() const {
			return as<CborTagValue>();
		}
		inline CborSpecialValue as_special() const {
			return as<CborSpecialValue>();
		}
		inline const CborBytesValue& as_bytes() const {
			return as<CborBytesValue>();
		}
		inline const CborArrayValue& as_array() const {
			return as<CborArrayValue>();
		}
		inline const CborMapValue& as_map() const {
			return as<CborMapValue>();
		}
		inline const CborStringValue& as_string() const {
			return as<CborStringValue>();
		}


		static CborObjectP from_int(CborIntValue value);
		static CborObjectP from_bool(CborBoolValue value);
		static CborObjectP from_bytes(const CborBytesValue& value);
		static CborObjectP from_string(const std::string& value);
		static CborObjectP create_array(size_t size);
		static CborObjectP create_map(size_t size);
		static CborObjectP from_tag(CborTagValue value);
		static CborObjectP create_undefined();
		static CborObjectP create_null();
		static CborObjectP from_special(uint32_t value);
		static CborObjectP from_extra_integer(uint64_t value, bool sign);
		static CborObjectP from_extra_tag(uint64_t value);
		static CborObjectP from_extra_special(uint64_t value);
	};
}