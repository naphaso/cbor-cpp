#include "cborcpp/output.h"

#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <boost/algorithm/hex.hpp>

namespace cbor {
	std::vector<unsigned char> output::bytes() const {
		std::vector<unsigned char> result(size());
		memcpy(result.data(), data(), size());
		return result;
	}

	std::string output::hex() const {
		const auto& data_bytes = bytes();
		std::string res;
		boost::algorithm::hex(data_bytes.begin(), data_bytes.end(), std::back_inserter(res));
		return res;
	}
}