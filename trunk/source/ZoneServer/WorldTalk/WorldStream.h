#pragma once

#include "../../common/EmuStream.h"

enum class EAuthentication {
	ENonAuth = 0,
	EAuthenticating = 1,
	EAuthGranted = 2,
	EAuthDenied = 3
};

class WorldStream : public EmuStream, public std::enable_shared_from_this<WorldStream> {
public:
	WorldStream(unsigned int ip, unsigned int port);
	~WorldStream() = default;

	void Process() override;

	void SetAuthentication(EAuthentication auth) { authentication = auth; }

private:
	EAuthentication authentication;
};