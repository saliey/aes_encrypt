    #include <stdio.h>  
      
    #include <iostream>  
    #include <fstream>  
    #include <sstream>  
	
#include <getopt.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
      
    #include <cryptopp/aes.h>  
    #include <cryptopp/filters.h>  
    #include <cryptopp/modes.h>  
	
	using namespace std;
int retDataSize;

const char* short_options = "hdm:d:e:o:";
const struct option long_options[] = {
        {"help", 0, NULL, 'h'},
        {"decode", 1, NULL, 'd'},
        {"encode", 1, NULL, 'e'},
		{"output", 1, NULL, 'o'},
        {nullptr, 0, nullptr, 0}
};
void useage();
    
char g_key[17] = "asdfwetyhjuytrfd"; 
char g_iv[17] = {0};
      
    using namespace std;  
      
    byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE];  
      
    void initKV()  
    {  
        memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );  
        memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );  
      
        // 或者也可以  
        /* 
        char tmpK[] = "1234567890123456"; 
        char tmpIV[] = "1234567890123456"; 
        for (int j = 0; j < CryptoPP::AES::DEFAULT_KEYLENGTH; ++j) 
        { 
            key[j] = tmpK[j]; 
        } 
     
        for (int i = 0; i < CryptoPP::AES::BLOCKSIZE; ++i) 
        { 
            iv[i] = tmpIV[i]; 
        } 
        */  
    }  
      
    char* encrypt(char *sourData, int leng)  
    {  
        string cipherText;  
      
        CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);  
        CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );  
        CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( cipherText ));
        stfEncryptor.Put(sourData, leng + 1 );  
        stfEncryptor.MessageEnd();  
      
        string cipherTextHex;  
		char *retChar = new char[cipherText.size()];
		printf("encrypt  file length : %d \n", cipherText.size());
        for( int i = 0; i < cipherText.size(); i++ )  
        {  
            *(retChar + i) = static_cast<byte>(cipherText[i]);
        }  
		retDataSize = cipherText.size();
        return retChar;  
    }  
      
      
      
    void writeCipher(string output)  
    {  
        ofstream out("/tmp/cipher.data");  
        out.write(output.c_str(), output.length());  
        out.close();  
      
        cout<<"writeCipher finish "<<endl<<endl;  
    }  
      
      
      
int main(int argc, char *argv[])  
{  
	std::string source, output;
	int isencode = 0;
	int c;
	initKV();
	while((c = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
		printf("[+]get options : %d \n", c);
		switch (c) {
			case 'e':
				source = optarg;
				isencode = 1;
				break;
			case 'd':
				printf("[+]get options 1 : %d \n", c);
				source = optarg;
				printf("[+]get options 2 : %d \n", c);
				isencode = 2;
				printf("[+]get options 3 : %d \n", c);
				break;
			case 'o':
				output = optarg;
				break;
			case 'h':
				useage();
				return 0;
				break;
			default:
				break;
		}
	}
	printf("[-]open  %s gooooo, cnum : %d \n",source.c_str(), c);
	
	int fd=open(source.c_str(), O_RDWR);
	if(!fd) {
		printf("source so file cannot found!!!\n");
		return -1;
	}
	struct stat buf={0};
	printf("step 1 \n");
	int status=stat(source.c_str(),&buf);            // 获取文件信息
	printf("[+]fstat %s path, size : %d \n",source.c_str(), buf.st_size);
	if (status==-1) {
		printf("[-]fstat %s failed \n",source.c_str());
		return -1;
	}
	printf("step 2 \n");
	int g_dexSize = buf.st_size;
	printf("step 3 \n");
	char *readbuf = new char[g_dexSize];
	printf("step 4 \n");
	int ret = read(fd, readbuf, g_dexSize);
	printf("step 5 ret: %d \n", ret);
	if (ret != g_dexSize) {
		printf("[-]read %d failed",g_dexSize);
	}
	printf("step 6 \n");
	char *rbuf = readbuf;
	string s = rbuf;
	char* rstr;
	int lenss = s.length();
	printf("step 7 lensss : %d\n", lenss);
	if (isencode == 1) {
		for (int i = 0; i < 32; i ++) {
			printf("[+] jarray i1 start index: %x, content: %x \n", i, *(readbuf+i));
		}
		for (int i = g_dexSize - 32; i < g_dexSize; i ++) {
			printf("[+] jarray i1 end index: %x, content: %x \n", i, *(readbuf+i));
		}
		rstr = EncryptionAES(readbuf, g_dexSize);
		printf("step 8 rstr\n");
		
		
	}
	if (isencode == 2) {
		for (int i = 0; i < 32; i ++) {
			printf("[+] jarray i2 start index: %x, content: %x \n", i, *(readbuf+i));
		}
		for (int i = g_dexSize - 32; i < g_dexSize; i ++) {
			printf("[+] jarray i2 end index: %x, content: %x \n", i, *(readbuf+i));
		}
		//rstr = DecryptionAES(readbuf, g_dexSize);
	}
	printf("step 9 \n");
	//const char *t = rstr.data();
	//printf("step 10 t : %c \n", t);
	//int len = rstr.length();
	printf("[+]encode length :%d \n", retDataSize);
	
	
	auto wfd=fopen(output.c_str(), "wb+");
	if(nullptr == wfd) {
        printf("output so file cannot write !!!\n");
        return -1;
	}
	printf("step 11 wfd\n");
	//int wsize = write(wfd, t, len);
	int wsize = fwrite(rstr, retDataSize, 1, wfd);
	printf("step 12 wfd : %d \n", wsize);
	if (wsize != 1) {
		printf("[-]encode %d error", wsize);
	}
	close(fd);
	fclose(wfd);
	printf("step 13 end \n");
	return 0;
}

void useage() {
    printf(" Options are:\n");

    printf("  -d --decode                                DecryptionAES  \n");
    printf("  -e --memso memBaseAddr(16bit format)       EncryptionAES \n");
    printf("  -o --output generateFilePath               Generate file path\n");
    printf("  -h --help                                  Display this information\n");
}