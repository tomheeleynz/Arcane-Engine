#pragma once

#include <string>
#include <vector>

#include "ArcaneScript/Token/Token.h"

namespace ArcaneScript
{
	class Scanner
	{
	public:
		Scanner(std::string source);

		std::vector<Token> ScanTokens();
	private:
		bool IsAtEnd();
		char Advance();

		void AddToken(TokenType type);

		// If i am adding a string
		void AddToken(TokenType type, std::string value);

		// If I am adding a number
		void AddToken(TokenType type, float number);

		void ScanToken();
	private:
		std::string m_Source;
		int m_Start = 0;
		int m_Current = 0;
		int m_Line = 1;
		std::vector<Token> m_Tokens;
	};
}