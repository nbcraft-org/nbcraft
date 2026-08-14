/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <iomanip>
#include "Util.hpp"

const std::string Util::EMPTY_STRING = "";

std::string Util::stringTrim(const std::string& str, const std::string& filter, bool a4, bool a5)
{
	if (str.empty() || filter.empty())
		return "";

	// don't know what the hell this does. TODO: clean this crap up
	if (!a4 && !a5)
		return "";

	int startIndex = 0, endIndex = int(str.size()) - 1;

	// @TODO: Is this accurate?
	if (a4)
	{
		while (startIndex < endIndex && strchr(filter.c_str(), str[startIndex]))
		{
			startIndex++;
		}
	}
	if (a5)
	{
		while (startIndex < endIndex && strchr(filter.c_str(), str[endIndex]))
		{
			endIndex--;
		}
	}

	return str.substr(startIndex, endIndex + 1 - startIndex);
}

std::string Util::stringTrim(const std::string& str)
{
	return stringTrim(str, " \t\n\r", true, true);
}

std::string Util::vformat(const char *fmt, va_list argPtr)
{
	char str[1024];

	vsnprintf(str, sizeof(str), fmt, argPtr);

	return std::string(str);
}

std::string Util::format(const char *fmt, ...)
{
	std::string str;
	va_list argList;

	va_start(argList, fmt);
	str = vformat(fmt, argList);
	va_end(argList);

	return str;
}

// Done this way for readability
class CUrlEncodeCharacterWhitelist
{
private:
	bool data[256];

private:
	void whitelist(uint8_t c)
	{
		data[c] = true;
	}

public:
	CUrlEncodeCharacterWhitelist()
	{
		memset(data, false, sizeof(data));

		// whitelist alphanumeric
		for (uint8_t c = 'a'; c <= 'z'; ++c)
			whitelist(c);
		for (uint8_t c = 'A'; c <= 'Z'; ++c)
			whitelist(c);
		for (uint8_t c = '0'; c <= '9'; ++c)
			whitelist(c);

		// whitelist specific cases
		whitelist('-');
		whitelist('_');
		whitelist('.');
		whitelist('~');
	}

	bool isWhitelisted(uint8_t c) const
	{
		return data[c];
	}
};

static CUrlEncodeCharacterWhitelist UrlEncodeCharacterWhitelist;

std::string Util::urlEncode(const std::string& str)
{
	if (str.empty())
		return str;

	std::string::size_type extraSize = 0;
	for (std::string::size_type i = 0; i < str.size(); ++i)
		if (!UrlEncodeCharacterWhitelist.isWhitelisted(str[i]))
			extraSize += 2; // we need an additional 2 characters ('?' to '%??')

	std::string output;
	output.reserve(str.size() + extraSize);

	char buffer[3];
	buffer[0] = '%';

	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		uint8_t c = str[i];

		if (!UrlEncodeCharacterWhitelist.isWhitelisted(c))
		{
			// write the hex representation of this character
			int cDiv16 = static_cast<int>(c) / 16;
			int cMod16 = static_cast<int>(c) % 16;

			buffer[1] = (cDiv16 <= 9) ? ('0' + cDiv16) : ('A' + cDiv16 - 10);
			buffer[2] = (cMod16 <= 9) ? ('0' + cMod16) : ('A' + cMod16 - 10);
			output.append(buffer, sizeof(buffer));
		}
		else
		{
			output += c;
		}
	}

	return output;
}

bool Util::isValidPath(const std::string& path)
{
	for (size_t i = 0; i < path.size(); i++)
	{
		switch (path[i])
		{
		case '\n':
		case '\r':
			return false;
		}
	}

	return true;
}

std::string Util::getFileName(const std::string& path)
{
	size_t namePos = 0;
	size_t nameSize = std::string::npos;

	size_t lastOf = path.find_last_of("/\\");
	if (lastOf != std::string::npos)
		namePos = lastOf + 1;

	size_t dotPos = path.rfind('.');
	if (dotPos != std::string::npos && dotPos > namePos)
		nameSize = dotPos - namePos;

	return path.substr(namePos, nameSize);
}

std::string Util::getExtension(const std::string& path)
{
	size_t dotPos = path.rfind('.');
	if (dotPos == std::string::npos)
	{
		return "";
	}

	return path.substr(dotPos + 1);
}

#define _TOSTRING_S(_type, _digits)                                                 \
std::string Util::toString(_type value)                                             \
{                                                                                   \
	if (value == 0)                                                                 \
		return "0";                                                                 \
		                                                                            \
	/* X digits, 1 for sign */                                                      \
	char buffer[_digits + 1];                                                       \
                                                                                    \
	char* ptr = &buffer[sizeof(buffer)];                                            \
                                                                                    \
	/* Use unsigned to safely handle INT_MIN */                                     \
	u ## _type absValue = static_cast<u ## _type>(value);                           \
	if (value < 0)                                                                  \
		absValue = 0 - absValue;                                                    \
                                                                                    \
	/* Build the string backwards (more efficient than calculating powers of 10) */ \
	while (absValue > 0)                                                            \
	{                                                                               \
		*(--ptr) = '0' + (absValue % 10);                                           \
		absValue /= 10;                                                             \
	}                                                                               \
                                                                                    \
	/* Add sign */                                                                  \
	if (value < 0)                                                                  \
		*(--ptr) = '-';                                                             \
                                                                                    \
	return std::string(ptr, sizeof(buffer) - (ptr - buffer));                       \
}                                                                                   \

#define _TOSTRING_U(_type, _digits)                                                 \
std::string Util::toString(_type value)                                             \
{                                                                                   \
	if (value == 0)                                                                 \
		return "0";                                                                 \
		                                                                            \
	/* X digits */                                                                  \
	char buffer[_digits];                                                           \
                                                                                    \
	char* ptr = &buffer[sizeof(buffer)];                                            \
                                                                                    \
	/* Build the string backwards (more efficient than calculating powers of 10) */ \
	while (value > 0)                                                               \
	{                                                                               \
		*(--ptr) = '0' + (value % 10);                                              \
		value /= 10;                                                                \
	}                                                                               \
                                                                                    \
	return std::string(ptr, sizeof(buffer) - (ptr - buffer));                       \
}  

_TOSTRING_S( int8_t,  3)
_TOSTRING_U(uint8_t,  3)
_TOSTRING_S( int16_t, 5)
_TOSTRING_U(uint16_t, 5)
_TOSTRING_S( int32_t, 10)
_TOSTRING_U(uint32_t, 10)
_TOSTRING_S( int64_t, 19)
_TOSTRING_U(uint64_t, 20)

std::string Util::toString(float value)
{
	std::ostringstream ss;
	ss << value;
	return ss.str();
}

std::string Util::toString(double value)
{
	std::ostringstream ss;
	ss << value;
	return ss.str();
}

#ifndef MC_NO_WSTRING

std::string Util::toString(const wchar_t* str)
{
	std::string result(wcslen(str), 0);
	wcstombs((char*)result.data(), str, result.size());
	return result;
}

std::string Util::toString(const std::wstring& str)
{
	std::string result(str.size(), 0);
	wcstombs((char*)result.data(), str.c_str(), str.size());
	return result;
}

std::wstring Util::toWideString(const std::string& str)
{
	std::wstring result(str.size(), 0);
	mbstowcs((wchar_t*)result.data(), str.c_str(), str.size());
	return result;
}

#endif
