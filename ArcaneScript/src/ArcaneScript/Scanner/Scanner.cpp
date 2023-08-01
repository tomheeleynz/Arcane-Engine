#include "Scanner.h"

namespace ArcaneScript
{
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
		return std::vector<Token>();
	}


	void Scanner::ScanToken()
	{
		char c = Advance();

		switch (c)
		{
		case ';': 
			AddToken(TokenType::SEMICOLON);
			break;
		default:
			break;
		}

	}

	char Scanner::Advance()
	{
		return m_Source[m_Current++];
	}

	void Scanner::AddToken(TokenType type)
	{
	}
	
	void Scanner::AddToken(TokenType type, std::string value)
	{
	}
	
	void Scanner::AddToken(TokenType type, float number)
	{
	}

}