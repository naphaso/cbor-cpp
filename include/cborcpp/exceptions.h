#pragma once

#include <string>
#include <exception>
#include <memory>
#include <cstdint>
#include <cstdint>
#include <boost/preprocessor/stringize.hpp>

namespace cbor
{
#define CBOR_DEFAULT_EXCEPTION_CODE 1

		class CborException : public std::exception
		{
		protected:
			int64_t _code;
			std::string _name_value;
			std::string _error_msg;
		public:
			inline CborException() : _code(0) { }
			inline CborException(int64_t code, const std::string &name_value, const std::string &error_msg)
				: _code(code), _name_value(name_value), _error_msg(error_msg) {}
			inline CborException(const CborException& other) {
				_code = other._code;
				_name_value = other._name_value;
				_error_msg = other._error_msg;
			}
			inline CborException(const char *msg)
			{
				_code = CBOR_DEFAULT_EXCEPTION_CODE;
				_error_msg = msg;
			}
			inline CborException(const std::string& msg) {
				_code = CBOR_DEFAULT_EXCEPTION_CODE;
				_error_msg = msg;
			}
			inline CborException& operator=(const CborException& other) {
				if (this != &other)
				{
					_error_msg = other._error_msg;
				}
				return *this;
			}
			inline virtual ~CborException() {}

#ifdef _WIN32
#ifndef CBOR_NOEXCEPT
#define CBOR_NOEXCEPT
#endif // !CBOR_NOEXCEPT
#else
#define CBOR_NOEXCEPT noexcept
#endif

			inline virtual const char* what() const CBOR_NOEXCEPT
			{
				return _error_msg.c_str();
			}
			inline virtual int64_t code() const {
				return _code;
			}
			inline std::string name() const
			{
				return _name_value;
			}
			inline virtual std::shared_ptr<CborException> dynamic_copy_exception()const
			{
				return std::make_shared<CborException>(*this);
			}
			inline virtual void dynamic_rethrow_exception()const
			{
				if (code() == 0)
					throw *this;
				else
					CborException::dynamic_rethrow_exception();
			}
		};

}

#define CBOR_DECLARE_DERIVED_EXCEPTION(TYPE, BASE, CODE, WHAT) \
	class TYPE : public BASE  \
   { \
      public: \
       explicit TYPE( int64_t code, const std::string& name_value, const std::string& what_value ) \
       :BASE( code, name_value, what_value ){} \
		explicit TYPE(const std::string& what_value ) \
       :BASE( CODE, BOOST_PP_STRINGIZE(TYPE), what_value ){} \
		explicit TYPE()	\
		:BASE(CODE, BOOST_PP_STRINGIZE(TYPE), WHAT) {} \
       virtual std::shared_ptr<cbor::CborException> dynamic_copy_exception()const \
       { return std::make_shared<TYPE>( *this ); } \
       virtual void dynamic_rethrow_exception()const \
       { if( code() == CODE ) throw *this;\
         else cbor::CborException::dynamic_rethrow_exception(); \
       } \
   };

#define CBOR_DECLARE_EXCEPTION(TYPE, CODE, WHAT) CBOR_DECLARE_DERIVED_EXCEPTION(TYPE, cbor::CborException, CODE, WHAT)

namespace cbor
{
	CBOR_DECLARE_EXCEPTION(cbor_encode_exception, 101, "cbor encode exception");
	CBOR_DECLARE_EXCEPTION(cbor_decode_exception, 102, "cbor decode exception");
	CBOR_DECLARE_EXCEPTION(cbor_output_exception, 103, "cbor output exception");
}
