#ifndef SUBMISSION_HMAC_H
#define SUBMISSION_HMAC_H

#include <string>

std::string get_hmac(const std::string& key, const std::string& message);

#endif
