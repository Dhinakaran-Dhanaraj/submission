#ifndef SUBMISSION_CRYPTO_H
#define SUBMISSION_CRYPTO_H

#include <string>
#include <vector>

std::vector<uint8_t> encrypt_message(const std::string& key, const std::string& message);
std::string decrypt_message(const std::string& key, const std::vector<uint8_t>& encrypted);

#endif