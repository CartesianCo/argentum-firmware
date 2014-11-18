void decb_init();

#define KEEP_GOING 0
#define NEED_MORE_DATA 1
#define DECODE_ERROR 2
int decb(char *inbuf, int *pinoff, int inlen, char *outbuf, int *poutlen);
