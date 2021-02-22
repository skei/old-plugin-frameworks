#ifndef s3_debug_xlib_included
#define s3_debug_xlib_included
//----------------------------------------------------------------------

/*
  https://tronche.com/gui/x/xlib/event-handling/protocol-errors/synchronization.html

  When debugging X applications, it often is very convenient to require Xlib to
  behave synchronously so that errors are reported as they occur. The following
  function lets you disable or enable synchronous behavior. Note that graphics
  may occur 30 or more times more slowly when synchronization is enabled. On
  POSIX-conformant systems, there is also a global variable _Xdebug that, if
  set to nonzero before starting a program under a debugger, will force
  synchronous library behavior.

  After completing their work, all Xlib functions that generate protocol
  requests call what is known as an after function. XSetAfterFunction() sets
  which function is to be called.

  To enable or disable synchronization, use XSynchronize().
*/

//----------------------------------------------------------------------

/*
  https://tronche.com/gui/x/xlib/event-handling/protocol-errors/default-handlers.html

  There are two default error handlers in Xlib: one to handle typically fatal
  conditions (for example, the connection to a display server dying because a
  machine crashed) and one to handle protocol errors from the X server. These
  error handlers can be changed to user-supplied routines if you prefer your
  own error handling and can be changed as often as you like. If either
  function is passed a NULL pointer, it will reinvoke the default handler. The
  action of the default handlers is to print an explanatory message and exit.

  To set the error handler, use XSetErrorHandler().
*/

//----------------------------------------------------------------------

/*
  see also:
  http://www.motifdeveloper.com/tips/tip29.html
  https://tronche.com/gui/x/xlib/event-handling/protocol-errors/XGetErrorDatabaseText.html
  https://tronche.com/gui/x/xlib/event-handling/protocol-errors/XGetErrorText.html
*/

//----------------------------------------------------------------------

#ifdef S3_DEBUG_XLIB

#include <X11/Xlib.h>

int s3_debug_xlib_error_handler(Display *display, XErrorEvent *error);
int s3_debug_xlib_io_error_handler(Display *display);

//----------------------------------------------------------------------

class S3_Debug_Xlib {

  private:

    int (*old_error_handler)(Display*, XErrorEvent*);
    int (*old_io_error_handler)(Display*);
    //int (*old_after_function)(void);

  public:

    /*
      Install our error handler to override Xlib's termination behavior
    */

    S3_Debug_Xlib() {
      old_error_handler = XSetErrorHandler(s3_debug_xlib_error_handler);
      old_io_error_handler = XSetIOErrorHandler(s3_debug_xlib_io_error_handler);
      //old_after_function = XSynchronize(MDisplay,True);
      //old_after_function = XSetAfterFunction(MDisplay,s3_debug_xlib_error_handler);
    }

    //----------

    /*
      Restore original X handler
      Assumption: the X queue has been temporarily flushed/synchronized
      at the end of the application-critical code, otherwise a possible
      asynchronously arriving error might arrive further down the stream
      after the original handlers are restored.
        XFlush(display);
        XSync(display, False);

      if XSetErrorHandler argunemt = NULL, set default handler
    */

    ~S3_Debug_Xlib() {
      XSetErrorHandler(old_error_handler);
      XSetIOErrorHandler(old_io_error_handler);
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    /*
    Success           0	everything's okay
    BadRequest	      1	bad request code
    BadValue	        2	int parameter out of range
    BadWindow	        3	parameter not a Window
    BadPixmap	        4	parameter not a Pixmap
    BadAtom	          5	parameter not an Atom
    BadCursor	        6	parameter not a Cursor
    BadFont	          7	parameter not a Font
    BadMatch	        8	parameter mismatch
    BadDrawable	      9	parameter not a Pixmap or Window
    BadAccess	        10	depending on context:
      key/button already grabbed
      attempt to free an illegal cmap entry
      attempt to store into a read-only color map entry
      attempt to modify the access control list from other than the local host
    BadAlloc	        11	insufficient resources
    BadColor	        12	no such colormap
    BadGC	            13	parameter not a GC
    BadIDChoice	      14	choice not in range or already used
    BadName	          15	font or color name doesn't exist
    BadLength	        16	request length incorrect
    BadImplementation	17	server is defective
    */

    void printErrorText(unsigned char error_code) {
      switch(error_code) {
        case BadAccess:
          S3_DTrace("* A client attempts to grab a key/button combination already grabbed by another\n");
          S3_DTrace("  client.\n");
          S3_DTrace("* A client attempts to free a colormap entry that it had not already allocated\n");
          S3_DTrace("  or to free an entry in a colormap that was created with all entries writable.\n");
          S3_DTrace("* A client attempts to store into a read-only or unallocated colormap entry.\n");
          S3_DTrace("* A client attempts to modify the access control list from other than the local\n");
          S3_DTrace("  (or otherwise authorized) host.\n");
          break;
        case BadAlloc:
          S3_DTrace("* A client attempts to select an event type that another client has already\n");
          S3_DTrace("  selected.\n");
          S3_DTrace("* The server fails to allocate the requested resource. Note that the explicit\n");
          S3_DTrace("  listing of BadAlloc errors in requests only covers allocation errors at a\n");
          S3_DTrace("  very coarse level and is not intended to (nor can it in practice hope to)\n");
          S3_DTrace("  cover all cases of a server running out of allocation space in the middle of\n");
          S3_DTrace("  service. The semantics when a server runs out of allocation space are left\n");
          S3_DTrace("  unspecified, but a server may generate a BadAlloc error on any request for\n");
          S3_DTrace("  this reason, and clients should be prepared to receive such errors and handle\n");
          S3_DTrace("  or discard them.\n");
          break;
        case BadAtom:
          S3_DTrace("* A value for an atom argument does not name a defined atom.\n");
          break;
        case BadColor:
          S3_DTrace("* A value for a colormap argument does not name a defined colormap.\n");
          break;
        case BadCursor:
          S3_DTrace("* A value for a cursor argument does not name a defined cursor.\n");
          break;
        case BadDrawable:
          S3_DTrace("* A value for a drawable argument does not name a defined window or pixmap.\n");
          break;
        case BadFont:
          S3_DTrace("* A value for a font argument does not name a defined font (or, in some cases\n");
          S3_DTrace("  GContext).\n");
          break;
        case BadGC:
          S3_DTrace("* A value for a GContext argument does not name a defined GContext\n");
          break;
        case BadIDChoice:
          S3_DTrace("* The value chosen for a resource identifier either is not included in the\n");
          S3_DTrace("  range assigned to the client or is already in use. Under normal\n");
          S3_DTrace("  circumstances, this cannot occur and should be considered a server or Xlib\n");
          S3_DTrace("  error\n");
          break;
        case BadImplementation:
          S3_DTrace("* The server does not implement some aspect of the request. A server that\n");
          S3_DTrace("  generates this error for a core request is deficient. As such, this error is\n");
          S3_DTrace("  not listed for any of the requests, but clients should be prepared to receive\n");
          S3_DTrace("  such errors and handle or discard them.\n");
          break;
        case BadLength:
          S3_DTrace("* The length of a request is shorter or longer than that required to contain\n");
          S3_DTrace("  the arguments. This is an internal Xlib or server error.\n");
          S3_DTrace("* The length of a request exceeds the maximum length accepted by the server.\n");
          break;
        case BadMatch:
          S3_DTrace("* In a graphics request, the root and depth of the graphics context does not\n");
          S3_DTrace("  match that of the drawable.\n");
          S3_DTrace("* An InputOnly window is used as a drawable.\n");
          S3_DTrace("* Some argument or pair of arguments has the correct type and range, but it\n");
          S3_DTrace("  fails to match in some other way required by the request.\n");
          S3_DTrace("* An InputOnly window lacks this attribute.\n");
          break;
        case BadName:
          S3_DTrace("* A font or color of the specified name does not exist.\n");
          break;
        case BadPixmap:
          S3_DTrace("* A value for a pixmap argument does not name a defined pixmap.\n");
          break;
        case BadRequest:
          S3_DTrace("* The major or minor opcode does not specify a valid request. This usually is\n");
          S3_DTrace("  an Xlib or server error.\n");
          break;
        case BadValue:
          S3_DTrace("* Some numeric value falls outside of the range of values accepted by the\n");
          S3_DTrace("  request. Unless a specific range is specified for an argument, the full range\n");
          S3_DTrace("  defined by the argument's type is accepted. Any argument defined as a set of\n");
          S3_DTrace("  alternatives typically can generate this error (due to the encoding).\n");
          break;
        case BadWindow:
          S3_DTrace("* A value for a window argument does not name a defined window\n");
          break;
        default:
          S3_DTrace("* unknown error.\n");
          break;
      }
    }

    /*

      https://tronche.com/gui/x/xlib/event-handling/protocol-errors/default-handlers.html

      typedef struct {
        int type;
        Display *display;           // Display the event was read from
        unsigned long serial;       // serial number of failed request
        unsigned char error_code;   // error code of failed request
        unsigned char request_code; // Major op-code of failed request
        unsigned char minor_code;   // Minor op-code of failed request
        XID resourceid;             // resource id
      } XErrorEvent;

      The serial member is the number of requests, starting from one, sent over
      the network connection since it was opened. It is the number that was the
      value of NextRequest immediately before the failing call was made. The
      request_code member is a protocol request of the procedure that failed,
      as defined in <X11/Xproto.h>.

    */

    int errorHandler(Display *display, XErrorEvent *error) {
      char error_text[1024];
      error_text[0] = '\0';
      XGetErrorText(display,error->error_code,error_text,1024);
      S3_DTrace("\n");
      S3_DTrace("Xlib error %i: %s\n",error->error_code,error_text);
      S3_DTrace("  type:         %i\n",error->type);
      S3_DTrace("  display:      0x%X\n",error->display);
      S3_DTrace("  serial:       %i\n",error->serial);
      S3_DTrace("  request_code: %i\n",error->request_code);
      S3_DTrace("  minor_code:   %i\n",error->minor_code);
      S3_DTrace("  resourceid:   %i\n",error->resourceid);
      S3_DTrace("\n");
      printErrorText(error->error_code);
      S3_DTrace("\n");
      #ifdef S3_DEBUG_CALLSTACK
      S3_DumpCallStackSkip(8);
      #endif
      exit(1);
      return 0;
    }

    //----------

    int ioErrorHandler(Display *display) {
      S3_Trace("Xlib IO error\n");
      return 0;
    }

};

//----------------------------------------------------------------------

static S3_Debug_Xlib s3_debug_xlib;

//----------------------------------------------------------------------

//static
int s3_debug_xlib_error_handler(Display *display, XErrorEvent *error) {
  return s3_debug_xlib.errorHandler(display,error);
}

//static
int s3_debug_xlib_io_error_handler(Display *display) {
  return s3_debug_xlib.ioErrorHandler(display);
}


#endif // S3_DEBUG_XLIB

//----------------------------------------------------------------------
#endif

/*
  http://stackoverflow.com/questions/6096605/is-handler-set-via-xseterrorhandler-global-or-threadlocal
  XSetErrorHandler is global in two senses.
  One, it's not per-thread.
  Two, it's not per-display: if you have multiple Display connections within
       the same process, the same handler will apply to all of them.
*/
