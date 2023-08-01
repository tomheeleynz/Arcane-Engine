#include "Token.h"

namespace ArcaneScript
{
	Token::Token(TokenType type, std::string lexeme, int line)
	{
		m_Type = type;
		m_Lexeme = lexeme;
		m_Line = line;
	}

	Token::Token(TokenType type, std::string lexeme, std::string value, int line)
	{
		m_Type = type;
		m_Lexeme = lexeme;
		m_StringLiteral = value;
		m_Line = line;
	}

	Token::Token(TokenType type, std::string lexeme, float value, int line)
	{
		m_Type = type;
		m_Lexeme = lexeme;
		m_NumberLiteral = value;
		m_Line = line;
	}
}