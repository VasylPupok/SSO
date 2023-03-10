#pragma once

#include <iostream>

const class String {
public:
	_CONSTEXPR17 String();
	String(const char* c_str);
	_CONSTEXPR17 String(const String& that);
	_CONSTEXPR17 String(String&& that) noexcept;

	_CONSTEXPR17 String& operator=(const String& that) = default;
	_CONSTEXPR17 String& operator=(String&& that) = default;

	String& operator+=(const char* c_str);
	String& operator+=(const String& that);

	_CONSTEXPR17 ~String() { deleteAllocator(); }

	_CONSTEXPR17 char* begin() { return this->_allocator; };
	_CONSTEXPR17 char* end() { return this->_allocator + this->length(); };

	_CONSTEXPR17 const char* begin() const { return this->_allocator; };
	_CONSTEXPR17 const char* end() const { return this->_allocator + this->length(); };

	_CONSTEXPR17 size_t length() const {
		return this->isSmol() ? this->smolStrLength() : this->normalStrSize();
	}
	_CONSTEXPR17 size_t capacity() const {
		return this->isSmol() ? _SMOL_STR_CAPACITY : this->normalStrCapacity();
	}

	_CONSTEXPR17 char& operator[](size_t index) { return this->_allocator[index]; }
	_CONSTEXPR17 char operator[](size_t index) const { return this->_allocator[index]; }

private:
	_CONSTEXPR17 static const size_t _SMOL_STR_CAPACITY = sizeof(size_t) * 2;

	char* _allocator;
	union StringRep {
		struct StrInfo {
			size_t _size;
			size_t _capacity;
		} _normalStr;
		char _smolStrAlocator[_SMOL_STR_CAPACITY];
	} _strRep;

	String(char* smolAllocator);
	String(char* normalAllocator, size_t length, size_t capacity);

	_CONSTEXPR17 bool isSmol() const { return this->_allocator == this->_strRep._smolStrAlocator; }

	_CONSTEXPR17 void deleteAllocator() const {
		if (!this->isSmol()) { 
			delete[] _allocator; 
		} 
	}
	
	_CONSTEXPR17 size_t& normalStrSize() { return this->_strRep._normalStr._size; }
	_CONSTEXPR17 size_t normalStrSize() const { return this->_strRep._normalStr._size; }
	_CONSTEXPR17 size_t& normalStrCapacity() { return this->_strRep._normalStr._capacity; }
	_CONSTEXPR17 size_t normalStrCapacity() const { return this->_strRep._normalStr._capacity; }

	_CONSTEXPR17 char* smolAllocator() { return this->_strRep._smolStrAlocator; }
	_CONSTEXPR17 const char* smolAllocator() const { return this->_strRep._smolStrAlocator; }
	_CONSTEXPR17 size_t smolStrLength() const { return _SMOL_STR_CAPACITY - 1 - this->lastSmolStrByte(); }
	_CONSTEXPR17 char& lastSmolStrByte() { return this->smolAllocator()[_SMOL_STR_CAPACITY - 1]; }
	_CONSTEXPR17 char lastSmolStrByte() const { return this->smolAllocator()[_SMOL_STR_CAPACITY - 1]; }

	friend std::ostream& operator<<(std::ostream& os, const String& s);

	friend String operator+(const String& a, const String& b);
};
