#include <iostream>

#include "crypto.h"
#include "hmac.h"
#include "file_handling.h"
#include "constants.h"
#include "error.h"

void print_help()
{
	std::cerr << "input must include -k <key> -m <message>" << std::endl;
	exit(1);
}

int main(int argc, char **argv)
{

	if (argc != 5)
	{
		std::cerr << argc << std::endl;
		print_help();
	}

	// process command line inputs
	std::string key{};
	std::string message{};

	for (int i = 1; i < argc; i += 2)
	{
		auto val = argv[i];
		switch (val[1])
		{
		case 'k':
			key = std::string(argv[i + 1]);
			break;
		case 'm':
			message = std::string(argv[i + 1]);
			break;
		default:
			print_help();
			break;
		}
	}

	if (key.size() != SHA_KEY_SZ)
	{
		print_key_error(key);
	}

	bool err = false;

	auto hmac = get_hmac(key, message);
	auto encrypted = encrypt_message(key, message);

	err = write_to_file(MESSAGE_ENCRYPTED_LOC, encrypted);

	auto hmac_as_vec = string_to_vector(hmac);
	err = write_to_file(HMAC_FILE_LOC, hmac_as_vec);
}
