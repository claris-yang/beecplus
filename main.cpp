#include <iostream>
#include "fcgio.h"

using namespace std;
int main() {
    streambuf *cin_streambuf = cin.rdbuf();
    streambuf *cout_streambuf = cout.rdbuf();
    streambuf *cerr_streambuf = cerr.rdbuf();

    FCGX_Request request;

    FCGX_Stream *in, *out, *err;
    FCGX_ParamArray envp;

    FCGX_Init();
    FCGX_InitRequest(&request, 0 ,0);
    //while (FCGX_Accept(&in, &out, &err, &envp) == 0) {
    while(FCGX_Accept_r(&request) >=0 ) {
        std::cout << "ok , i'm in" << std::endl;
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);
        FCGX_ParamArray envp = request.envp;

        std::string remote_addr = FCGX_GetParam("REMOTE_ADDR", request.envp ) == NULL ? "":
                                  FCGX_GetParam("REMOTE_ADDR", request.envp);

        std::cout << remote_addr << std::endl;
        std::string _queryString = FCGX_GetParam("QUERY_STRING", request.envp) == NULL ? "":
                                   FCGX_GetParam("QUERY_STRING", request.envp);

        std::cout << _queryString << std::endl;
        std::cout << FCGX_GetParam("REMOTE_ADDR", request.envp) << std::endl;

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);

        cout << "Content-type: text/html\r\n"
             << "\r\n"
             << "<html>\n"
             << "  <head>\n"
             << "    <title>Hello, World!</title>\n"
             << "  </head>\n"
             << "  <body>\n"
             << "    <h1>Hello, World!</h1>\n"
             << "  </body>\n"
             << "</html>\n";


        // Note: the fcgi_streambuf destructor will auto flush
    }

    // restore stdio streambufs
    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

    return 0;
}