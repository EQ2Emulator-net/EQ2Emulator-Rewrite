/*  
    EQ2Emulator:  Everquest II Server Emulator
    Copyright (C) 2007  EQ2EMulator Development Team (http://www.eq2emulator.net)

    This file is part of EQ2Emulator.

    EQ2Emulator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    EQ2Emulator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with EQ2Emulator.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _CRYPTO_H
#define _CRYPTO_H

#include "RC4.h"
#include <stdint.h>

using namespace std;
class Crypto {
public:
	Crypto() {
		rc4_key = 0;
		encrypted = false;
		client = 0;
		server = 0;
	};
	~Crypto() {
		if (client)
			delete client;
		
		if (server)
			delete server;
	}

	static uint64_t RSADecrypt(const unsigned char* text, uint16_t size);
	void RC4Encrypt(unsigned char* text, uint32_t size);
	void RC4Decrypt(unsigned char* text, uint32_t size);
	uint64_t getRC4Key() { return rc4_key; }
	void setRC4Key(uint64_t key) { 
		rc4_key = key;
		if(key > 0) {
			encrypted = true; 
			client = new RC4(~key);
			server = new RC4(key); 
			unsigned char temp[20];
			client->Cypher(temp, 20);
			server->Cypher(temp, 20);
		}
		else {
			encrypted = false;
			if (client)
				delete client;
			if (server)
				delete server;
		}
	}

	uint16_t ReadRSAKey(const unsigned char* buf) {
		if (buf[0] == 0)
			setRC4Key(Crypto::RSADecrypt(buf + 62, 8));
		else
			setRC4Key(Crypto::RSADecrypt(buf + 61, 8));
		return 0;
	}

	bool isEncrypted() { return encrypted; }
	void setEncrypted(bool in_val) { encrypted = in_val; }


private:
	RC4* server;
	RC4* client;
	bool encrypted;
	uint64_t rc4_key;
};

#endif

