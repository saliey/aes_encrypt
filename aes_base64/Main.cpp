#include <iostream>  
#include "AES.h"  
#include "Base64.h"  

#include <getopt.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

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
string EncryptionAES(const string& strSrc) //AES����
{  
	printf("EncryptionAES sss size  b_num\n");
    size_t length = strSrc.length();  
    int block_num = length / BLOCK_SIZE + 1;  
    //����  
    char* szDataIn = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataIn, 0x00, block_num * BLOCK_SIZE + 1);
    strcpy(szDataIn, strSrc.c_str());  
    
    //����PKCS7Padding��䡣  
    int k = length % BLOCK_SIZE;  
    int j = length / BLOCK_SIZE;  
    int padding = BLOCK_SIZE - k;  
    for (int i = 0; i < padding; i++)  
    {  
        szDataIn[j * BLOCK_SIZE + k + i] = padding;  
    }  
    szDataIn[block_num * BLOCK_SIZE] = '\0';  
    
    //���ܺ������  
    char *szDataOut = new char[block_num * BLOCK_SIZE + 1];  
    memset(szDataOut, 0, block_num * BLOCK_SIZE + 1);  
    
    //���н���AES��CBCģʽ����
    AES aes;  
    aes.MakeKey(g_key, g_iv, 16, 16);  
    aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, AES::CBC);  
    string str = base64_encode((unsigned char*) szDataOut,  
            block_num * BLOCK_SIZE);  
    delete[] szDataIn;  
    delete[] szDataOut;  
    return str;  
}
char* EncryptionAES(char* strSrc, int leng) //AES����
{  
	printf("EncryptionAES 0 size  b_num : %d \n", leng);
    int block_num = leng / BLOCK_SIZE + 1;
	printf("EncryptionAES 1 size  b_num : %d, B_SIZE : %d \n", block_num, BLOCK_SIZE);
    //����  
    char* szDataIn = new char[block_num * BLOCK_SIZE + 1];
	printf("EncryptionAES 2  size  b_num : %d, B_SIZE : %d \n", block_num, BLOCK_SIZE);
    memset(szDataIn, 0x00, block_num * BLOCK_SIZE + 1);
    strcpy(szDataIn, strSrc);
    
    //����PKCS7Padding��䡣  
    int k = leng % BLOCK_SIZE;  
    int j = leng / BLOCK_SIZE;  
    int padding = BLOCK_SIZE - k;  
    for (int i = 0; i < padding; i++)
    {  
        szDataIn[j * BLOCK_SIZE + k + i] = padding;  
    }  
    szDataIn[block_num * BLOCK_SIZE] = '\0';
    
    //���ܺ������  
    char *szDataOut = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataOut, 0, block_num * BLOCK_SIZE + 1);
    retDataSize = block_num * BLOCK_SIZE + 1;
	printf("EncryptionAES size : %d , b_num : %d, B_SIZE : %d \n", retDataSize, block_num, BLOCK_SIZE);
    //���н���AES��CBCģʽ����
    AES aes;  
    aes.MakeKey(g_key, g_iv, 16, 16);  
    aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, AES::CBC);
    //string str = base64_encode((unsigned char*) szDataOut,  block_num * BLOCK_SIZE);
		for (int i = 0; i < 32; i ++) {
			printf("[+] jarray start index: %x, content: %x \n", i, *(szDataOut+i));
		}
		for (int i = retDataSize - 32; i < retDataSize; i ++) {
			printf("[+] jarray end index: %x, content: %x \n", i, *(szDataOut+i));
		}
    delete[] szDataIn;  
    //delete[] szDataOut;  
    return (char*) szDataOut;
}
char* DecryptionAES(char* strSrc, int leng) //AES����
{  
    //����  
    char *szDataIn = new char[leng + 1];
    memcpy(szDataIn, strSrc, leng+1);
    //����  
    char *szDataOut = new char[leng + 1];  
    memcpy(szDataOut, strSrc, leng+1);  
    printf("[+] DecryptionAES length : %d \n", leng);
    //����AES��CBCģʽ����  
    AES aes;  
    aes.MakeKey(g_key, g_iv, 16, 16);
    aes.Decrypt(szDataIn, szDataOut, leng, AES::CBC);
    
    //ȥPKCS7Padding���
    if (0x00 < szDataOut[leng - 1] <= 0x16)  
    {  
        int tmp = szDataOut[leng - 1];  
        for (int i = leng - 1; i >= leng - tmp; i--)  
        {  
            if (szDataOut[i] != tmp)  
            {  
                memset(szDataOut, 0, leng);  
                cout << "ȥ���ʧ�ܣ����ܳ�����" << endl;  
                break;  
            }  
            else  
                szDataOut[i] = 0;  
        }  
    }  
	retDataSize = leng + 1;
	printf("EncryptionAES size : %d , b_num  B_SIZE : %d \n", retDataSize, BLOCK_SIZE);
		for (int i = 0; i < 32; i ++) {
			printf("[+] jarray start index: %x, content: %x \n", i, *(szDataOut+i));
		}
		for (int i = retDataSize - 32; i < retDataSize; i ++) {
			printf("[+] jarray end index: %x, content: %x \n", i, *(szDataOut+i));
		}
		printf("[+] -------------------------------------------");
		for (int i = 0; i < 32; i ++) {
			printf("[+] jarray start index: %x, content: %x \n", i, *(szDataIn+i));
		}
		for (int i = retDataSize - 32; i < retDataSize; i ++) {
			printf("[+] jarray end index: %x, content: %x \n", i, *(szDataIn+i));
		}
		printf("[+] ----------------------ret---------------------");
    //string strDest(szDataOut);
    delete[] szDataIn;  
    //delete[] szDataOut;  
    return szDataOut;  
}  
string DecryptionAES(const string& strSrc) //AES����  
{  
    string strData = base64_decode(strSrc);  
    size_t length = strData.length();  
    //����  
    char *szDataIn = new char[length + 1];  
    memcpy(szDataIn, strData.c_str(), length+1);  
    //����  
    char *szDataOut = new char[length + 1];  
    memcpy(szDataOut, strData.c_str(), length+1);  
    
    //����AES��CBCģʽ����  
    AES aes;  
    aes.MakeKey(g_key, g_iv, 16, 16);  
    aes.Decrypt(szDataIn, szDataOut, length, AES::CBC);  
    
    //ȥPKCS7Padding���  
    if (0x00 < szDataOut[length - 1] <= 0x16)  
    {  
        int tmp = szDataOut[length - 1];  
        for (int i = length - 1; i >= length - tmp; i--)  
        {  
            if (szDataOut[i] != tmp)  
            {  
                memset(szDataOut, 0, length);  
                cout << "ȥ���ʧ�ܣ����ܳ�����" << endl;  
                break;  
            }  
            else  
                szDataOut[i] = 0;  
        }  
    }  
    string strDest(szDataOut);  
    delete[] szDataIn;  
    delete[] szDataOut;  
    return strDest;  
}  
int main(int argc, char *argv[])  
{  
/*    string str1;
    cin >> str1; 
    cout << "����ǰ:" << str1 << endl;
    string str2 = EncryptionAES(str1);  
    cout << "���ܺ�:" << str2 << endl;  
    string str3 = DecryptionAES(str2);  
    cout << "���ܺ�:" << str3 << endl;  */
	if (argc != 5) {
		printf("Usage: %s <option -e/-d> <input file> <option -o> <output file>\n", argv[0]);
		useage();
		return -1;
	}
	std::string source, output;
	int isencode = 0;
	int c;
	while((c = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
		printf("[+]get options : %d \n", c);
		switch (c) {
			case 'e':
				source = optarg;
				isencode = 2;
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
	int status=stat(source.c_str(),&buf);            // ��ȡ�ļ���Ϣ
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
		rstr = DecryptionAES(readbuf, g_dexSize);
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



