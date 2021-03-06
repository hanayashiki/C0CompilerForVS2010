#include "stdafx.h"

bool Syntax::statement() {
	if (match_type(Token::IF)) {
		//��������䣾
		return if_statement();
	}
	else if (match_pattern(pattern_assign, 2)) {
		//����ֵ��䣾
		return assign() && semicolon_handler();
	}
	else if (match_pattern(pattern_call_func, 2)) {
		// ���з���ֵ����������䣾/���޷���ֵ����������䣾 
		return call_func(NULL) && semicolon_handler();
	}
	else if (match_type(Token::LEFT_BRACE)) {
		// ��{��������У���}��
		next_token();
		if (statement_sequence() == false) {
			return false;
		}
		if (match_type(Token::RIGHT_BRACE)) {
			next_token();
		}
		else {
			error_handler("'}' is needed");
			return false;
		}
		return true;
	}
	else if (match_type(Token::SCANF)) {
		// ������䣾
		return scanf_() && semicolon_handler();
	}
	else if (match_type(Token::PRINTF)) {
		// ��д��䣾
		return printf_() && semicolon_handler();
	}
	else if (match_type(Token::SWITCH)) {
		// �������䣾
		//cout << "in to switch" << endl;
		return switch_();
	}
	else if (match_type(Token::FOR)) {
		// ��ѭ����䣾
		//cout << "in to for" << endl;
		return for_();
	}
	else if (match_type(Token::RETURN)) {
		// ��������䣾
		//cout << "in to ret" << endl;
		return return_() && semicolon_handler();
	}
	else if (match_type(Token::SEMICOLON)) {
		// ���գ�
		next_token();
		return true;
	}
	else if (match_type(Token::RIGHT_BRACE)) {
		return true;
	}

	SyntaxError::StatementException e = { "Unknown statement. " };
	throw e;
	return false;
}

bool Syntax::statement_sequence() {
	// a whole sequence of a function
	while (true) {
		statement_try();
		//cout<<"try end\n";
		//read_token.display();
		if (match_type(Token::RIGHT_BRACE)) {
			//cout << "?????\n";
			break;
		}
	}
	return true;
}

void Syntax::statement_try() {
	try {
		//cout << "statement" << endl;
		statement();
	}
	catch (SyntaxError::StatementException e) {
		//cout << "caught exception\n";
		if (!error_handler(e.what, RegexHandler::JUMP_TO_NEXT_STATEMENT)) {
			//cout << "not handled\n";
			throw e;
		}
	}
}