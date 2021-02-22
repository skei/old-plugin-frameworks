#ifndef s3_debug_socket_included
#define s3_debug_socket_included
//----------------------------------------------------------------------

// nc -U -l -k /tmp/s3.socket

/*
  win32:
    #pragma comment(lib,"ws3_32.lib") //Winsock Library
    link with ws3_32.lib
    can't get win32 sockets to work :-/
    and how do we view/intercept these?
*/

#ifdef S3_DEBUG_SOCKET

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#ifdef S3_WIN32
  #include <stdio.h>
  #include <winsock2.h>
#endif

//----------

class S3_Debug_Socket {
  private:
    #ifdef S3_LINUX
    int s3_socket_out;
    #endif
    #ifdef S3_WIN32
    WSADATA wsa;
    SOCKET  s3_socket_out;
    #endif
  public:
    S3_Debug_Socket();
    ~S3_Debug_Socket();
    void print(const char* ABuffer);
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

#include "common/s3_string_stdlib.h"

#ifdef S3_LINUX

  #include <sys/socket.h>
  #include <sys/un.h>
  #include <unistd.h>
  #include <string.h>
  #include <stdio.h>  // snprintf, dprintf

  S3_Debug_Socket::S3_Debug_Socket() {
    s3_socket_out = socket(PF_UNIX,SOCK_STREAM,0);
    sockaddr_un address;
    memset(&address,0,sizeof(sockaddr_un));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path,108,"/tmp/s3.socket"); // max 108?
    connect(s3_socket_out,reinterpret_cast<sockaddr*>(&address),sizeof(sockaddr_un));
  }

  S3_Debug_Socket::~S3_Debug_Socket() {
    close(s3_socket_out);
  }

  void S3_Debug_Socket::print(const char* ABuffer) {
    dprintf(s3_socket_out,"%s",ABuffer);
  }

#endif // S3_LINUX

//----------

#ifdef S3_WIN32

  S3_Debug_Socket::S3_Debug_Socket() {
    WSAStartup(MAKEWORD(2,2),&wsa);
    s3_socket_out = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in address;
    memset(&address,0,sizeof(sockaddr_in));
    address.sin_family = AF_INET;
    //address.sin_addr.s_addr = inet_addr("74.125.235.20");
    address.sin_addr.s_addr = inet_addr("/tmp/s3.socket");
    //server.sin_port = htons( 80 );
    connect(s3_socket_out,(struct sockaddr *)&address , sizeof(address));
  }

  S3_Debug_Socket::~S3_Debug_Socket() {
    closesocket(s3_socket_out);
    WSACleanup();
  }

  void S3_Debug_Socket::print(const char* ABuffer) {
    //dprintf(s3_socket_out,"%s",ABuffer);
    send(s3_socket_out,ABuffer,S3_Strlen(ABuffer),0);
  }

#endif // S3_WIN32

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

static S3_Debug_Socket s3_debug_socket;

#define S3_Debug_Socket_Print  s3_debug_socket.print

#endif // S3_DEBUG_SOCKET

//----------------------------------------------------------------------
#endif
