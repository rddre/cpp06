#include "ScalarConverter.hpp"

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

namespace
{
	enum LiteralType
	{
		LITERAL_CHAR,
		LITERAL_INT,
		LITERAL_FLOAT,
		LITERAL_DOUBLE,
		LITERAL_INVALID
	};

	bool isPseudoFloat(const std::string &literal)
	{
		return (literal == "nanf" || literal == "+inff" || literal == "-inff" || literal == "inff");
	}

	bool isPseudoDouble(const std::string &literal)
	{
		return (literal == "nan" || literal == "+inf" || literal == "-inf" || literal == "inf");
	}

	bool isSinglePrintableChar(const std::string &literal)
	{
		return (literal.length() == 1 && !std::isdigit(static_cast<unsigned char>(literal[0])));
	}

	bool isIntegerLiteral(const std::string &literal)
	{
		std::size_t index = 0;

		if (literal[index] == '+' || literal[index] == '-')
			++index;
		if (index >= literal.length())
			return false;
		for (; index < literal.length(); ++index)
		{
			if (!std::isdigit(static_cast<unsigned char>(literal[index])))
				return false;
		}
		return true;
	}

	bool isFloatLiteral(const std::string &literal)
	{
		return (literal.length() >= 2 && literal[literal.length() - 1] == 'f');
	}

	LiteralType detectLiteralType(const std::string &literal)
	{
		if (isSinglePrintableChar(literal))
			return LITERAL_CHAR;
		if (isPseudoFloat(literal))
			return LITERAL_FLOAT;
		if (isPseudoDouble(literal))
			return LITERAL_DOUBLE;
		if (isIntegerLiteral(literal))
			return LITERAL_INT;
		if (isFloatLiteral(literal))
			return LITERAL_FLOAT;

		char *end = NULL;
		errno = 0;
		std::strtod(literal.c_str(), &end);
		if (errno == 0 && end != NULL && *end == '\0')
			return LITERAL_DOUBLE;
		return LITERAL_INVALID;
	}

	double parseToDouble(const std::string &literal, LiteralType type, bool &ok)
	{
		ok = true;
		if (type == LITERAL_CHAR)
			return static_cast<unsigned char>(literal[0]);
		if (type == LITERAL_FLOAT && isPseudoFloat(literal))
		{
			if (literal == "nanf")
				return std::numeric_limits<double>::quiet_NaN();
			if (literal == "+inff" || literal == "inff")
				return std::numeric_limits<double>::infinity();
			return -std::numeric_limits<double>::infinity();
		}
		if (type == LITERAL_DOUBLE && isPseudoDouble(literal))
		{
			if (literal == "nan")
				return std::numeric_limits<double>::quiet_NaN();
			if (literal == "+inf" || literal == "inf")
				return std::numeric_limits<double>::infinity();
			return -std::numeric_limits<double>::infinity();
		}

		std::string toParse(literal);
		if (type == LITERAL_FLOAT && !toParse.empty())
			toParse.erase(toParse.length() - 1);

		char *end = NULL;
		errno = 0;
		double value = std::strtod(toParse.c_str(), &end);
		if (errno == ERANGE || end == NULL || *end != '\0')
		{
			ok = false;
			return 0.0;
		}
		if (type == LITERAL_FLOAT && value == value && value > std::numeric_limits<float>::max())
		{
			ok = false;
			return 0.0;
		}
		if (type == LITERAL_FLOAT && value == value && value < -std::numeric_limits<float>::max())
		{
			ok = false;
			return 0.0;
		}
		return value;
	}

	void printChar(double value)
	{
		std::cout << "char: ";
		if (value != value || value < std::numeric_limits<char>::min() || value > std::numeric_limits<char>::max())
		{
			std::cout << "impossible" << std::endl;
			return;
		}

		char c = static_cast<char>(value);
		if (!std::isprint(static_cast<unsigned char>(c)))
		{
			std::cout << "Non displayable" << std::endl;
			return;
		}

		std::cout << '\'' << c << '\'' << std::endl;
	}

	void printInt(double value)
	{
		std::cout << "int: ";
		if (value != value || value < static_cast<double>(INT_MIN) || value > static_cast<double>(INT_MAX))
		{
			std::cout << "impossible" << std::endl;
			return;
		}
		std::cout << static_cast<int>(value) << std::endl;
	}

	void printFloat(double value)
	{
		std::cout << "float: ";
		if (value != value)
		{
			std::cout << "nanf" << std::endl;
			return;
		}
		if (value == std::numeric_limits<double>::infinity())
		{
			std::cout << "inff" << std::endl;
			return;
		}
		if (value == -std::numeric_limits<double>::infinity())
		{
			std::cout << "-inff" << std::endl;
			return;
		}
		if (value > std::numeric_limits<float>::max() || value < -std::numeric_limits<float>::max())
		{
			std::cout << "impossible" << std::endl;
			return;
		}
		std::cout << std::fixed << std::setprecision(1) << static_cast<float>(value) << 'f' << std::endl;
	}

	void printDouble(double value)
	{
		std::cout << "double: ";
		if (value != value)
		{
			std::cout << "nan" << std::endl;
			return;
		}
		if (value == std::numeric_limits<double>::infinity())
		{
			std::cout << "inf" << std::endl;
			return;
		}
		if (value == -std::numeric_limits<double>::infinity())
		{
			std::cout << "-inf" << std::endl;
			return;
		}
		std::cout << std::fixed << std::setprecision(1) << value << std::endl;
	}
}

void ScalarConverter::convert(const std::string &literal)
{
	LiteralType type = detectLiteralType(literal);
	if (type == LITERAL_INVALID)
	{
		std::cout << "char: impossible" << std::endl;
		std::cout << "int: impossible" << std::endl;
		std::cout << "float: impossible" << std::endl;
		std::cout << "double: impossible" << std::endl;
		return;
	}

	bool ok = true;
	double value = parseToDouble(literal, type, ok);
	if (!ok)
	{
		std::cout << "char: impossible" << std::endl;
		std::cout << "int: impossible" << std::endl;
		std::cout << "float: impossible" << std::endl;
		std::cout << "double: impossible" << std::endl;
		return;
	}

	printChar(value);
	printInt(value);
	printFloat(value);
	printDouble(value);
}