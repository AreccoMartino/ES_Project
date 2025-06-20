#ifndef PARSER_LIB_H
#define	PARSER_LIB_H

#define STATE_DOLLAR  (1)   // we discard everything until a dollar is found
#define STATE_TYPE    (2)   // we are reading the type of msg until a comma is found
#define STATE_PAYLOAD (3)   // we read the payload until an asterix is found
#define NEW_MESSAGE (1)     // new message received and parsed completely
#define NO_MESSAGE (0)      // no new messages

typedef struct { 
	int state;
	char msg_type[6]; // type is 5 chars + string terminator
	char msg_payload[100];  // assume payload cannot be longer than 100 chars
	int index_type;
	int index_payload;
} parser_state;

/*
Requires a pointer to a parser state, and the byte to process.
returns NEW_MESSAGE if a message has been successfully parsed.
The result can be found in msg_type and msg_payload.
Parsing another byte will override the contents of those arrays.
*/
int parse_byte(parser_state* ps, char byte);

/*
Takes a string as input, and converts it to an integer. Stops parsing when reaching
the end of string or a ","
the result is undefined if a wrong string is passed
*/
int extract_integer(const char* str);

/*
The function takes a string, and an index within the string, and returns the index where the next data can be found
Example: with the string "10,20,30", and i=0 it will return 3. With the same string and i=3, it will return 6.
*/  
int next_value(const char* msg, int i);

#endif	/* PARSER_LIB_H */
