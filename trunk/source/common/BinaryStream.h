#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <sstream>
#include <zlib.h>
#include <optional>

#include "EmuAssert.h"

class MemoryReadStream : public std::istream {
public:
	MemoryReadStream(const char* buf, size_t size);
	~MemoryReadStream() = default;
private:

	class MemoryReadBuffer : public std::streambuf {
	public:
		MemoryReadBuffer(const char* buf, size_t size);
		~MemoryReadBuffer() = default;

		pos_type seekoff(off_type offset, ios_base::seekdir dir, ios_base::openmode) override {
			if (dir == std::ios::end || dir == std::ios::beg) {
				setg(eback(), eback(), egptr());
			}

			std::streamsize n = egptr() - gptr();

			if (n <= offset) {
				return -1;
			}

			if (dir == std::ios::end) {
				offset = n - offset - 1;
			}

			gbump(static_cast<int>(offset));

			return gptr() - eback();
		}

		pos_type seekpos(pos_type offset, std::ios_base::openmode which = std::ios::in) override {
			return seekoff(offset, std::ios::beg, which);
		}
	};

	MemoryReadBuffer myBuf;
};

namespace is_stl_vector_impl {
	template <typename T> struct is_stl_vector : std::false_type {};
	template <typename... Args> struct is_stl_vector<std::vector <Args...>> : std::true_type {};
};

template <typename T> struct is_stl_vector {
	static constexpr bool const value = is_stl_vector_impl::is_stl_vector<std::decay_t<T>>::value;
};

namespace is_smart_ptr_impl {
	template <typename T> struct is_smart_ptr : std::false_type {};
	template <typename... Args> struct is_smart_ptr<std::unique_ptr<Args...>> : std::true_type {};
	template <typename... Args> struct is_smart_ptr<std::shared_ptr<Args...>> : std::true_type {};
}

template <typename T> struct is_smart_ptr {
	static constexpr bool const value = is_smart_ptr_impl::is_smart_ptr<std::decay_t<T>>::value;
};

class BinaryStream {
protected:
	BinaryStream() : bUsePrimitiveTable(false) {}

public:
	virtual ~BinaryStream() = default;

	bool bUsePrimitiveTable;

private:
	template<typename T> inline void TransformObjectInternal(T& obj) {
		static_assert(!std::is_pointer<std::remove_reference<T>::type>::value, "VGStream::TransformObject should not take a raw pointer!");
		TransformBytes(&obj, sizeof(T));
	}

	template<typename T> std::enable_if_t<is_stl_vector<T>::value> TransformVector(T& obj) {
		uint32_t count = static_cast<uint32_t>(obj.size());
		*this << count;
		obj.resize(count);
		for (uint32_t i = 0; i < count; i++) {
			*this << obj[i];
		}
	}

	template<typename T> std::enable_if_t<!is_stl_vector<T>::value> TransformVector(T& obj) {
		EmuAssert(false);
	}

	//If your smart pointer will not serialize this way, feel free to add an overload to TransformObject using that pointer type
	template <typename T> std::enable_if_t<is_smart_ptr<T>::value> TransformPointer(T& ptr) {
		if (IsReading()) {
			ptr.reset(new T::template element_type(*static_cast<class BinaryReadStream*>(this)));
		}
		else {
			EmuAssert(ptr);
			*this << *ptr;
		}
	}

	template <typename T> std::enable_if_t<!is_smart_ptr<T>::value> TransformPointer(T& ptr) {
		EmuAssert(false);
	}

public:
	template<typename T> void TransformObject(T& obj) {
		//Overload to detect if T is a std::vector, if so automatically transform the array count and objects in the array
		if (is_stl_vector<T>::value) {
			TransformVector(obj);
		}
		//Overload to detect if this is a smart pointer, try to auto allocate/serialize as needed. Overload the type manually otherwise
		else if (is_smart_ptr<T>::value) {
			TransformPointer(obj);
		}
		else {
			TransformObjectInternal(obj);
		}
	}

	void TransformObject(std::string& str) {
		TransformString(str);
	}

	void TransformObject(bool& obj) {
		TransformBool(obj);
	}

	template<typename T> BinaryStream& operator<<(T& obj) {
		TransformObject(obj);
		return *this;
	}

	virtual bool IsReading() { return false; }
	virtual bool IsWriting() { return false; }
	virtual std::streampos Tell() = 0;
	virtual void Seek(std::streampos, std::ios::seekdir) = 0;

	//If any class requires custom transformation declare it here then define the function elsewhere 
	//(not aligned to 4, multiple members, class with virtual functions etc)

	void TransformObject(class BinaryCompressedStream&);
	void TransformObject(class BinaryCompressedWriteStream&);

	virtual void TransformBytes(void* addr, uint32_t count) = 0;

	virtual void TransformString(std::string& str) = 0;

	virtual void TransformBool(bool& obj) = 0;
};


//VGWriteStream assumes "myStream" will stay valid until this object is destroyed.
class BinaryWriteStream : public BinaryStream {
public:
	BinaryWriteStream(std::ostream& streamRef);
	~BinaryWriteStream() = default;

	void TransformBytes(void* addr, uint32_t count) override final;

	void TransformString(std::string& s) override final;

	//Does a static cast style contruct on the supplied arg to type T and transforms it into the stream
	template<typename T, typename A> void TransformAs(A arg) {
		T t = static_cast<T>(arg);
		*this << t;
	}

	void TransformBool(bool& b) override final;

	bool IsWriting() override { return true; }

	void TransformObject(class BinaryReadStream&);

	std::streampos Tell() override;

	void Seek(std::streampos, std::ios::seekdir) override;

	template<typename T> BinaryStream& operator<<(T& obj) {
		BinaryStream::TransformObject(obj);
		return *this;
	}

	BinaryWriteStream& operator<<(class BinaryReadStream& s) {
		TransformObject(s);
		return *this;
	}

private:
	std::ostream& myStream;
};

//VGReadStream assumes "myStream" will stay valid until this object is destroyed.
class BinaryReadStream : public BinaryStream {
	friend class BinaryWriteStream;
public:
	BinaryReadStream(const char* buf, size_t size);
	BinaryReadStream(std::istream& streamRef);
	~BinaryReadStream() = default;

	void TransformBytes(void* addr, uint32_t count) override;

	void TransformString(std::string& s) override;

	template<typename T> T TransformInto() {
		T ret;
		*this << ret;
		return ret;
	}

	std::streampos Tell() override;

	void Seek(std::streampos, std::ios::seekdir) override;

	void TransformBool(bool& b) override final;

	bool IsReading() override { return true; }

	uint32_t CountRemainingBytes();
	uint64_t GetTotalSize();

private:
	std::optional<MemoryReadStream> internalStream;

protected:
	enum ENoStream { ENOSTREAM};
	std::istream* myStream;

protected:
	BinaryReadStream(ENoStream empty) : myStream(nullptr) {}
};

class BinaryCompressedStream {
protected:
	BinaryCompressedStream() : uncompressedSize(0), compressedSize(0), bTransformed(false) {}

public:
	virtual ~BinaryCompressedStream() = default;
	virtual void Transform(BinaryStream& s) = 0;

	uint32_t uncompressedSize;
	uint32_t compressedSize;
	bool bTransformed;
};

class BinaryCompressedReadStream : public BinaryCompressedStream, public BinaryReadStream {
public:
	BinaryCompressedReadStream(BinaryReadStream& s);
	~BinaryCompressedReadStream() = default;

	void Transform(BinaryStream& s) final override;

	BinaryReadStream GetNewStreamForBuf();

private:
	std::optional<MemoryReadStream> s;
	std::vector<char> buf;
};

class BinaryCompressedWriteStream : public BinaryCompressedStream, public BinaryWriteStream {
public:
	BinaryCompressedWriteStream();
	~BinaryCompressedWriteStream() = default;

	void Transform(BinaryStream& s) final override;

	std::string ToString();

private:
	std::ostringstream ss;
};