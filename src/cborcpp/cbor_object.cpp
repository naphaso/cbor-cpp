#include "cborcpp/cbor_object.h"

namespace cbor {
	CborObjectP CborObject::from_int(CborIntValue value) {
		auto result = std::make_shared<CborObject>();
		result->type = COT_INT;
		result->value = value;
		return result;
	}
	CborObjectP CborObject::from_bool(CborBoolValue value) {
		auto result = std::make_shared<CborObject>();
		result->type = COT_BOOL;
		result->value = value;
		return result;
	}
	CborObjectP CborObject::from_bytes(const CborBytesValue& value) {
		auto result = std::make_shared<CborObject>();
		result->type = COT_BYTES;
		result->value = value;
		return result;
	}
	CborObjectP CborObject::from_string(const std::string& value) {
		auto result = std::make_shared<CborObject>();
		result->type = COT_STRING;
		result->value = value;
		return result;
	}
	CborObjectP CborObject::create_array(size_t size) {
		auto result = std::make_shared<CborObject>();
		result->type = COT_ARRAY;
		result->value = CborArrayValue();
		result->array_or_map_size = size;
		return result;
	}
	CborObjectP CborObject::create_map(size_t size) {
		auto result = std::make_shared<CborObject>();
		result->type = COT_MAP;
		result->value = CborMapValue();
		result->array_or_map_size = size;
		return result;
	}
	CborObjectP CborObject::from_tag(CborTagValue value) {
		auto result = std::make_shared<CborObject>();
		result->type = COT_TAG;
		result->value = value;
		return result;
	}
	CborObjectP CborObject::create_undefined() {
		auto result = std::make_shared<CborObject>();
		result->type = COT_UNDEFINED;
		return result;
	}
	CborObjectP CborObject::create_null() {
		auto result = std::make_shared<CborObject>();
		result->type = COT_NULL;
		return result;
	}
	CborObjectP CborObject::from_special(uint32_t value) {
		auto result = std::make_shared<CborObject>();
		result->type = COT_SPECIAL;
		result->value = value;
		return result;
	}
	CborObjectP CborObject::from_extra_integer(uint64_t value, bool sign) {
		auto result = std::make_shared<CborObject>();
		result->type = COT_EXTRA_INT;
		result->value = value;
		result->is_positive_extra = sign;
		return result;
	}
	CborObjectP CborObject::from_extra_tag(uint64_t value) {
		auto result = std::make_shared<CborObject>();
		result->type = COT_EXTRA_TAG;
		result->value = value;
		return result;
	}
	CborObjectP CborObject::from_extra_special(uint64_t value) {
		auto result = std::make_shared<CborObject>();
		result->type = COT_EXTRA_SPECIAL;
		result->value = value;
		return result;
	}
}