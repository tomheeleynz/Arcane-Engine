#pragma once

namespace ArcaneScript
{
	enum class TokenType
	{
		EQUAL,
		VAR, PRINT,
		// Single Character Tokens
		SEMICOLON,
		// Literals
		NUMBER, STRING, IDENTIFIER,
		END_OF_FILE
	};
}