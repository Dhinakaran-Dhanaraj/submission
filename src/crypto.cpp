#include "crypto.h"

#include "constants.h"
#include "error.h"

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#include <memory>
#include <functional>
#include <istream>
#include <ostream>
#include <iostream>

using std::string;
using std::unique_ptr;

#define CYPHER_TO_USE EVP_aes_256_cbc()
constexpr uint8_t iv[SHA_BLOCK_SZ] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

std::unique_ptr<EVP_CIPHER_CTX, void (*)(EVP_CIPHER_CTX *)> make_safe_cipher_ctx()
{
    std::unique_ptr<EVP_CIPHER_CTX, void (*)(EVP_CIPHER_CTX *)> output(
        EVP_CIPHER_CTX_new(),
        [](EVP_CIPHER_CTX *ptr)
        {
            if (ptr)
            {
                EVP_CIPHER_CTX_free(ptr);
            }
        });

    return output;
}

std::vector<uint8_t> encrypt_message(const std::string &key, const std::string &message)
{
    auto ctx = make_safe_cipher_ctx();
    std::vector<uint8_t> output;
    output.reserve(0);

    int error = 0;

    error = EVP_CIPHER_CTX_init(ctx.get());
    if (!error)
    {
        print_open_ssl_error();
        return output;
    }

    error = EVP_EncryptInit_ex(
        ctx.get(), CYPHER_TO_USE, NULL,
        reinterpret_cast<const unsigned char *>(key.c_str()), iv);
    if (!error)
    {
        print_open_ssl_error();
        return output;
    }

    output.resize(message.size() * SHA_BLOCK_SZ);
    int len = 0, output_len = 0;
    error = EVP_EncryptUpdate(
        ctx.get(),
        output.data(), &len,
        reinterpret_cast<const unsigned char *>(message.c_str()), message.size());
    if (!error)
    {
        print_open_ssl_error();
        return output;
    }

    output_len += len;
    
    EVP_EncryptFinal_ex(
        ctx.get(),
        output.data() + output_len, &len);
    if (!error)
    {
        print_open_ssl_error();
        return output;
    }

    output_len += len;
    output.resize(output_len);

    return output;
}

std::string decrypt_message(const std::string &key, const std::vector<uint8_t> &encrypted)
{
    auto ctx = make_safe_cipher_ctx();
    int error = 0;

    error = EVP_CIPHER_CTX_init(ctx.get());
    if (!error)
    {
        print_open_ssl_error();
        return "";
    }

    error = EVP_DecryptInit_ex(
        ctx.get(), CYPHER_TO_USE, NULL,
        reinterpret_cast<const unsigned char *>(key.c_str()), iv);
    if (!error)
    {
        print_open_ssl_error();
        return "";
    }

    int len = 0, output_len = 0;
    std::unique_ptr<unsigned char> output(new unsigned char[encrypted.size()]);
    error = EVP_DecryptUpdate(
        ctx.get(),
        output.get(), &len,
        encrypted.data(), encrypted.size());
    if (!error)
    {
        print_open_ssl_error();
        return "";
    }
    output_len += len;

    error = EVP_DecryptFinal_ex(ctx.get(), output.get() + output_len, &len);
    if (!error)
    {
        print_open_ssl_error();
        return "";
    }

    output_len += len;

    string str(reinterpret_cast<const char *>(output.get()), output_len);
    return str;
}