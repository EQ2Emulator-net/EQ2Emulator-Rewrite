#include "stdafx.h"

#include "BinaryStream.h"
#include <memory>
#include "EmuAssert.h"
#include "util.h"

MemoryReadStream::MemoryReadBuffer::MemoryReadBuffer(const char* s, size_t n) {
	char* ptr = (char*)s;
	setg(ptr, ptr, ptr + n);
}

MemoryReadStream::MemoryReadStream(const char* buf, size_t n)
	: std::istream(&myBuf),
	myBuf(buf, n) {

}

BinaryWriteStream::BinaryWriteStream(std::ostream& streamRef) : myStream(streamRef) {

}

BinaryReadStream::BinaryReadStream(std::istream& streamRef) : myStream(&streamRef) {

}

BinaryReadStream::BinaryReadStream(const char* buf, size_t size) : internalStream(std::in_place, buf, size), myStream(&internalStream.value()) {

}

void BinaryWriteStream::TransformBytes(void* addr, uint32_t count) {
	EmuAssert(myStream);
	EmuAssert(myStream.write(static_cast<char*>(addr), count));
}

void BinaryReadStream::TransformBytes(void* addr, uint32_t count) {
	EmuAssert(*myStream);
	EmuAssert(myStream->read(static_cast<char*>(addr), count));
}

void BinaryWriteStream::TransformString(std::string& str) {
	uint32_t size = str.size();
	BinaryStream::TransformObject(size);
	myStream.write(str.c_str(), size);
}

void BinaryReadStream::TransformString(std::string& str) {
	//Try to use stack memory, if this is a large string then we can use heap
	char tmp[2048];
	std::unique_ptr<char[]> oversizedBuf;

	uint32_t size;
	TransformObject(size);

	char* ptr;

	if (size > sizeof(tmp)) ptr = (oversizedBuf.reset(new char[size]), oversizedBuf.get());
	else ptr = tmp;

	myStream->read(ptr, size);
	str.assign(ptr, size);
}

void BinaryWriteStream::TransformBool(bool& b) {
	myStream << (b ? (uint8_t)1 : (uint8_t)0);
}

//This writes any remaining data in a read stream (from current position to end)
void BinaryWriteStream::TransformObject(BinaryReadStream& r) {
	uint32_t count = r.CountRemainingBytes();

	if (count == 0) return;

	//10 mb buf..seems reasonable
	static const uint32_t bufSize = 10 * 1024 * 1024;
	std::vector<char> buf(bufSize);

	for (;;) {
		uint32_t n;
		bool bBreak = false;
		if (count <= bufSize) {
			n = count;
			bBreak = true;
		}
		else {
			n = bufSize;
			count -= bufSize;
		}

		r.TransformBytes(buf.data(), n);
		TransformBytes(buf.data(), n);

		if (bBreak) break;
	}
}

void BinaryReadStream::TransformBool(bool& b) {
	uint8_t val;
	myStream->read(reinterpret_cast<char*>(&val), 1);
	b = (val != 0);
}

std::streampos BinaryWriteStream::Tell() {
	return myStream.tellp();
}

void BinaryWriteStream::Seek(std::streampos pos, std::ios::seekdir dir) {
	EmuAssert(myStream.seekp(pos, dir));
}

std::streampos BinaryReadStream::Tell() {
	return myStream->tellg();
}

void BinaryReadStream::Seek(std::streampos pos, std::ios::seekdir dir) {
	EmuAssert(myStream->seekg(pos, dir));
}

uint32_t BinaryReadStream::CountRemainingBytes() {
	std::streampos current = myStream->tellg();
	myStream->seekg(0, std::ios::end);
	std::streampos end = myStream->tellg();
	myStream->seekg(current, std::ios::beg);
	return static_cast<uint32_t>(end - current);
}

uint64_t BinaryReadStream::GetTotalSize() {
	std::streampos current = myStream->tellg();
	myStream->seekg(0, std::ios::beg);
	std::streampos beg = myStream->tellg();
	myStream->seekg(0, std::ios::end);
	std::streampos end = myStream->tellg();
	myStream->seekg(current, std::ios::beg);
	return end - beg;
}

void BinaryStream::TransformObject(BinaryCompressedStream& s) {
	s.Transform(*this);
}

void BinaryStream::TransformObject(BinaryCompressedWriteStream& s) {
	s.Transform(*this);
}

BinaryCompressedWriteStream::BinaryCompressedWriteStream() : BinaryWriteStream(ss) {

}

void BinaryCompressedWriteStream::Transform(BinaryStream& s) {
	EmuAssert(s.IsWriting());
	std::string uncompressedData = ss.str();

	uncompressedSize = uncompressedData.size();

	std::vector<char> compressedData;
	compressedData.resize(CalculateMaxZlibSize(uncompressedSize));

	Bytef* dest = reinterpret_cast<Bytef*>(compressedData.data());
	uLongf destLen = compressedData.size();
	const Bytef* source = reinterpret_cast<const Bytef*>(uncompressedData.data());
	EmuAssert(compress(dest, &destLen, source, static_cast<uLongf>(uncompressedSize)) == Z_OK);

	compressedSize = destLen;

	s << uncompressedSize << compressedSize;
	s.TransformBytes(compressedData.data(), compressedSize);

	bTransformed = true;
}

std::string BinaryCompressedWriteStream::ToString() {
	std::ostringstream ret;
	BinaryWriteStream s(ret);

	Transform(s);

	return std::move(ret.str());
}

BinaryCompressedReadStream::BinaryCompressedReadStream(BinaryReadStream& s) : BinaryReadStream(ENOSTREAM) {
	Transform(s);
}

BinaryReadStream BinaryCompressedReadStream::GetNewStreamForBuf() {
	return BinaryReadStream(buf.data(), buf.size());
}

void BinaryCompressedReadStream::Transform(BinaryStream& s) {
	std::vector<char> compressedData;

	uncompressedSize = buf.size();
	s << uncompressedSize;
	buf.resize(uncompressedSize);

	if (s.IsReading()) {
		s << compressedSize;
		compressedData.resize(compressedSize);
		s.TransformBytes(compressedData.data(), compressedSize);
		Bytef* dest = reinterpret_cast<Bytef*>(buf.data());
		uLongf destLen = uncompressedSize;
		const Bytef* source = reinterpret_cast<Bytef*>(compressedData.data());
		EmuAssert(uncompress(dest, &destLen, source, static_cast<uLongf>(compressedData.size())) == Z_OK);
		EmuAssert(destLen == uncompressedSize);
		this->s.emplace(buf.data(), buf.size());
		myStream = &this->s.value();
	}
	else {
		compressedData.resize(CalculateMaxZlibSize(uncompressedSize));
		Bytef* dest = reinterpret_cast<Bytef*>(compressedData.data());
		uLongf destLen = compressedData.size();
		const Bytef* source = reinterpret_cast<Bytef*>(buf.data());
		EmuAssert(compress(dest, &destLen, source, static_cast<uLongf>(buf.size())) == Z_OK);
		compressedSize = destLen;
		compressedData.resize(compressedSize);
		s << compressedSize;
		s.TransformBytes(compressedData.data(), static_cast<uint32_t>(compressedData.size()));
	}
}