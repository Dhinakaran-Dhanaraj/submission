#include <iostream>
#include "crypto.h"
#include "hmac.h"
#include "file_handling.h"
#include "constants.h"
#include "error.h"

void exit_error()
{
	std::cout << -1 << std::endl;
	exit(1);
}

void print_help()
{
	std::cerr << "input must include -k <key>" << std::endl;
	exit(1);
}

int main(int argc, char **argv)
{
	std::string key{};
	if (argc != 3)
	{
		std::cerr << argc << std::endl;
		print_help();
	}

	for (int i = 1; i < argc; i += 2)
	{
		auto val = argv[i];
		switch (val[1])
		{
		case 'k':
			key = std::string(argv[i + 1]);
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
	std::vector<uint8_t> encrypted_on_disk;
	err = read_from_file(MESSAGE_ENCRYPTED_LOC, encrypted_on_disk);
	if (err) {
		exit_error();
	}

	std::vector<uint8_t> hmac_on_disk;
	err = read_from_file(HMAC_FILE_LOC, hmac_on_disk);
	if (err) {
		exit_error();
	}

	auto message = decrypt_message(key, encrypted_on_disk);
	auto hmac_of_decrypted = get_hmac(key, message);

	auto hmac_as_vec = string_to_vector(hmac_of_decrypted);
	if (hmac_as_vec == hmac_on_disk)
	{
		std::cout << message << std::endl;
	}
	else
	{
		exit_error();
	}
}
