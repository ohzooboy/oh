#include <afxwin.h>
#include <afx.h>
#include <boost/spirit.hpp> 
#include <boost/spirit/iterator/position_iterator.hpp>
#include <assert.h>
#include <fstream> 
#include <iostream> 
#include <sstream> 
#include <stdio.h>
#include "CodeSnippets.h"
#include "CSymbolTable.h"


const bool k_assert_this_must_not_happen = false;

using namespace boost::spirit;

// http://www.highscore.de/cpp/boost/parser.html
// http://www.boost.org/doc/libs/1_35_0/libs/spirit/doc/operators.html
// http://www.boost.org/doc/libs/1_34_1/libs/spirit/doc/primitives.html
// http://beta.boost.org/doc/libs/1_41_0/libs/spirit/doc/html/spirit/qi/reference/char/char.html
// http://www.johannes-asal.de/?p=107 (danish)
// http://www.ibm.com/developerworks/aix/library/au-boost_parser/index.html

int bracket_counter = 0;

// skip grammar
struct skip_grammar : public grammar<skip_grammar>
{
	template <typename ScannerT>
	struct definition
	{
		definition(skip_grammar const& /*self*/)
		{
			skip
				=   space_p
				|   "//" >> *(anychar_p - eol_p - end_p) >> (eol_p | end_p)	 // C++ comment
				|   "/*" >> *(anychar_p - "*/") >> "*/"	// C comment
				|	as_lower_d["in"]					// Useless fill-word "In SmallBlind"
				;
		}

		rule<ScannerT> skip;

		rule<ScannerT> const&

		start() const { return skip; }
	};
};


struct json_grammar: public boost::spirit::grammar<json_grammar> 
{ 
	struct print_number 
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::cout << std::string(begin, end); 
		} 
	};

	struct print_header
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::cout << k_code_snippet_license; 
		} 
	};

	struct print_options
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::cout << k_code_snippet_options; 
		} 
	};

	struct print_predefined_action_constants
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::cout << k_code_snippet_predefined_constants; 
		} 
	};

	struct print_technical_functions
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::cout << k_code_snippet_technical_functions; 
		} 
	};

	struct print_newline
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::cout << std::endl; 
		} 
	};

	struct print_function_header_for_betting_round
	{
		void operator()(const char *begin, const char *end) const 
		{ 
			std::string text = std::string(begin, end);
			if (text == "preflop")
			{
				std::cout << "##f$preflop##\n"; 
			}
			else if (text == "flop")
			{
				std::cout << "##f$flop##\n"; 
			}
			else if (text == "turn")
			{
				std::cout << "##f$turn##\n"; 
			}
			else if (text == "river")
			{
				std::cout << "##f$river##\n"; 
			}
			else
			{
				//std::cout << "##f$" << text << "##\n";
				assert(k_assert_this_must_not_happen);
			}
		} 
	};

	struct print_operator 
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::string text = std::string(begin, end);
			std::cout << " " << text << " ";
			return;
			// Some operators have to be translated,
			// as they are named differently in OpenPPL and OH-script.
			if (text == "and")
			{
				std::cout << "&&";
			}
			else if (text == "or")
			{
				std::cout << "||";
			}
			else if (text == "xor")
			{
				std::cout << "^^";
			}
			else if (text == "not")
			{
				std::cout << "!";
			}
			else if (text == "bitand")
			{
				std::cout << "&";
			}
			else if (text == "bitor")
			{
				std::cout << "|";
			}
			else if (text == "bitxor")
			{
				std::cout << "^";
			}
			else if (text == "bitnot")
			{
				std::cout << "~";
			}
			else if (text == "=")
			{
				std::cout << "==";
			}
			else if (text == "mod")
			{
				// Modulo needs special treatment,
				// as "%" gets used as the percentage-operator in OpenPPL.
				std::cout << "%";
			}
			else
			{
				// No translation necessary
				// The operators are names the same way in both languages.
				std::cout << text;
			}
			std::cout << " ";
		} 
	};

	struct print_keyword
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			if (1/*p_symbol_table->IsOpenPPLSymbol()*/)
			{
				// OpenPPL symbols and user-defined symbols 
				// get translated to f$...OH-script-symbols.
				// So we prepend "f$"
				std::cout << "f$";
			}
			std::cout << std::string(begin, end); 
		} 
	}; 

	struct print_bracket 
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::string text = std::string(begin, end);
			if (text == "(")
			{
				char open_brackets[4] = "[({";
				std::cout << open_brackets[bracket_counter%3];
				bracket_counter++;
			}
			else
			{
				bracket_counter--;
				char close_brackets[4] = "])}";
				std::cout << close_brackets[bracket_counter%3];
				bracket_counter++;
			}
		} 
	};

	struct print_percentage_operator
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::cout << "/100 * "; 
		} 
	};

	struct print_relative_potsize_action
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::cout << "pot"; 
		} 
	};

	struct print_questionmark_for_condition
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::cout << " ? "; 
		} 
	};

	struct print_colon_for_condition
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::cout << " :"; 
		} 
	};

	struct error_beep_not_supported
	{
		void operator()(const char *begin, const char *end) const 
		{
			MessageBox(0, "Error: \"Beep\" is no valid action for OpenPPL, as\n\n"
				"* OpenHoldem is meant as a bot, not as a tool for manual play\n"
				"* Human and bot competing for the mouse just calls for troubles.\n\n"
				"Please complete your profile so that all actions are specified.",
				"Error", 0);
		}
	};

	struct print_fold_as_last_alternative_for_when_condition_sequence
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::cout << "//\n"
				"// Undefined action\n"
				"// No backup action available (no built-in default bot)\n"
				"// Going to fold\n"
				"//\n"
				"f$Action_Fold\n\n"; 
		} 
	};

	struct print_predefined_action
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			std::string text = std::string(begin, end);
			std::transform(begin, end, text.begin(), std::tolower);
			if ((text == "call") || (text == "play"))
			{
				std::cout << " f$Action_Call";
			}
			else if ((text == "raisemin") || (text == "betmin"))
			{
				std::cout << " f$Action_RaiseMin";
			}
			else if ((text == "raisehalfpot") || (text == "bethalfpot"))
			{
				std::cout << " f$Action_RaiseHalfPot";
			}
			else if ((text == "raisepot") || (text == "betpot"))
			{
				std::cout << " f$Action_RaisePot";
			}
			else if ((text == "raisemax") || (text == "betmax"))
			{
				std::cout << " f$Action_RaiseMax";
			}
			else if ((text == "raise") || (text == "bet"))
			{
				std::cout << " f$Action_Raise";
			}
			else if (text == "fold")
			{
				std::cout << " f$Action_Fold";
			}
			else if (text == "sitout")
			{
				std::cout << " f$Action_SitOut";
			}
			// Beep not supported and handled otherwhere.
		} 
	};

	template <typename Scanner>

	struct definition 
	{ 
		boost::spirit::rule<Scanner> openPPL_code, option_settings_to_be_ignored, 
			single_option, option_name, option_value,

			symbol_section,
			preflop_section, 
			flop_section, 
			turn_section, 
			river_section, 
			symbol_definition,
			when_section,
			when_section_r,
			when_condition_sequence_with_action,
			when_condition_sequence_without_action,
			//when_condition_sequence_with_action_and_fold_force,
			when_condition_with_action,
			when_condition_without_action,
			when_condition_with_return_statement,
			when_condition_sequence_with_return_statement,
			when_condition_without_return_statement,
			when_condition_sequence_without_return_statement,
			return_statement,
			when_condition,
			condition,
			expression,
			primary_expression,
			
			terminal_expression,		
			bracket_expression,
			operand_expression,
			unary_operator,
			unary_expression,
			percentage_operator,
			multiplicative_operator,
			multiplicative_expression,
			additive_operator,
			additive_expression,
			relational_operator,
			relational_expression,
			equality_expression,
			and_expression,
			xor_expression,
			or_expression,
			action,
			predefined_action,
			fixed_betsize_action,
			relative_betsize_action,
			keyword_predefined_action,
			number,
			boolean_constant,
			symbol,

			// Keywords
			keyword_when,
			keyword_on,
			keyword_off,
			keyword_custom,
			keyword_symbols,
			keyword_preflop,
			keyword_flop,
			keyword_turn,
			keyword_river,
			keyword_not,
			keyword_and,
			keyword_xor,
			keyword_or,
			keyword_true,
			keyword_false,
			keyword_force,
			keyword_beep,
			keyword_call,
			keyword_play,
			keyword_raise,
			keyword_raisemin,
			keyword_raisehalfpot,
			keyword_raisepot,
			keyword_raisemax,
			keyword_fold,
			keyword_bet,
			keyword_betmin,
			keyword_bethalfpot,
			keyword_betpot,
			keyword_betmax,
			keyword_sitout,
			keyword_others,
			when_others_fold_force,
			when_others_when_others,
			card_constant,
			keyword_suited,
			keyword_hand,
			keyword_board,
			keyword_new,
			keyword_symbol,
			keyword_return,
			keyword_end,
			card_expression,
			board_expression,
			hand_expression
			;

		definition(const json_grammar &self) 
		{ 
			using namespace boost::spirit; 
			// Whole PPL-file
			openPPL_code = option_settings_to_be_ignored
				>> keyword_custom [print_header()][print_options()]
				>> !symbol_section
				>> preflop_section [print_newline()] [print_newline()]
				>> flop_section [print_newline()][print_newline()] 
				>> turn_section [print_newline()][print_newline()] 
				>> river_section [print_newline()][print_newline()]
				>> end_p[print_predefined_action_constants()][print_technical_functions()]; 
			  
			// Option settings - to be ignored
			option_settings_to_be_ignored = *single_option;
			single_option = option_name >> '=' >> option_value;
			option_name = alpha_p >> *(alnum_p | "-");
			keyword_on = str_p("on") | "On" | "ON";
			keyword_off = str_p("off") | "Off" | "OFF";
			option_value = keyword_on | keyword_off | number;

			// Start of custom code
			keyword_custom = str_p("custom") | "Custom" | "CUSTOM";

			// User defined symbols
			symbol_section = keyword_symbols >> *symbol_definition;
			keyword_new = str_p("new") | "New";
			keyword_symbols = str_p("symbols") | "Symbols";
			keyword_symbol = str_p("symbol") | "Symbol";
			keyword_end = str_p("end") | "End";
			symbol_definition = keyword_new >> keyword_symbol
				>> symbol//[print_function_header_for_betting_round()] 
				>> when_section_r
				>> keyword_end >> keyword_symbol;


			// Preflop, flop, turn, river
			preflop_section = keyword_preflop[print_function_header_for_betting_round()] >> when_section;
			keyword_preflop = str_p("preflop") | "Preflop" | "PREFLOP";
			flop_section = keyword_flop[print_function_header_for_betting_round()] >> when_section;
			keyword_flop = str_p("flop") | "Flop" | "FLOP";
			turn_section = keyword_turn[print_function_header_for_betting_round()] >> when_section;
			keyword_turn = str_p("turn") | "Turn" | "TURN";
			river_section = keyword_river[print_function_header_for_betting_round()] >> when_section;
			keyword_river = str_p("river") | "River" | "RIVER";
			when_section = !when_condition_sequence_with_action 
				>> when_condition_sequence_without_action[print_fold_as_last_alternative_for_when_condition_sequence()];

			when_section_r = !when_condition_sequence_with_return_statement 
				>> when_condition_sequence_without_return_statement[print_fold_as_last_alternative_for_when_condition_sequence()];

			// When-condition sequences (with action)
			keyword_when = str_p("when") | "When" | "WHEN";
			when_condition = keyword_when >> condition;
			when_condition_with_action = when_condition[print_questionmark_for_condition()]
				>> action[print_colon_for_condition()][print_newline()];
			when_condition_sequence_with_action = *when_condition_with_action;
			//when_condition_sequence_with_action_and_fold_force =
			//when_condition_sequence_with_action >> when_others_fold_force;
			//>> *when_condition_without_action;
			when_condition_without_action = when_condition >> *when_condition_with_action;
			when_condition_sequence_without_action = *when_condition_without_action;

			// When-condition sequences (with return statement)
			when_condition_with_return_statement = when_condition[print_questionmark_for_condition()]
				>> return_statement[print_colon_for_condition()][print_newline()];
			when_condition_sequence_with_return_statement = *when_condition_with_return_statement;
			//when_condition_sequence_with_action_and_fold_force =
			//when_condition_sequence_with_action >> when_others_fold_force;
			//>> *when_condition_without_action;
			when_condition_without_return_statement = when_condition >> *when_condition_with_return_statement;
			when_condition_sequence_without_return_statement = *when_condition_without_return_statement;

			// Return statement
			keyword_return = str_p("return") | "Return";
			return_statement = keyword_return >> expression >> keyword_force;
			
			// Conditions
			condition = expression;

			// Expressions
			expression = or_expression;
			bracket_expression = str_p("(")[print_bracket()] >> expression >> str_p(")")[print_bracket()];
			operand_expression = terminal_expression | bracket_expression;
			keyword_not = (str_p("not") | "Not" | "NOT")[print_operator()];
			unary_operator = (keyword_not | "-")[print_operator()];
			unary_expression = !unary_operator >> operand_expression;
			percentage_operator = str_p("%");
			multiplicative_operator = (str_p("*") | ("/"))[print_operator()] | percentage_operator[print_percentage_operator()]; 
			multiplicative_expression = unary_expression >> *(multiplicative_operator >> unary_expression);
			additive_operator = (str_p("+") | "-")[print_operator()];
			additive_expression = multiplicative_expression >> *(additive_operator >> multiplicative_expression);
			relational_operator = (str_p("<=") | ">=" | "<" | ">")[print_operator()];
			relational_expression = additive_expression >> *(relational_operator >> additive_expression);
			equality_expression = longest_d[hand_expression | board_expression | (relational_expression >> *(str_p("=")[print_operator()] >> relational_expression))];
			keyword_and = (str_p("and") | "And" | "AND")[print_operator()];
			and_expression = equality_expression >> *(keyword_and >> equality_expression);
			keyword_xor = (str_p("xor") | "Xor" | "XOr" | "XOR")[print_operator()];
			xor_expression = and_expression >> *(keyword_xor >> and_expression);
			keyword_or = (str_p("or") | "Or" | "OR")[print_operator()];
			or_expression = xor_expression >> *(keyword_or >> xor_expression);

			// Hand and board expressions
			card_constant = lexeme_d[ch_p("A") | "a" | "K" | "k" | "Q" | "q" | "J" | "j" |
				"T" | "t" | "9" | "8" | "7" | "6" | "5" | "4" | "3" | "2"];
			keyword_suited = str_p("suited") | "Suited" | "SUITED";
			keyword_board = str_p("board") | "Board" | "BOARD";
			keyword_hand = str_p("hand") | "Hand" | "HAND";
			card_expression = lexeme_d[+card_constant] >> !keyword_suited;
			board_expression = keyword_board >> str_p("=")[print_operator()] >> card_expression;
			hand_expression = keyword_hand >> str_p("=")[print_operator()] >> card_expression;
				 

			//Actions
			action = predefined_action | fixed_betsize_action | relative_betsize_action;
			predefined_action = keyword_predefined_action[print_predefined_action()] >> keyword_force;
			keyword_force = (str_p("force") | "Force" | "FORCE");
			keyword_beep = (str_p("beep") | "Beep" | "BEEP")[error_beep_not_supported()];
			keyword_call = str_p("call") | "Call" | "CALL";
			keyword_play = str_p("play") | "Play" | "PLAY";
			keyword_raise = str_p("raise") | "Raise" | "RAISE";
			keyword_raisemin = str_p("raisemin") | "Raisemin" | "RaiseMin" | "RAISEMIN";
			keyword_raisehalfpot = str_p("raisehalfpot") | "Raisehalfpot" | "RaiseHalfPot" | "RAISEHALFPOT";
			keyword_raisepot = str_p("raisepot") | "Raisepot" | "RaisePot" | "RAISEPOT";
			keyword_raisemax = str_p("raisemax") | "Raisemax" | "RaiseMax" | "RAISEMAX";
			keyword_fold = str_p("fold") | "Fold" | "FOLD";
			keyword_bet = str_p("bet") | "Bet" | "BET";
			keyword_betmin = str_p("betmin") | "Betmin" | "BetMin" | "BETMIN";
			keyword_bethalfpot = str_p("bethalfpot") | "Bethalfpot" | "BetHalfPot" | "BETHALFPOT";
			keyword_betpot = str_p("betpot") | "Betpot" | "BetPot" | "BETPOT";
			keyword_betmax = str_p("betmax") | "Betmax" | "BetMax" | "BETMAX";
			keyword_sitout = str_p("sitout") | "Sitout" | "SitOut" | "SITOUT";
			keyword_predefined_action = longest_d[keyword_beep | keyword_call	| keyword_play 
				| keyword_raisemin | keyword_raisehalfpot
				| keyword_raisepot | keyword_raisemax |	keyword_raise | keyword_fold
				| keyword_betmin | keyword_bethalfpot
				| keyword_betpot | keyword_betmax | keyword_bet | keyword_sitout];
			fixed_betsize_action = (keyword_bet | keyword_raise) >> number >> keyword_force;
			relative_betsize_action = (keyword_bet | keyword_raise) >> number[print_number()] 
				>> percentage_operator[print_percentage_operator()][print_relative_potsize_action()] 
				>> keyword_force;
			keyword_others = str_p("others") | "Others" | "OTHERS";

			// When others
			when_others_fold_force = keyword_when >> keyword_others >> keyword_fold >> keyword_force;
			when_others_when_others = keyword_when >> keyword_others >> when_others_fold_force;

			// Terminal expressions
			terminal_expression = number[print_number()] | boolean_constant | symbol;
			number = real_p;
			keyword_true = str_p("true") | "True" | "TRUE";
			keyword_false = (str_p("false") | "False" | "FALSE")[print_keyword()];
			boolean_constant = keyword_true | keyword_false;
			// "Symbol" is a lexeme - we have to be very careful here.
			// We have to use the lexeme_d directive to disable skipping of whitespace,
			// otherwise things like "When x Bet force" would treat "x Bet force"
			// as a single keyword and cause an error.
			// http://www.boost.org/doc/libs/1_40_0/libs/spirit/classic/doc/quickref.html
			symbol = lexeme_d[alpha_p >> *alnum_p][print_keyword()];
		} 

		const boost::spirit::rule<Scanner> &start() 
		{ 
			return openPPL_code; 
		} 
	}; 
}; 

int main(int argc, char *argv[]) 
{ 
	CString InputFile;
	/*p_symbol_table = new(CSymbolTable);*/
	if (argc <= 1)
	{
		// One argument is always present
		// It is the name of the executable in argv[0]
		InputFile = "input.txt";
		MessageBox(0, "No inputfile specified.\nAssuming \"input.txt\"", "Warning", 0);
	}
	else
	{
		// Using the first real argument
		InputFile = argv[1];
	}
	std::ifstream fs(InputFile); 
	std::ostringstream ss; 
	ss << fs.rdbuf(); 
	std::string data = ss.str(); 

	json_grammar g; 
	skip_grammar skip;
	// Case insensitive parsing: 
	// http://www.ibm.com/developerworks/aix/library/au-boost_parser/index.html
	boost::spirit::parse_info<> pi = boost::spirit::parse(data.c_str(), /*as_lower_d*/g, skip); 
	if (!pi.hit) 
	{
		CString rest_of_input = pi.stop;
		CString erroneous_input = rest_of_input.Left(100);
		if (erroneous_input.GetLength() > 0)
		{
		  // Examples about how to use the position operator:
		  // http://live.boost.org/doc/libs/1_34_0/libs/spirit/doc/position_iterator.html
		  // http://live.boost.org/doc/libs/1_34_0/libs/spirit/example/fundamental/position_iterator/position_iterator.cpp
		  file_position parse_error_position;
		  //parse_error_position = pi.first.get_position(); 
		  CString error_message;
		  error_message.Format("%s%d%s%d%s%s%s",
			  "Stopped at [line: ", 
			  12345, //parse_error_position.line,
			  ", column: ",
			  67890, //parse_error_position.column,	
			  "]\n================================\n\n", 
			  erroneous_input,
			  "\n================================\n\n"
			  "Common mistakes are:\n"
			  "  * malformed brackets\n"
			  "  * missing keyword \"custom\"\n"
			  "  * missing preflop / flop / turn or river section\n"
			  "  * missing \"when others fold force\" at the end of a block\n"
			  "  * misspelling of keywords\n");
		  MessageBox(0, error_message, "OpenPPL: Syntax Error", 0);
		}
	}
} 