#include "XMPython.h"
#include "XMPythonFactory.h"

// force real dlunload on this lib
int __kde_do_unload = 1;

XMPythonFactory::XMPythonFactory( QObject* parent, const char* name )
  : KLibFactory( parent, name )
{
  //  s_instance = new KInstance("KXMPythonFactory");
}

XMPythonFactory::~XMPythonFactory() { 
  //  delete s_instance; 
}

QObject* XMPythonFactory::createObject(QObject* parent,
				       const char* name, 
				       const char* /* unused: classname */, 
				       const QStringList &/* unused: args */) {

  // cout << "Creating XMPerl" <<endl;
  return new XMPython(parent,name);
} 

#ifndef WIN32
K_EXPORT_COMPONENT_FACTORY(libxmpythoninterpreter,XMPythonFactory)
#else
extern "C" { __declspec(dllexport) void *init_libxmpythoninterpreter() { return new XMPythonFactory; } }
#endif

// KInstance *XMPythonFactory::s_instance = 0L;
