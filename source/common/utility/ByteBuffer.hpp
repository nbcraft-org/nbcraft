#pragma once

#include <stdint.h>
#include <assert.h>
#include "compat/LegacyCPP.hpp"

// A "vector" willing to give up its memory
class ByteBuffer
{
public:
	static const ByteBuffer EMPTY;

protected:
	void _init(ByteBuffer& other);
	void _move(ByteBuffer& other);

public:
	ByteBuffer();
	~ByteBuffer();
	MC_CTOR_MOVE_CUSTOM(ByteBuffer);

public:
	void swap(ByteBuffer& other) { assert(other.isEmpty() || other.isOrphaned()); return _move(other); }
    void assign(ByteBuffer& other);
    
	void clear();
	bool resize(size_t newSize);
	void reserve(size_t size) { resize(size); }

	void orphan() { m_bIsOrphaned = true; }
	void adopt() { m_bIsOrphaned = false; }
	bool isOrphaned() const { return m_bIsOrphaned; }

	uint8_t*& data() { return m_data; }
	const uint8_t* getData() const { return m_data; }

	size_t getSize() const { return m_size; }
	bool isEmpty() const { return m_size == 0; }

public:
	MC_FUNC_MOVE(ByteBuffer);
	uint8_t& operator[](size_t index) { return m_data[index]; }
	const uint8_t& operator[](size_t index) const { return m_data[index]; }
	operator bool() const { return m_data != nullptr; }

private:
	uint8_t* m_data;
	size_t m_size;
	bool m_bIsOrphaned; // indicates whether or not the buffer has a parent or guardian
    bool m_bIsReference;
};

