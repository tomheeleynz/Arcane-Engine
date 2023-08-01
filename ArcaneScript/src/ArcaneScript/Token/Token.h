#pragma once 

#include <string>
#include "TokenTypes.h"

namespace ArcaneScript
{
	class Token
	{
	public:
		Token(TokenType type, std::string lexeme, int line);
		Token(TokenType type, std::string lexeme, std::string value, int line);
		Token(TokenType type, std::string lexeme, float value, int line);
	private:
		TokenType m_Type;
		int m_Line;
		std::string m_Lexeme;

		std::string m_StringLiteral;
		float m_NumberLiteral;
	};
}