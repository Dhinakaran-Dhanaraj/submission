#include "hmac.h"
#include "constants.h"
#include "error.h"

#include <openssl/evp.h>

#include <memory>
#include <istream>
#include <functional>
#include <iostream>
#include <sstream>
#include <iomanip>

std::unique_ptr<EVP_MD_CTX, void (*)(EVP_MD_CTX *)> make_safe_digest_ctx()
{
    std::unique_ptr<EVP_MD_CTX, void (*)(EVP_MD_CTX *)> ctx(
        EVP_MD_CTX_new(),
        [](EVP_MD_CTX *ptr)
        {
            if (ptr)
            {
                EVP_MD_CTX_free(ptr);
            }
        });

    return std::move(ctx);
}

std::unique_ptr<EVP_PKEY, void (*)(EVP_PKEY *)> make_safe_evp_pkey(EVP_PKEY *key)
{
    std::unique_ptr<EVP_PKEY, void (*)(EVP_PKEY *)> output(
        key,
        [](EVP_PKEY *ptr)
        {
            if (ptr)
            {
                EVP_PKEY_free(ptr);
            }
        });

    return std::move(output);
}

std::string get_hmac(const std::string &key, const std::string &message)
{
    int error{0};
    const EVP_MD *md_type = EVP_sha256();

    auto ctx = make_safe_digest_ctx();
    if (!ctx)
    {
        print_open_ssl_error();
        return "";
    }

    auto pkey = make_safe_evp_pkey(
        EVP_PKEY_new_mac_key(
            EVP_PKEY_HMAC, NULL,
            reinterpret_cast<const unsigned char *>(key.c_str()), key.size()));

    if (!pkey)
    {
        print_open_ssl_error();
        return "";
    }

    error = EVP_DigestSignInit(ctx.get(), NULL, md_type, NULL, pkey.get());
    if (!error)
    {
        print_open_ssl_error();
        return "";
    }

    error = EVP_DigestSignUpdate(ctx.get(), reinterpret_cast<const void *>(message.c_str()), message.size());
    if (!error)
    {
        print_open_ssl_error();
        return "";
    }

    unsigned char digest[SHA_256_BUFFER_LEN];
    size_t digestLength = SHA_256_BUFFER_LEN;

    error = EVP_DigestSignFinal(ctx.get(), digest, &digestLength);
    if (!error)
    {
        print_open_ssl_error();
        return "";
    }

    std::string(reinterpret_cast<const char *>(digest), digestLength);

    std::stringstream ss;
    for (unsigned int i = 0; i < digestLength; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }
    return ss.str();
}
