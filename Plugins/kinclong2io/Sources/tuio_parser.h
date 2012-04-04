/**
The MIT License (MIT)

Copyright (c) ????????????????????????????????

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.


I don't know who coded these functions and I forgot where did I find it. It is far simpler than tuio.org implementation.

*/

// TUIOParser.cpp : Defines the exported functions for the DLL application.
//

#define TUIOPARSER_API


////////////////////////////////////////////////////////////////////////////
///
//
//
//
////////////////////////////////////////////////////////////////////////////
///


#define MAX_SETS 16


typedef unsigned char byte;

typedef int OSCInt;
typedef struct {
	unsigned int seconds;
	unsigned int fractions;
} OSCTimeTag;
typedef float OSCFloat;
typedef const char* OSCString;
typedef struct {
	int size;
	const byte* data;
} OSCBlob;


/*
 *
 */
struct OSCPARSER {
	const byte* data;
	int size;
	int offset;
};


struct FEEDBACK_SET {
	int mode;
	int sid;
	float x;
	float y;
};


struct FEEDBACK {
	OSCTimeTag timetag;
	int frame;
	int alive_count;
	int alive_sid[MAX_SETS];
	int reserved[3];
	int sets_count;
	struct FEEDBACK_SET sets[MAX_SETS];
};


////////////////////////////////////////////////////////////////////////////
///
//
//
//
////////////////////////////////////////////////////////////////////////////
///


void dump( unsigned char* data ) {

	int col;
	int row;

	for( row = 0; row < 4; row++ ) {
		for( col = 0; col < 16; col++ ) {
			printf( "%02x ", (int) data[col] );
		}
		printf( "\n" );
		data += 16;
	}
}


/*
 *
 */
void copyReversed( byte* target, const byte* source, int n ) {

	int i, j;

	for( i = n, j = n - 1; i > 0; i--, j-- ) {
		target[j] = source[n - i];
	}
}


////////////////////////////////////////////////////////////////////////////
///
//
//
//
////////////////////////////////////////////////////////////////////////////
///


/*
 *
 */
void initParser( struct OSCPARSER* parser, const void* data, int size ) {

	parser->data = (const byte*) data;
	parser->size = size;
	parser->offset = 0;
}


/*
 *
 */
int canRead( struct OSCPARSER* parser, int bytes ) {

	return ( parser->offset + bytes ) <= parser->size ? 1 : 0;
}


/*
 *
 */
int isBundle( struct OSCPARSER* parser ) {

	if( *( (const char*) parser->data ) == '#' ) {
		if( strcmp( (const char*) parser->data, "#bundle" ) == 0 ) {
			return 1;
		}
	}

	return 0;
}


/*
 *
 */
const byte* getCurrentPointer( struct OSCPARSER* parser ) {

	return parser->data + parser->offset;
}


/*
 *
 */
void skip( struct OSCPARSER* parser, int bytes ) {

	parser->offset += bytes;
}


/*
 *
 */
int readInteger( struct OSCPARSER* parser, OSCInt* pvalue ) {

	if( !canRead( parser, sizeof( *pvalue ) ) ) {
		return 0;
	}

	copyReversed(
		(byte*) pvalue,
		getCurrentPointer( parser ),
		sizeof( *pvalue )
	);

	skip( parser, sizeof( *pvalue ) );

	return 1;
}


/*
 *
 */
int readTimetag( struct OSCPARSER* parser, OSCTimeTag* pvalue ) {

	if( !canRead( parser, sizeof( *pvalue ) ) ) {
		return 0;
	}

	copyReversed(
		(byte*) pvalue,
		getCurrentPointer( parser ),
		sizeof( *pvalue )
	);

	skip( parser, sizeof( *pvalue ) );

	return 1;
}


/*
 *
 */
int readFloat( struct OSCPARSER* parser, OSCFloat* pvalue ) {

	if( !canRead( parser, sizeof( *pvalue ) ) ) {
		return 0;
	}

	copyReversed(
		(byte*) pvalue,
		getCurrentPointer( parser ),
		sizeof( *pvalue )
	);

	skip( parser, sizeof( *pvalue ) );

	return 1;
}


/*
 *
 */
int readString( struct OSCPARSER* parser, OSCString* pvalue, int* plength )
{

	if( !canRead( parser, 4 ) ) {
		return 0;
	}

	const byte* chunk = parser->data + parser->offset;
	OSCString value = (OSCString) chunk;
	int length = strlen( value );
	int size = ( ( length + 1 ) + 3 ) & ~3;

	skip( parser, size );
	*pvalue = value;
	if( plength ) {
		*plength = length;
	}

	return 1;
}


/*
 *
 */
int readBlob( struct OSCPARSER* parser, OSCBlob* blob ) {

	int actualSize;


	if( !canRead( parser, sizeof( blob->size ) ) ) {
		return 0;
	}

	copyReversed(
		(byte*) ( &blob->size ),
		getCurrentPointer( parser ),
		sizeof( blob->size )
	);

	actualSize = ( blob->size + 3 ) & ~3;

	if( !canRead( parser, sizeof( blob->size ) + actualSize ) ) {
		return 0;
	}

	blob->data = getCurrentPointer( parser ) + sizeof( blob->size );

	skip(
		parser,
		sizeof( blob->size ) + actualSize
	);

	return 1;
}


////////////////////////////////////////////////////////////////////////////
///
//
//
//
////////////////////////////////////////////////////////////////////////////
///


/*
 *
 */
void initFeedback( struct FEEDBACK* feedback ) {
	memset( feedback, 0, sizeof( *feedback ) );
}


////////////////////////////////////////////////////////////////////////////
///
//
//
//
////////////////////////////////////////////////////////////////////////////
///


/*
 *
 */
void readMessage( struct FEEDBACK* feedback, struct OSCPARSER* parser ) {

	OSCString path;
	OSCString tag;
	OSCString message;
	OSCInt intParam;
	OSCFloat floatParam;
	int nparams;
	int index;
	struct FEEDBACK_SET* set;


	readString( parser, &path, 0 );

	if( strcmp( path, "/tuio/2Dcur" ) != 0 ) {
		return;
	}

	readString( parser, &tag, &nparams );

	if( tag[0] != ',' || tag[1] != 's' ) {
		return;
	}

	nparams -= 2; // Remove the ',' and the 's'.

	readString( parser, &message, 0 );

	if( strcmp( message, "set" ) == 0 ) {

		set = feedback->sets + feedback->sets_count;
		set->mode = 0;
		readInteger( parser, &intParam );
		set->sid = intParam;
		readFloat( parser, &floatParam );
		set->x = floatParam;
		readFloat( parser, &floatParam );
		set->y = floatParam;
		feedback->sets_count++;

	} else if( strcmp( message, "alive" ) == 0 ) {

		for( index = 0; index < nparams; index++ ) {
			readInteger( parser, &intParam );
			feedback->alive_sid[feedback->alive_count] = intParam;
			feedback->alive_count++;
		}

	} else if( strcmp( message, "fseq" ) == 0 ) {

		readInteger( parser, &intParam );
		feedback->frame = intParam;
	}
}


/*
 *
 */
void readBundle( struct FEEDBACK* feedback, struct OSCPARSER* parser ) {

	OSCString string;
	OSCTimeTag timetag;
	OSCBlob blob;
	struct OSCPARSER subParser;


	readString( parser, &string, 0 );

	readTimetag( parser, &timetag );

	feedback->timetag = timetag;

	while( 1 ) {

		if( !readBlob( parser, &blob ) || !blob.size ) {
			break;
		}

		initParser( &subParser, blob.data, blob.size );

		if( isBundle( &subParser ) ) {
			readBundle( feedback, &subParser );
		} else {
			readMessage( feedback, &subParser );
		}
	}
}


////////////////////////////////////////////////////////////////////////////
///
//
//
//
////////////////////////////////////////////////////////////////////////////
///


/*
 *
 */
TUIOPARSER_API int _cdecl Test() {

	int error = 123456;

	return error;
}


////////////////////////////////////////////////////////////////////////////
///
//
//
//
////////////////////////////////////////////////////////////////////////////
///


/*
 *
 */
TUIOPARSER_API int _cdecl Parse(
	const void* inpbank,
	int inpbanksize,
	void* outpbank,
	int outpbanksize
) {

	int error = 0;
	struct OSCPARSER parser;
	struct FEEDBACK* feedback;


	initParser( &parser, inpbank, inpbanksize );
	feedback = (struct FEEDBACK*) outpbank;
	initFeedback( feedback );

	if( isBundle( &parser ) ) {
		readBundle( feedback, &parser );
	} else {
		readMessage( feedback, &parser );
	}

	return error;
}

