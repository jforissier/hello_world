/***
*
* FILENAME :
*
*        test.c
*
* DESCRIPTION :
*
*        Test en/decrypt functions in TEE
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
* DATE    		WHO               DETAIL
* 15.08.2017   Ursus Schneider 	Initial release
*
***/

#include <stdio.h>

#include "../types.h"
#include "test.h"

// main
int main(int argc, char *argv[]) {

   // header
   printf("\nTesting the en/decrypt function in TEE\n");

   // local encrypt and decrypt i.e. using openssl
   local_encrypt_and_decrypt ();

   // TA encrypt and decrypt
   local_encrypt_and_decrypt_in_secure_world ();
   encrypt_in_secure_world ();
   decrypt_in_secure_world ();

   // finally finished
   printf("Testing finished\n\n");
   return 0;
}
