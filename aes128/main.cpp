#include "globals.h"
#include <string.h>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <sys/stat.h>

bool fileExists(const std::string& filename);
uint16_t read_permissions(char *);
void set_permissions(char *, uint16_t);

int main(int argc, char **argv)
{
	uint8_t keys[16] = {0x33, 0x33, 0x33};
						
	unsigned char test_key[16] = {0x35, 0x48, 0x22};
	uint8_t inp[16];
	char buffer[2];
	char buf[32];
	int64_t file_length = 0;
	int64_t remaining_length;
	uint8_t output[16];
	uint32_t j;
	char padded_zeroes = 0;
	char file_mode_char[2];
	int file_mode = 0;
	std::fstream fs, outfs;
	if(argc < 4)
	{
		printf("\nEncryption Usage: aes_encryption -e <16 Bytes Long Key(Optional)> <Input File Name> <Output File Name>\r\nIf the key is not provided as an argument, a random key will be generated and displayed.\r\n");
		printf("\nDecryption Usage: aes_encryption -d <16 Bytes Long Key> <Input File Name> <Output File Name>\r\n\n");
		return -1;
	}
	if(0 == memcmp(argv[1], "-e", 2))
	{
		if(4 > argc || 5 < argc)
		{
			printf("\nEncryption Usage: aes_encryption -e <16 Bytes Long Key(Optional)> <Input File Name> <Output File Name>\r\nIf the key is not provided as an argument, a random key will be generated and displayed.\r\n\n");
			return -1;
		}
		srand ((unsigned int) time (NULL));
		if(4 == argc)
		{
			for (uint8_t i = 0; i < 16; i++)
			{
			  //keys[i] = rand();
			}
		}
		else
		{
			for(uint8_t i = 0; i < 32; i=i+2)
			{
				//memcpy(buffer, (argv[2])+i, 2);
		//		printf("%x, %x\r\n", buffer[0], buffer[1]);
				//sscanf(buffer, "%x", &j);
		//		printf("%x, ", j);
				//keys[i/2] = j;
			}
		}
		printf("Using Key: ");
		print_cipher(keys);
		if(argc == 5)
		{
			if(!fileExists(argv[3]))
			{
				printf("Error: Input file not found, Ensure file exists...\r\n");
				return -1;
			}
			outfs.open(argv[4], std::fstream::out | std::fstream::app);
			fs.open(argv[3] , std::fstream::in);
			file_mode = read_permissions(argv[3]);
			fs.seekg(0, fs.end);
			file_length = fs.tellg();
			printf("Input File Length = %lu\r\n", file_length);
			remaining_length = file_length;
			fs.seekg(0, fs.beg);
		}
		else
		{
			if(!fileExists(argv[2]))
			{
				printf("Error: Input file not found, Ensure file exists...\r\n");
				return -1;
			}
			outfs.open(argv[3], std::fstream::out | std::fstream::app);
			fs.open(argv[2] , std::fstream::in);
			file_mode = read_permissions(argv[2]);
			fs.seekg(0,fs.end);
			file_length = fs.tellg();
			printf("Input File Length = %lu\r\n", file_length);
			remaining_length = file_length;
			fs.seekg(0, fs.beg);
		}
		if(file_length == 0)
		{
			printf("Error: Empty File, Nothing to Encrypt...\r\n");
			return -1;
		}
		outfs << std::setbase(16);
		padded_zeroes = 0;
		while(remaining_length > 1)
		{
			if(remaining_length < 16)
			{
				fs.read(buf, remaining_length);
				for(uint8_t i = 0; i < remaining_length; i++)
				{
					memcpy(buffer, buf + i, 1);
					inp[i] = buffer[0];
				}
				for(uint8_t i = remaining_length; i < 16; i++)
				{
					inp[i] = 0x00;
				}
				padded_zeroes = 16 - remaining_length;
				remaining_length -= remaining_length;
				aes_encrypt(inp, (uint8_t*)test_key, output);
	//			print_cipher(output);
				for(uint8_t i = 0; i < 16; i++)
				{
					outfs << output[i];
				}
				break;
			}
			fs.read(buf, 16);
			for(uint8_t i = 0; i < 16; i++)
			{
				memcpy(buffer, buf + i, 1);
				inp[i] = buffer[0];
			}
			remaining_length -= 16;
			aes_encrypt(inp, (uint8_t*)test_key, output);
	//		print_cipher(output);
			for(uint8_t i = 0; i < 16; i++)
			{
				outfs << output[i];
			}
		}
		fs.close();
		outfs << padded_zeroes << (char)((file_mode & 0xFF00) >> 8) << (char)(file_mode & 0xFF);
		outfs.close();
		if(argc == 4)
		{
			printf("Encrypted file \"%s\" generated successfully...\r\n", argv[3]);
		}
		else
		{
			printf("Encrypted file \"%s\" generated successfully...\r\n", argv[4]);
		}
	}
	else if(0 == memcmp(argv[1], "-d", 2))
	{
		if(5 != argc)
		{
			printf("\nDecryption Usage: aes_encryption -d <16 Bytes Long Key> <Input File Name> <Output File Name>\r\n\n");
			return -1;
		}
		for(uint8_t i = 0; i < 32; i=i+2)
		{
			//memcpy(buffer, (argv[2])+i, 2);
	//		printf("%x, %x\r\n", buffer[0], buffer[1]);
			//sscanf(buffer, "%x", &j);
	//		printf("%x, ", j);
			//keys[i/2] = j;
		}
		if(!fileExists(argv[3]))
		{
			printf("Error: Input file not found, Ensure file exists...\r\n");
			return -1;
		}
		outfs.open(argv[4], std::fstream::out | std::fstream::app);
		fs.open(argv[3] , std::fstream::in);
		fs.seekg(-3, fs.end);
		file_length = fs.tellg();
		fs.read(&padded_zeroes, 1);
		fs.read(file_mode_char, 2);
		printf("Input File Length = %lu , p_zero : %x , fi_cahr: %x , two: %x \r\n", file_length, padded_zeroes, file_mode_char[0], file_mode_char[1]);
		remaining_length = file_length;
		fs.seekg(0, fs.beg);
		if(file_length == 0)
		{
			printf("Error: Empty File, Nothing to Encrypt...\r\n");
			return -1;
		}
		printf("remaining: %lu\n", remaining_length);
		while(remaining_length > 1)
		{
			if(remaining_length < 16)
			{
				fs.read(buf, remaining_length);
				for(uint8_t i = 0; i < remaining_length; i++)
				{
					memcpy(buffer, buf+i, 1);
					printf("ffdd %x, %x\r\n", buffer[0], buffer[1]);
					printf("ffdd %x, \n", j);
					inp[i] = buffer[0];
				}
				for(uint8_t i = remaining_length; i < 16; i++)
				{
					inp[i] = 0x00;
				}
				remaining_length -= remaining_length;
				aes_decrypt(inp, (uint8_t*)test_key, output);
				print_cipher(output);
				for(uint8_t i = 0; i < 16; i++)
				{
					printf("ffddu %x, \n", output[i]);
					if(0x00 != output[i])
						outfs << output[i];
				}
				break;
			}
			fs.read(buf, 16);
			for(uint8_t i = 0; i < 16; i++)
			{
				memcpy(buffer, buf+i, 1);
				//printf("fff %x, %x\r\n", buffer[0], buffer[1]);
				//printf("fff %x, ", j);
				inp[i] = buffer[0];
			}
			remaining_length -= 16;
			aes_decrypt(inp, (uint8_t*)test_key, output);
	 		//print_cipher(output);
			for(uint8_t i = 0; i < 16; i++)
			{
				if(0 != remaining_length)
				{
					outfs << (output[i]);
				}
				else
				{
					if(i < (16 - padded_zeroes))
					printf("ffddu less %x, \n", output[i]);
					outfs << (output[i]);
				}
			}
		}
		fs.close();
//		outfs << "\n";
		outfs.close();
		set_permissions(argv[4], ((((uint16_t)(file_mode_char[0]) & 0x00FF) << 8) | (file_mode_char[1] & 0xFF)));
		printf("Decrypted file \"%s\" generated successfully...\r\n", argv[4]);
	}
	else
	{
		printf("\nEncryption Usage: aes_encryption -e <16 Bytes Long Key(Optional)> <Input File Name> <Output File Name>\r\nIf the key is not provided as an argument, a random key will be generated and displayed.\r\n");
		printf("\nDecryption Usage: aes_encryption -d <16 Bytes Long Key> <Input File Name> <Output File Name>\r\n\n");
		return -1;
	}
}


bool fileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

uint16_t read_permissions(char *file)
{
	struct stat buf;
	stat(file, &buf);
//	printf("Heres File Mode: %d\n", buf.st_mode);
	return buf.st_mode;
}

void set_permissions(char *file, uint16_t perm)
{
//	uint16_t perm1 = 0;
	chmod(file, perm);
}

