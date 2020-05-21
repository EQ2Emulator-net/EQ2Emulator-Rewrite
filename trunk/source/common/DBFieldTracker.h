#pragma once

//This template class is used for tracking changes for a db field and convert updated fields to strings for an update query

#include <type_traits>
#include <sstream>

#ifdef EQ2_WORLD
#include "../WorldServer/Database/WorldDatabase.h"
extern WorldDatabase database;
#elif defined EQ2_ZONE
#include "../ZoneServer/Database/ZoneDatabase.h"
extern ZoneDatabase database;
#endif

class DBFieldTrackerBase {
public:
	virtual ~DBFieldTrackerBase(){};

	virtual bool CheckAndAdd(std::ostringstream& ss, bool bFirst) = 0;

protected:
	DBFieldTrackerBase() = default;
};

template<typename T>
struct InvalidManip {
	int Transform(const T& v) { assert(false); return 0; }
};

template<typename T, typename Manip = InvalidManip<T> >
class DBFieldTracker : public DBFieldTrackerBase {
public:
	static_assert(!std::is_pointer_v<T>, "Dereference this pointer!");

	template<typename X, typename Y>
	static bool constexpr bHasManip = !std::is_same_v<Y, InvalidManip<X>>;

	template<typename X>
	static bool constexpr bIsString = std::is_same_v<X, std::string>;

	DBFieldTracker(const char* field, const T& ref) : field(field), currentVal(ref), lastVal(ref) {
		if (!std::is_same_v<Manip, InvalidManip<T>>) {
			manip.emplace();
		}
	}

	~DBFieldTracker() = default;

	T lastVal;
	const T& currentVal;
	const char* field;

	bool CheckAndAdd(std::ostringstream& ss, bool bFirst) override {
		bool bAdd = lastVal != currentVal;

		if (bAdd) {
			AddToQuery(ss, !bFirst);
			lastVal = currentVal;
		}

		return bAdd;
	}

	std::optional<Manip> manip;

	template<typename X>
	std::enable_if_t<bIsString<X> && !bHasManip<X, Manip>> PushValue(std::ostringstream& ss) {
		ss << '\'' << database.Escape(currentVal) << '\'';
	}

	template<typename X>
	std::enable_if_t<bHasManip<X, Manip>> PushValue(std::ostringstream& ss) {
		ss << manip->Transform(currentVal);
	}

	template<typename X>
	std::enable_if_t<std::is_floating_point_v<X> && !bHasManip<X, Manip>> PushValue(std::ostringstream& ss) {
		ss << currentVal;
	}

	template<typename X>
	std::enable_if_t<std::is_integral_v<X>&& std::is_unsigned_v<X> && !bHasManip<X, Manip>> PushValue(std::ostringstream& ss) {
		ss << static_cast<uint64_t>(currentVal);
	}

	template<typename X>
	std::enable_if_t<std::is_integral_v<X>&& std::is_signed_v<X> && !bHasManip<X, Manip>> PushValue(std::ostringstream& ss) {
		ss << static_cast<int64_t>(currentVal);
	}

	void AddToQuery(std::ostringstream& ss, bool bAddComma) {
		if (bAddComma) ss << ", ";
		ss << field << '=';
		PushValue<T>(ss);
	}
};