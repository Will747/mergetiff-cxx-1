#ifndef _MERGETIFF_ERROR_HANDLING
#define _MERGETIFF_ERROR_HANDLING

#include <stdexcept>
#include <string>

//Determine whether C++ exception handling is enabled
#ifdef _MSC_VER
	
	//As per <https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros>,
	//MSVC defines _CPPUNWIND with a value of 1 if exception handling is enabled
	#ifdef _CPPUNWIND
		#define _MERGETIFF_USE_EXCEPTIONS 1
	#else
		#define _MERGETIFF_USE_EXCEPTIONS 0
	#endif
	
#else
	
	//For non-Microsoft compilers use the standard feature check from here:
	//<https://isocpp.org/std/standing-documents/sd-6-sg10-feature-test-recommendations>
	#if __cpp_exceptions == 199711
		#define _MERGETIFF_USE_EXCEPTIONS 1
	#else
		#define _MERGETIFF_USE_EXCEPTIONS 0
	#endif
	
#endif

namespace mergetiff {

class ErrorHandling
{
	public:
		
		//Handles an error, taking into account whether exception handling is enabled
		template <typename T>
		static inline T handleError(const T& sentinel, const std::string& message)
		{
			#if _MERGETIFF_USE_EXCEPTIONS
				throw std::runtime_error(message);
			#else
				
				//If an error logging mechanism has been specified then log the error message
				#ifdef MERGETIFF_ERROR_LOGGER
					MERGETIFF_ERROR_LOGGER((message.c_str()))
				#endif
				
				//Return the caller-defined sentinel value that indicates failure
				return sentinel;
				
			#endif
		}
		
		//Overload for just specifying a message and using a default-constructed sentinel value
		template <typename T>
		static inline T handleError(const std::string& message) {
			return ErrorHandling::handleError(T(), message.c_str());
		}
};

} //End namespace mergetiff

#endif
