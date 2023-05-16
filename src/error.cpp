
#include <openssl/err.h>
#include <iostream>

void print_open_ssl_error() {
    unsigned long error = ERR_get_error();
    char errorString[256];
    ERR_error_string_n(error, errorString, sizeof(errorString));
    std::cerr << "OpenSSL Error: " << errorString << std::endl;
}