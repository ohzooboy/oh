// ListOfSyntaxElements_Inc.cpp
//
// This file gets included as pure CPP as part of the parser definition,
// otherwise the parser would become way too large to keep it understandable
// and AFAIK we can't distribute it over multiple modules.

openPPL_code, 
option_settings_to_be_ignored, 
single_option, 
option_name, 
option_value,
custom_sections,
missing_keyword_custom,
symbol_section,
code_sections,
preflop_section, 
flop_section, 
turn_section, 
river_section, 
missing_code_section,
symbol_definition,
when_section,
when_condition_sequence_with_action,
when_condition_sequence_without_action,
//when_condition_sequence_with_action_and_fold_force,
open_ended_when_condition_sequence,
open_ended_when_condition,
when_condition_with_action,
when_condition_without_action,
return_statement,
when_condition,
condition,
expression,
primary_expression,

terminal_expression,		
bracket_expression,
missing_closing_bracket_expression,
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
action_without_force,
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
when_others_when_others_fold_force,
suit_constant,
card_constant,
card_expression_with_specific_suits,
non_suited_card_expression,
suited_card_expression,
non_suited_board_expression,
suited_board_expression,
keyword_suited,
keyword_hand,
keyword_board,
keyword_new,
keyword_symbol,
keyword_return,
keyword_end,
card_expression,
board_expression_with_brackets,
hand_expression_with_brackets,
board_expression_without_brackets,
hand_expression_without_brackets,
erroneous_action_without_force,
invalid_character,
invalid_symbol
;