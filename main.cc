/*
 * main.cc
 *
 *  Created on: Dec 28, 2008
 *      Author: root
 */

#include "simlab.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <map>
#include <string>

JNIEXPORT int welcome();
JNIEXPORT int store( simlab name );
JNIEXPORT int parse( simlab name, simlab func );
JNIEXPORT int cmd( simlab name );

int module;
extern int (*prnt)( const char*, ... );
extern int postprintf( const char*, ... );
extern int current;

extern simlab nulldata;
extern simlab data;
extern simlab prev;

//c_const<int>	iconst(0);
//c_const<float>	fconst(0.0f);

template <typename T> class PseudoBuffer {
public:
	virtual T inline operator[]( int ind ) { return (T)-1; };
	virtual ~PseudoBuffer() {};
	virtual int length() { return 0; };
};

template<typename T> class c_ind : PseudoBuffer<T> {
public:
	inline T operator[]( int ind ) {
		return ind;
	}

	int length() {
		return 10;
	}
};

template<typename T> class c_rnd : PseudoBuffer<T> {
public:
	inline T operator[]( int ind ) {
		return ind;
	}

	int length() {
		return 1;
	}
};

c_ind<int>		int_ind;
c_rnd<double>	dbl_rnd;

JNIEXPORT int jcrnt( void* buffer, long type, long length ) {
	data.buffer = (long)buffer;
	data.type = type;
	data.length = length;

	return 0;
}

JNIEXPORT int getlen() {
	return data.length;
}

JNIEXPORT int gettype() {
	return data.type;
}

JNIEXPORT int buff() {
	printf( "%ld\n", data.buffer );

	return 0;
}

#ifndef DL
std::map<std::string,long>	dlmap;
long dsym( int handle, const char* symbol ) {
	return dlmap[symbol];
}

void initdl() {
	dlmap["welcome"] = (long)welcome;
}
#endif

JNIEXPORT int init() {
	current = (long)&data;

#ifdef DL
	module = dopen( NULL );
	prnt = printf;
#else
	module = NULL;
	prnt = postprintf;
	initdl();
#endif

	//memset( &prev, sizeof(simlab), 0 );

	simlab name;
	float pi = acosf( -1.0f );
	float e = 2.1;//expf( 1.0f );

	nulldata.buffer = 0;
	nulldata.length = 0;
	nulldata.type = 0;

	data.buffer = *((int*)&pi);
	data.type = 34;
	data.length = 0;
	name.buffer = (long)"PI";
	name.type = 8;
	name.length = 2;
	store( name );

	data.buffer = *((int*)&e);
	name.buffer = (long)"e";
	name.length = 1;
	store( name );

	data.buffer = 1;
	data.type = 32;
	name.buffer = (long)"one";
	name.length = 3;
	store( name );

	data.buffer = 0;
	name.buffer = (long)"nil";
	name.length = 3;
	store( name );

	data.buffer = sizeof(double)*8+2;
	name.buffer = (long)"double";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = sizeof(float)*8+2;
	name.buffer = (long)"float";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = sizeof(long)*8;
	name.buffer = (long)"long";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = sizeof(int)*8;
	name.buffer = (long)"int";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = sizeof(short)*8;
	name.buffer = (long)"short";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = sizeof(char)*8;
	name.buffer = (long)"byte";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = 1;
	name.buffer = (long)"bit";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = (long)&int_ind;
	data.length = -1;
	data.type = 32;
	name.buffer = (long)"ind";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = (long)&dbl_rnd;
	data.length = -1;
	data.type = 66;
	name.buffer = (long)"rnd";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = 0;
	data.length = 0;
	data.type = 0;
	name.buffer = (long)"end";
	name.length = 3;
	store( name );
#ifdef GL
	data.buffer = GL_POINTS;
	data.type = 32;
	data.length = 0;
	name.buffer = (long)"GL_POINTS";
	name.type = 8;
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = GL_LINE_STRIP;
	data.type = 32;
	data.length = 0;
	name.buffer = (long)"GL_LINE_STRIP";
	name.type = 8;
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = GL_LINE_LOOP;
	data.type = 32;
	data.length = 0;
	name.buffer = (long)"GL_LINE_LOOP";
	name.type = 8;
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = GL_TRIANGLE_STRIP;
	data.type = 32;
	data.length = 0;
	name.buffer = (long)"GL_TRIANGLE_STRIP";
	name.type = 8;
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = GL_TRIANGLE_FAN;
	data.type = 32;
	data.length = 0;
	name.buffer = (long)"GL_TRIANGLE_FAN";
	name.type = 8;
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = GL_QUADS;
	name.buffer = (long)"GL_QUADS";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = GL_QUAD_STRIP;
	name.buffer = (long)"GL_QUAD_STRIP";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = GL_V2F;
	name.buffer = (long)"GL_V2F";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = GL_V3F;
	name.buffer = (long)"GL_V3F";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = GL_C3F_V3F;
	name.buffer = (long)"GL_C3F_V3F";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = GL_T2F_V3F;
	name.buffer = (long)"GL_T2F_V3F";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = GL_PROJECTION_MATRIX;
	name.buffer = (long)"GL_PROJECTION_MATRIX";
	name.length = strlen((char*)name.buffer);
	store( name );

	data.buffer = GL_MODELVIEW_MATRIX;
	name.buffer = (long)"GL_MODELVIEW_MATRIX";
	name.length = strlen((char*)name.buffer);
	store( name );
#endif

	return 0;
}

void start() {
	welcome();
	parse( nulldata, nulldata );
}

int mainfunc( int argc, char** argv ) {
#ifdef GTK
	gtk_init(&argc, &argv);
//#ifndef PTHREAD
	g_thread_init( NULL );
//#endif
#ifdef GL
	//gtk_gl_init( &argc, &argv );
#endif
#endif
	init();
	if( argc > 1 ) {
		simlab line;
		line.type = 8;
		for( int i = 1; i < argc; i++ ) {
			line.buffer = (long)argv[i];
			line.length = strlen(argv[i]);
			cmd( line );
		}
	} else {
		start();
	}

	return 0;
}
