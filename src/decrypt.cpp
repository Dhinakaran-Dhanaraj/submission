#include <iostream>
#include "crypto.h"
#include "hmac.h"
#include "file_handling.h"

void exit_error() {
	std::cout << -1 << std::endl;
	exit(1);
}

int main(int argc, char** argv){
	
	// process command line inputs
    std::string key { 
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
        0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
        0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31
    };
	
	bool err = false;
	std::vector<uint8_t> encrypted_on_disk;
	err = read_from_file(MESSAGE_ENCRYPTED_LOC, encrypted_on_disk);

	std::vector<uint8_t> hmac_on_disk;
	err = read_from_file(HMAC_FILE_LOC,hmac_on_disk);

	auto message = decrypt_message(key,encrypted_on_disk);	
	auto hmac_of_decrypted = get_hmac(key, message);

	auto hmac_as_vec = string_to_vector(hmac_of_decrypted);
	if (hmac_as_vec == hmac_on_disk) {
		std::cout << message << std::endl;
	} else {
		exit_error();
	}
}
