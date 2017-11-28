 ///
 /// @file    main.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2017-11-22 15:12:17
 ///
 
#include "openssl/ssl.h"
#include "openssl/bio.h"
#include "openssl/err.h"


int main(){
	SSL_load_error_strings(); 
	ERR_load_BIO_strings(); 
	OpenSSL_add_all_algorithms();
	
	BIO *bio;
	return 0;
}
