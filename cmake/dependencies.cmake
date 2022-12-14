include(FetchContent)

if (LIBXID_ENABLE_TESTING)
	# googletest
	FetchContent_Declare(googletest
		URL      https://github.com/google/googletest/archive/refs/tags/release-1.12.0.tar.gz
		URL_HASH SHA256=2a4f11dce6188b256f3650061525d0fe352069e5c162452818efbbf8d0b5fe1c
	)
	FetchContent_MakeAvailable(googletest)
endif()

# OpenSSL
find_package(OpenSSL REQUIRED)

