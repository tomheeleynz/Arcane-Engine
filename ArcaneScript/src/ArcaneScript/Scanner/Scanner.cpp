#include "Scanner.h"
#include <assert.h>
#include <iostream>
#include <map>

namespace ArcaneScript
{
	std::map<std::string, TokenType> ReservedTypes = {
		{"var", TokenType::VAR}
	};

	Scanner::Scanner(std::string source)
	{
		m_Source = source;
	}

	bool Scanner::IsAtEnd()
	{
		return m_Current > m_Source.length();
	}

	std::vector<Token> Scanner::ScanTokens()
	{
		while (!IsAtEnd())
		{
			m_Start = m_Current;
			ScanToken();
		}
		return m_Tokens;
	}

	void Scanner::ScanToken()
	{
		char c = Advance();

		switch (c)
		{
		case ';':
			AddToken(TokenType::SEMICOLON);
			break;
		case '=':
			AddToken(TokenType::EQUAL);
			break;
		case '/':
		{
			if (Match('/')) {
				while (Peek() != '\n' && !IsAtEnd()) m_Line++;
			}
			break;
		}
		// String Literal
		case '"':
		{
			StringLiteral();
			break;
		}
		case '\n':
			m_Line++;
			break;
		case ' ':
			break;
		case '\t':
			break;
		case '\r':
			break;
		default:
		{
			if (IsDigit(c)) {
				NumberLiteral();
			}
			else if (IsAlpha(c)) {
				IdentifierLiteral();
			}
			else {
				assert("Unexpected Token");
			}
			break;
		}
		}

	}

	void Scanner::StringLiteral()
	{
		while (Peek() != '"' && !IsAtEnd())
		{
			if (Peek() == '\n') 
				m_Line++;
			
			Advance();
		}

		if (IsAtEnd())
		{
			assert("Unterminated String");
		}

		Advance();
		std::string value = m_Source.substr(m_Start + 1, m_Current - 1);
		AddToken(TokenType::STRING, value);
	}

	void Scanner::NumberLiteral()
	{
		while (IsDigit(Peek()))
			Advance();

		if (Peek() == '.' && IsDigit(PeekNext()))
		{
			Advance();

			while (IsDigit(Peek()))
				Advance();
		}

		AddToken(TokenType::NUMBER, std::stof(m_Source.substr(m_Start, m_Current)));
	}

	void Scanner::IdentifierLiteral()
	{
		while (isAlphaNumeric(Peek()))
			Advance();

		std::string text = m_Source.substr(m_Start, m_Current);

		if (ReservedTypes.find(text) != ReservedTypes.end())
		{
			AddToken(ReservedTypes[text]);
		}
		else {
			AddToken(TokenType::IDENTIFIER);
		}
	}

	bool Scanner::Match(char expected)
	{
		if (IsAtEnd()) return false;
		if (m_Source[m_Current] != expected) return false;

		m_Current++;
		return true;
	}

	char Scanner::Peek()
	{
		if (IsAtEnd()) return '\0';
		return m_Source[m_Current];
	}

	char Scanner::PeekNext()
	{
		if (m_Current + 1 >= m_Source.length()) return '\0';
		return m_Source[m_Current + 1];
	}

	char Scanner::Advance()
	{
		return m_Source[m_Current++];
	}

	void Scanner::AddToken(TokenType type)
	{
		std::string lexeme = m_Source.substr(m_Start, m_Current);
		m_Tokens.push_back(Token(type, lexeme, m_Line));
	}

	void Scanner::AddToken(TokenType type, std::string value)
	{
		std::string lexeme = m_Source.substr(m_Start, m_Current);
		m_Tokens.push_back(Token(type, lexeme, value, m_Line));
	}

	void Scanner::AddToken(TokenType type, float number)
	{
		std::string lexeme = m_Source.substr(m_Start, m_Current);
		m_Tokens.push_back(Token(type, lexeme, number, m_Line));
	}

	bool Scanner::IsDigit(char c)
	{
		return c >= '0' && c <= '9';
	}

	bool Scanner::IsAlpha(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
	}

	bool Scanner::isAlphaNumeric(char c)
	{
		return IsAlpha(c) || IsDigit(c);
	}
}