#include <iostream>

#include "crypto.h"
#include "hmac.h"
#include "file_handling.h"
#include "constants.h"
#include "error.h"


int main(int argc, char** argv){
	
	// process command line inputs
    std::string key { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                           0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
                           0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
                           0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31
                         };
	std::string message =  "";

	if ( key.size() != SHA_KEY_SZ ) {
		print_key_error(key);
		exit(1);
	}

	bool err = false;

	auto hmac = get_hmac(key, message);
	auto encrypted = encrypt_message(key, message) ;

	err = write_to_file(MESSAGE_ENCRYPTED_LOC, encrypted);

	auto hmac_as_vec = string_to_vector(hmac);
	err = write_to_file(HMAC_FILE_LOC, hmac_as_vec);

}
