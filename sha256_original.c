#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sha2.h>
#include <sha256.h>
#include <string.h>
int main(){
SHA2_CTX ctx;
           uint8_t results[SHA256_DIGEST_LENGTH];
           char *buf;
           int n;

           buf = "abc";
           n = strlen(buf);
           SHA256Init(&ctx);
           SHA256Update(&ctx, (uint8_t *)buf, n);
           SHA256Final(results, &ctx);

           /* Print the digest as one long hex value */
           printf("0x");
           for (n = 0; n < SHA256_DIGEST_LENGTH; n++)
                   printf("%02x", results[n]);
           putchar('\n');
return 0;
}
