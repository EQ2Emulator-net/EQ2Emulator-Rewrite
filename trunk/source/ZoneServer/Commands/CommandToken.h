#pragma once

#include <cstdint>
#include <string>
#include <utility>

class CommandToken {
public:
	CommandToken() : bitmask(0) {}

	CommandToken(const std::string str) {
		Parse(str);
	}

	bool Parse(const std::string str) {
		bitmask = 0;
		char lastToken = 0;
		bool bRange = false;

		for (const char& c : str) {
			if (!IsValidTokenCharacter(c)) {
				bitmask = 0;
				return false;
			}

			if (bRange) {
				if (c == '-') {
					bitmask = 0;
					return false;
				}

				uint64_t mask1 = GetTokenMask(lastToken);
				uint64_t mask2 = GetTokenMask(c);

				if (mask1 > mask2) {
					std::swap(mask1, mask2);
				}

				while (mask2 >= mask1) {
					bitmask |= mask2;
					mask2 >>= 1;
				}

				bRange = false;
				lastToken = c;
				continue;
			}
			else if (c == '-') {
				lastToken = c;
				bRange = true;
				continue;
			}

			lastToken = c;
			bitmask |= GetTokenMask(c);
		}

		if (bRange) {
			bitmask = 0;
			return false;
		}

		return true;
	}

	bool IsValidTokenCharacter(char c) {
		return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '-');
	}

	bool MatchesAllTokens(const CommandToken& other) {
		return (bitmask & other.bitmask) == bitmask;
	}

private:
	uint64_t bitmask;

	uint64_t GetTokenMask(char c) {
		uint32_t shift = 0;
		if (c >= '0' && c <= '9') {
			shift = c - '0';
		}
		else if (c >= 'A' && c <= 'Z') {
			shift = c - 'A' + 10;
		}
		else if (c >= 'a' && c <= 'z') {
			shift = c - 'a' + 36;
		}
		return (1ull << shift);
	}
};