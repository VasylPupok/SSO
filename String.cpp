
#include "String.h"

#include <string>
#include <cmath>

_CONSTEXPR17 String::String() {
    this->_allocator = this->smolAllocator();
}

String::String(const char* c_str) {
    size_t length = strlen(c_str);
    if (length > _SMOL_STR_CAPACITY) {
        this->normalStrSize() = length;
        this->normalStrCapacity() = this->normalStrSize() + 1;
        this->_allocator = new char[this->normalStrCapacity()];
        memcpy(this->_allocator, c_str, this->normalStrSize() + 1);
    }
    else {
        this->_allocator = this->smolAllocator();
        memcpy(this->_allocator, c_str, length + 1);
        this->_allocator[_SMOL_STR_CAPACITY - 1] = _SMOL_STR_CAPACITY - length - 1;
    }
}

_CONSTEXPR17 String::String(const String& that)
{
    this->normalStrSize() = that.normalStrSize();
    this->normalStrCapacity() = that.normalStrCapacity();
    if (that.isSmol()) {
        this->_allocator = this->smolAllocator();
    }
    else {
        this->_allocator = new char[that.normalStrCapacity()];
        memcpy(this->_allocator, that._allocator, this->normalStrSize() + 1);
    }
}

_CONSTEXPR17 String::String(String&& that) noexcept
{
    this->normalStrSize() = that.normalStrSize();
    this->normalStrCapacity() = that.normalStrCapacity();
    this->_allocator = that._allocator;
    that._allocator = nullptr;
}



String::String(char* stackAllocator) {
    this->_allocator = this->smolAllocator();
    memcpy(this->_allocator, stackAllocator, _SMOL_STR_CAPACITY);
}

String::String(char* normalAllocator, size_t length, size_t capacity) :
    _allocator(normalAllocator),
    _strRep{length, capacity}
{}



String& String::operator+=(const char* c_str) {
    size_t length = strlen(c_str);
    size_t newLength = this->length() + length + 1;
    
    if (newLength > this->capacity()) {
        deleteAllocator();
        size_t newCapacity = this->capacity();
        while (newCapacity < newLength) {
            newCapacity *= 2;
        }
        char* newAlloc = new char[newCapacity];
        memcpy(newAlloc, this->begin(), this->length());
        memcpy(newAlloc + this->length() + 1, c_str, length + 1);
        this->normalStrSize() += length;
        this->normalStrCapacity() = newCapacity;
    }
    else {
        memcpy(this->end(), c_str, length + 1);
        if (this->isSmol()) {
            this->_allocator[_SMOL_STR_CAPACITY - 1] -= length;
        }
    }
    return *this;
}

String& String::operator+=(const String& that) {
    size_t newLength = this->length() + that.length() + 1;

    if (newLength > this->capacity()) {
        deleteAllocator();
        size_t newCapacity = this->capacity();
        while (newCapacity < newLength) {
            newCapacity *= 2;
        }
        char* newAlloc = new char[newCapacity];
        memcpy(newAlloc, this->begin(), this->length());
        memcpy(newAlloc + this->length() + 1, that.begin(), that.length() + 1);
        this->normalStrSize() += that.length();
        this->normalStrCapacity() = newCapacity;
    }
    else {
        memcpy(this->end(), that.begin(), that.length() + 1);
        if (this->isSmol()) {
            this->_allocator[_SMOL_STR_CAPACITY - 1] -= that.length();
        }
    }
    return *this;
}



std::ostream& operator<<(std::ostream& os, const String& s) {
    return os << s._allocator;
}

String operator+(const String& a, const String& b) {
    size_t newLength = a.length() + b.length();
    if (newLength > String::_SMOL_STR_CAPACITY) {
        char* allocator = new char[newLength + 1];
        memcpy(allocator, a.begin(), a.length());
        memcpy(allocator + a.length(), b.begin(), b.length() + 1);
        return { allocator, newLength, newLength + 1 };
    }
    char stackAllocator[String::_SMOL_STR_CAPACITY];
    memcpy(stackAllocator, a.begin(), a.length());
    memcpy(stackAllocator + a.length(), b.begin(), b.length() + 1);
    return { stackAllocator };
}
