/***
*
* FILENAME :
*
*        crypto.h
*
* DESCRIPTION :
*
*        Crypto functions
*
* NOTES :
*
*        Copyright Ursus Schneider 2017.  All rights reserved.
*
* AUTHOR :
*
*        Ursus Schneider        START DATE :   15 August 2017
*
* CHANGES :
*
* DATE    		 WHO     				DETAIL
* 15.08.2017    Ursus Schneider 	   Initial release
*
***/

#ifndef CRYPTO_H
#define CRYPTO_H

BOOLEAN decrypt_using_public_key (char * in, int in_len, char * out, int * out_len);
BOOLEAN encrypt_using_private_key (char * in, int in_len, char * out, int * out_len);

#endif
