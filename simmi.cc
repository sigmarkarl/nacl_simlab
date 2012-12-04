/// @file <PROJECT_NAME>.cc
/// This example demonstrates loading, running and scripting a very simple NaCl
/// module.  To load the NaCl module, the browser first looks for the
/// CreateModule() factory method (at the end of this file).  It calls
/// CreateModule() once to load the module code from your .nexe.  After the
/// .nexe code is loaded, CreateModule() is not called again.
///
/// Once the .nexe code is loaded, the browser than calls the CreateInstance()
/// method on the object returned by CreateModule().  It calls CreateInstance()
/// each time it encounters an <embed> tag that references your NaCl module.
///
/// The browser can talk to your NaCl module via the postMessage() Javascript
/// function.  When you call postMessage() on your NaCl module from the browser,
/// this becomes a call to the HandleMessage() method of your pp::Instance
/// subclass.  You can send messages back to the browser by calling the
/// PostMessage() method on your pp::Instance.  Note that these two methods
/// (postMessage() in Javascript and PostMessage() in C++) are asynchronous.
/// This means they return immediately - there is no waiting for the message
/// to be handled.  This has implications in your program design, particularly
/// when mutating property values that are exposed to both the browser and the
/// NaCl module.

#include <cstdio>
#include <string>
#include <map>
#include <cstring>
#include <cstdarg>
#include <cassert>
#include <cmath>
#include <limits>
#include <sstream>
#include "ppapi/cpp/audio.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"

extern char* erm( int argc, const char* argv[] );
extern char* dna( int argc, const char* argv[] );
extern char* ml( int argc, const char* argv[] );

/// The Instance class.  One of these exists for each instance of your NaCl
/// module on the web page.  The browser will ask the Module object to create
/// a new Instance for each occurence of the <embed> tag that has these
/// attributes:
///     type="application/x-nacl"
///     src="<PROJECT_NAME>.nmf"
/// To communicate with the browser, you must override HandleMessage() for
/// receiving messages from the borwser, and use PostMessage() to send messages
/// back to the browser.  Note that this interface is entirely asynchronous.
class simmiInstance : public pp::Instance {
 public:
  /// The constructor creates the plugin-side instance.
  /// @param[in] instance the handle to the browser-side plugin instance.
  explicit simmiInstance(PP_Instance instance) : pp::Instance(instance)
  {}
  virtual ~simmiInstance() {}

  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);
  /// Handler for messages coming in from the browser via postMessage().  The
  /// @a var_message can contain anything: a JSON string; a string that encodes
  /// method names and arguments; etc.  For example, you could use
  /// JSON.stringify in the browser to create a message that contains a method
  /// name and some parameters, something like this:
  ///   var json_message = JSON.stringify({ "myMethod" : "3.14159" });
  ///   nacl_module.postMessage(json_message);
  /// On receipt of this message in @a var_message, you could parse the JSON to
  /// retrieve the method name, match it to a function call, and then call it
  /// with the parameter.
  /// @param[in] var_message The message posted by the browser.
  virtual void HandleMessage(const pp::Var& var_message) {
	    if (!var_message.is_string()) {
	      return;
	    }

	    std::string message = var_message.AsString();
	    const char* c = message.c_str();
	    char* ret;
	    if( c[0] == 'f' ) {
		const char* cc[] = { "FastTree", "-nt", c+1 };
		ret = erm( 3, cc );
	    } else if( c[0] == 'p' ) {
		const char* cc[] = { "dnapars", c+1 };
		ret = dna( 2, cc );
	    } else {
		const char* cc[] = { "dnaml", c+1 };
		ret = ml( 2, cc );
	    }

	    pp::Var return_var( ret );
	    PostMessage(return_var);
	    
	/*long (*func)() = (long (*)())dlmap[ message ];
	    if( func != NULL ) func();
	    else postprintf( "moos" );

	    pp::Var return_var;
	    if (message == kFortyTwoMethodId) {
	      // Note that no arguments are passed in to FortyTwo.
	      return_var = MarshallFortyTwo();
	    } else if (message.find(kReverseTextMethodId) == 0) {
	      // The argument to reverseText is everything after the first ':'.
	      size_t sep_pos = message.find_first_of(kMessageArgumentSeparator);
	      if (sep_pos != std::string::npos) {
	        std::string string_arg = message.substr(sep_pos + 1);
	        return_var = MarshallReverseText(string_arg);
	      }
	    }*/
	    // Post the return result back to the browser.  Note that HandleMessage() is
	    // always called on the main thread, so it's OK to post the return message
	    // directly from here.  The return post is asynhronous: PostMessage returns
	    // immediately.
	    //PostMessage(return_var);
	    //PostMessage(var_message);
  }
};

char cc[5000];
simmiInstance* gsimst;
extern int postprint( const char* c, ... ) {
	//std::memcpy( &theerm, c+1, sizeof(erm) );
	va_list	ap;
	va_start( ap, c );
	char* av = va_arg( ap, char* );
	sprintf( cc, c, av );
	va_end( ap );
	pp::Var return_var(cc);
	gsimst->PostMessage( return_var );

	return 0;
}

bool simmiInstance::Init(uint32_t argc,
                             const char* argn[],
                             const char* argv[]) {
  return true;
}

/// The Module class.  The browser calls the CreateInstance() method to create
/// an instance of your NaCl module on the web page.  The browser creates a new
/// instance for each <embed> tag with type="application/x-nacl".
class simmiModule : public pp::Module {
 public:
  simmiModule() : pp::Module() {}
  virtual ~simmiModule() {}

  /// Create and return a simmiInstance object.
  /// @param[in] instance The browser-side instance.
  /// @return the plugin-side instance.
  virtual pp::Instance* CreateInstance(PP_Instance instance) {
	  simmiInstance* simst = new simmiInstance(instance);
          gsimst = simst;
    return simst;
  }
};

namespace pp {
/// Factory function called by the browser when the module is first loaded.
/// The browser keeps a singleton of this module.  It calls the
/// CreateInstance() method on the object you return to make instances.  There
/// is one instance per <embed> tag on the page.  This is the main binding
/// point for your NaCl module with the browser.
Module* CreateModule() {
  return new simmiModule();
}
}  // namespace pp
