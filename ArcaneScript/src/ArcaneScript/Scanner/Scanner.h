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

		bool Match(char expected);
		char Peek();
		char PeekNext();

		// Adding a non-literal token
		void AddToken(TokenType type);

		// If i am adding a string
		void AddToken(TokenType type, std::string value);

		// If I am adding a number
		void AddToken(TokenType type, float number);

		void ScanToken();

		// Check for String Literal
		void StringLiteral();

		// Check for number literal
		void NumberLiteral();

		// Check If Identifie
		void IdentifierLiteral();

		// Check if is digit
		bool IsDigit(char c);
		bool IsAlpha(char c);
		bool isAlphaNumeric(char c);
	private:
		std::string m_Source;
		int m_Start = 0;
		int m_Current = 0;
		int m_Line = 1;
		std::vector<Token> m_Tokens;
	};
}