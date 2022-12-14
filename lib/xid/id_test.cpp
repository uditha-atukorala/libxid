#include <gtest/gtest.h>

#include "exceptions.h"
#include "id.h"

TEST(xid, id_operators) {
	// std::ostream &operator<<(std::ostream &, const id &)
	{
		std::string s  = "cbvs04c3r49m0uvfipsg";
		xid::id     id = s;

		std::stringstream ss;
		ss << id;

		std::string actual;
		ss >> actual;

		EXPECT_EQ(s, actual);
	}

	// bool operator==(const id &that) const noexcept
	{
		xid::id id("cbvutm76i1e12923egl0");
		EXPECT_TRUE(xid::id("cbvutm76i1e12923egl0") == id);
		EXPECT_FALSE(xid::id("cbvutm76i1e12923eglg") == id);
	}

	// bool operator!=(const id &that) const noexcept;
	{
		xid::id id("cbvuucn6i1e12qo0skkg");
		EXPECT_TRUE(xid::id("cbvuucn6i1e12qo0sklg") != id);
		EXPECT_FALSE(xid::id("cbvuucn6i1e12qo0skkg") != id);
	}
}

TEST(xid, id_conversions) {
	// std::string
	{
		std::string s  = "cbvr2ks3r49kvik8ddrg";
		xid::id     id = s;

		EXPECT_EQ(s, std::string(id));
	}
}

TEST(xid, id_decode) {
	// decode
	{
		xid::id::data_t expected{
			0x62, 0xff, 0xbd, 0x97, 0x83, 0xd9, 0x13, 0x5c, 0xf8, 0x0c, 0x5f, 0x4c};

		EXPECT_EQ(expected, xid::id::decode("cbvrr5s3r49lpu0cbt60"));
	}

	// exception - xid::invalid_id_string
	{
		EXPECT_THROW(xid::id::decode(""), xid::invalid_id_string);
		EXPECT_THROW(xid::id::decode("012345678901234567891"), xid::invalid_id_string);
	}

	// exception - xid::decode_validation_failure
	{
		EXPECT_THROW(xid::id::decode("01234567890123456789"), xid::decode_validation_failure);
		EXPECT_THROW(xid::id::decode("caia5ng890f0tr00hgt="), xid::decode_validation_failure);
	}
}

TEST(xid, id_encode) {
	xid::id::data_t data{0x4d, 0x88, 0xe1, 0x5b, 0x60, 0xf4, 0x86, 0xe4, 0x28, 0x41, 0x2d, 0xc9};
	xid::id         id(data);

	EXPECT_EQ("9m4e2mr0ui3e8a215n4g", id.encode());
}

TEST(xid, id_parts) {
	struct Test {
		xid::id            id;
		xid::id::counter_t counter;
		xid::id::machine_t machine;
		xid::id::pid_t     pid;
		xid::id::time_t    time;
	};

	Test tests[] = {
		Test{
			.id =
				xid::id::data_t{
					0x4d, 0x88, 0xe1, 0x5b, 0x60, 0xf4, 0x86, 0xe4, 0x28, 0x41, 0x2d, 0xc9},
			.counter = 4271561,
			.machine = {0x60, 0xf4, 0x86},
			.pid     = 0xe428,
			.time    = std::chrono::system_clock::from_time_t(1300816219),
		},
		Test{
			.id =
				xid::id::data_t{
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
			.counter = 0,
			.machine = {0x00, 0x00, 0x00},
			.pid     = 0x00,
			.time    = std::chrono::system_clock::from_time_t(0),
		},
		Test{
			.id =
				xid::id::data_t{
					0x00, 0x00, 0x00, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x00, 0x00, 0x01},
			.counter = 1,
			.machine = {0xaa, 0xbb, 0xcc},
			.pid     = 0xddee,
			.time    = std::chrono::system_clock::from_time_t(0),
		},
	};

	for (const auto &test : tests) {
		EXPECT_EQ(test.counter, test.id.counter());
		EXPECT_EQ(test.machine, test.id.machine());
		EXPECT_EQ(test.pid, test.id.pid());
		EXPECT_EQ(test.time, test.id.time());
	}
}
