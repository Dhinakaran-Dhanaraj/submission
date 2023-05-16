#ifndef SUBMISSION_FILE_HANDLING_H
#define SUBMISSION_FILE_HANDLING_H

#include <string>
#include <vector>

std::vector<uint8_t> string_to_vector(const std::string& str);

bool write_to_file(const std::string& filepath, const std::vector<uint8_t>& contents);
bool read_from_file(const std::string& filepath, std::vector<uint8_t>& contents);

#define MESSAGE_ENCRYPTED_LOC ".encrypted"
#define HMAC_FILE_LOC ".hmac"

#endif