#pragma once

//This template class is used for tracking changes for a db field and convert updated fields to strings for an update query

#include <type_traits>
#include <sstream>
#include "EmuAssert.h"

#ifdef EQ2_WORLD
#include "../WorldServer/Database/WorldDatabase.h"
extern WorldDatabase database;
#elif defined EQ2_ZONE
#include "../ZoneServer/Database/ZoneDatabase.h"
extern ZoneDatabase database;
#endif

struct SQLNull {
	bool operator!=(const SQLNull&) const { return false; }
};

class DBFieldTrackerBase {
public:
	virtual ~DBFieldTrackerBase(){};

	virtual bool CheckAndAdd(std::ostream& ss, bool bFirst) = 0;
	virtual std::ostream& AddWithoutCheck(std::ostream& ss, bool bUseOrigValue = false, bool bIncludeField = true) = 0;
	virtual std::string ValueToString() = 0;

	const char* field;

protected:
	DBFieldTrackerBase(const char* p_field) : field(p_field) {}
};

template<typename T>
struct InvalidManip {
	int Transform(const T& v) { EmuAssert(false); return 0; }
};

template<typename T, typename Manip = InvalidManip<T> >
class DBFieldTracker : public DBFieldTrackerBase {
public:
	static_assert(!std::is_pointer_v<T>, "Dereference this pointer!");

	template<typename X, typename Y>
	static bool constexpr bHasManip = !std::is_same_v<Y, InvalidManip<X>>;

	template<typename X>
	static bool constexpr bIsString = std::is_same_v<X, std::string>;

	DBFieldTracker(const char* field, const T& ref) : DBFieldTrackerBase(field), currentVal(ref), lastVal(ref) {
		if (!std::is_same_v<Manip, InvalidManip<T>>) {
			manip.emplace();
		}
	}

	~DBFieldTracker() = default;

	T lastVal;
	const T& currentVal;

	bool CheckAndAdd(std::ostream& ss, bool bFirst) override {
		bool bAdd = lastVal != currentVal;

		if (bAdd) {
			AddToQuery(ss, !bFirst);
			lastVal = currentVal;
		}

		return bAdd;
	}

	std::ostream& AddWithoutCheck(std::ostream& ss, bool bUseOrigValue = false, bool bIncludeField = true) override {
		AddToQuery(ss, false, bUseOrigValue, bIncludeField);
		return ss;
	}

	std::optional<Manip> manip;

	template<typename X>
	std::enable_if_t<bIsString<X> && !bHasManip<X, Manip>> PushValue(std::ostream& ss, const X& v) {
		ss << '\'' << database.Escape(v) << '\'';
	}

	template<typename X>
	std::enable_if_t<bHasManip<X, Manip>> PushValue(std::ostream& ss, const X& v) {
		ss << manip->Transform(v);
	}

	template<typename X>
	std::enable_if_t<std::is_floating_point_v<X> && !bHasManip<X, Manip>> PushValue(std::ostream& ss, const X& v) {
		ss << v;
	}

	template<typename X>
	std::enable_if_t<std::is_integral_v<X>&& std::is_unsigned_v<X> && !bHasManip<X, Manip>> PushValue(std::ostream& ss, const X& v) {
		ss << static_cast<uint64_t>(v);
	}

	template<typename X>
	std::enable_if_t<std::is_integral_v<X>&& std::is_signed_v<X> && !bHasManip<X, Manip>> PushValue(std::ostream& ss, const X& v) {
		ss << static_cast<int64_t>(v);
	}

	template<typename X>
	std::enable_if_t<std::is_same_v<X, SQLNull> && !bHasManip<X, Manip>> PushValue(std::ostream& ss, const X& v) {
		ss << "NULL";
	}

	void AddToQuery(std::ostream& ss, bool bAddComma, bool bUseOrigValue = false, bool bIncludeField = true) {
		if (bAddComma) ss << ", ";
		if (bIncludeField) ss << field << '=';
		PushValue<T>(ss, bUseOrigValue ? this->lastVal : this->currentVal);
	}

	std::string ValueToString() override {
		std::ostringstream ss;
		PushValue<T>(ss, currentVal);
		return ss.str();
	}
};