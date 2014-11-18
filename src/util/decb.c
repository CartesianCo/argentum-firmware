#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "decb.h"

//#define DEBUG

static char lastFiringLine[64];
static char lastFiring[4];
static char lastParts[50];
static char nLastParts;

#ifdef DEBUG
int lineno = 0;
#endif

void decb_init()
{
    lastFiringLine[0] = 0;
    lastFiring[0] = 0;
    memset(lastParts, 0, 50);
    nLastParts = 0;
}

static int ishexdigit(char ch)
{
    return ch >= '0' && ch <= '9' || ch >= 'A' && ch <= 'F';
}

int decb(char *inbuf, int *pinoff, int inlen, char *outbuf, int *poutlen)
{
    int outlen = *poutlen;
    *poutlen = 0;
    for (;;)
    {
        if (*poutlen == outlen)
            return KEEP_GOING;

#ifdef DEBUG
        lineno++;
#endif
        char *line = inbuf + *pinoff;
        int lineLen;
        int ncommas = 0;
        for (lineLen = 0; ; lineLen++)
            if (*pinoff + lineLen >= inlen)
                return NEED_MORE_DATA;
            else if (line[lineLen] == '\n')
                break;
            else if (line[lineLen] == ',')
                ncommas++;

        if (line[0] == 'X')
        {
            if (*poutlen + lineLen + 4 > outlen)
                return KEEP_GOING;
            memcpy(outbuf + *poutlen, "M X ", 4);
            memcpy(outbuf + *poutlen + 4, line + 1, lineLen);
            *poutlen += 4 + lineLen;
            *pinoff += lineLen + 1;
            continue;
        }

        if (ncommas == 0 && line[0] != 'd')
        {
            if (*poutlen + lineLen + 5 > outlen)
                return KEEP_GOING;
            memcpy(outbuf + *poutlen, "M Y ", 4);
            memcpy(outbuf + *poutlen + 4, line, lineLen+1);
            *poutlen += 5 + lineLen;
            *pinoff += lineLen + 1;
            continue;
        }

        char *firingLine = NULL;
        if (line[0] == 'd' && lineLen == 1)
        {
            if (lastFiringLine[0] == 0)
            {
#ifdef DEBUG
                fprintf(stderr, "no last firing line on line %d.\n", lineno);
#endif
                return DECODE_ERROR;
            }
            firingLine = lastFiringLine;
        }


        char *order="84C2A6E195D3B";
        if (ncommas == 12 || firingLine)
        {
            if (outlen - *poutlen < 13 * 8)
                return KEEP_GOING;
            if (firingLine == 0)
            {
                if (lineLen > sizeof(lastFiringLine)-1)
                {
#ifdef DEBUG
                    fprintf(stderr, "firing line too long.\n");
#endif
                    return DECODE_ERROR;
                }
                memcpy(lastFiringLine, line, lineLen);
                lastFiringLine[lineLen] = 0;
            }
            int firingLineLen = firingLine ? strlen(firingLine) : lineLen;
            firingLine = firingLine ? firingLine : line;
            char *value = firingLine;
            int i;
            for (i = 0; i < 13; i++)
            {
                char zone[4];
                char *firing = NULL;
                if (value - firingLine == firingLineLen || *value == ',')
                {
                    if (lastFiring[0] == 0)
                    {
#ifdef DEBUG
                        fprintf(stderr, "no last firing on line %d.\n", lineno);
#endif
                        return DECODE_ERROR;
                    }
                    firing = lastFiring;
                }
                else if (*value == 'z' &&
                         (value - firingLine + 1 == firingLineLen ||
                          value[1] == ','))
                {
                    firing = "0000";
                    value += 1;
                }
                else if (*value == 'z' &&
                         (value - firingLine + 2 == firingLineLen ||
                          (value[2] == ',' &&
                           value[1] >= 'a' && value[1] <= 'y')))
                {
                    char *part = lastParts + (value[1] - 'a') * 2;
                    if (part[0] == 0 || part[1] == 0)
                    {
#ifdef DEBUG
                        firingLine[firingLineLen] = 0;
                        fprintf(stderr, "bad zpart on line %d pos %d nLastParts=%d value:%s\n", lineno, i, nLastParts, value);
#endif
                        return DECODE_ERROR;
                    }
                    memcpy(zone, "00", 2);
                    memcpy(zone + 2, part, 2);
                    firing = zone;
                    value += 2;
                }
                else if (*value == 'z' &&
                         (value - firingLine + 3 == firingLineLen ||
                          value[3] == ',') &&
                         ishexdigit(value[1]) && ishexdigit(value[2]))
                {
                    memcpy(zone, "00", 2);
                    memcpy(zone + 2, value + 1, 2);
                    firing = zone;
                    if (nLastParts == 25)
                    {
                        memmove(lastParts, lastParts + 2, 24*2);
                        nLastParts--;
                    }
                    memcpy(lastParts + nLastParts * 2, value + 1, 2);
                    nLastParts++;
                    value += 3;
                }
                else if ((value - firingLine + 1 == firingLineLen ||
                          value[1] == ',') &&
                          value[0] >= 'a' && value[0] <= 'y')
                {
                    char *part = lastParts + (value[0] - 'a') * 2;
                    if (part[0] == 0 || part[1] == 0)
                    {
#ifdef DEBUG
                        firingLine[firingLineLen] = 0;
                        fprintf(stderr, "bad part on line %d pos %d nLastParts=%d value=%s\n", lineno, i, nLastParts, value);
#endif
                        return DECODE_ERROR;
                    }
                    memcpy(zone, part, 2);
                    memcpy(zone + 2, "00", 2);
                    firing = zone;
                    value += 1;
                }
                else if ((value - firingLine + 2 == firingLineLen ||
                         value[2] == ',') &&
                         ishexdigit(value[0]) && ishexdigit(value[1]))
                {
                    memcpy(zone, value, 2);
                    memcpy(zone + 2, "00", 2);
                    firing = zone;
                    if (nLastParts == 25)
                    {
                        memmove(lastParts, lastParts + 2, 24*2);
                        nLastParts--;
                    }
                    memcpy(lastParts + nLastParts * 2, value, 2);
                    nLastParts++;
                    value += 2;
                }
                else
                {
#ifdef DEBUG
                    firingLine[firingLineLen] = 0;
                    fprintf(stderr, "What's this value on line %d pos %d:%s\n", lineno, i, value);
#endif
                    return DECODE_ERROR;
                }

                if (value - firingLine > firingLineLen)
                {
#ifdef DEBUG
                    firingLine[firingLineLen] = 0;
                    fprintf(stderr, "Value incremented too much on line %d pos %d firingLine:%s.\n", lineno, i, firingLine);
#endif
                    return DECODE_ERROR;
                }
                if (value - firingLine != firingLineLen)
                {
                    if (value[0] != ',')
                    {
#ifdef DEBUG
                        fprintf(stderr, "Expected a comma.\n");
#endif
                        return DECODE_ERROR;
                    }
                    value++;
                }

                if (firing == NULL || firing[0] == 0)
                {
#ifdef DEBUG
                    firingLine[firingLineLen] = 0;
                    fprintf(stderr, "No firing on line %d for pos %d, firingLine:%s value:%4s\n", lineno, i, firingLine, value);
#endif
                    return DECODE_ERROR;
                }
                memcpy(lastFiring, firing, 4);

                memcpy(outbuf + *poutlen, "F ", 2);
                outbuf[*poutlen + 2] = order[i];
                memcpy(outbuf + *poutlen + 3, firing, 4);
                outbuf[*poutlen + 7] = '\n';
                *poutlen += 8;
            }

            *pinoff += lineLen + 1;
            continue;
        }

#ifdef DEBUG
        line[lineLen] = 0;
        fprintf(stderr, "What's this on line %d:%s\n", lineno, line);
#endif
        return DECODE_ERROR;
    }
}

#ifdef DEBUG
int main(int argc, char **argv)
{
#define CHUNK_SIZE 1024
#define OVERLAP 64
    char buf[CHUNK_SIZE + OVERLAP];
    char buf2[512 + OVERLAP];

    if (argc < 2)
    {
        printf("usage: decb <b file>\n");
        return 1;
    }

    char *filename = argv[1];
    if (filename[strlen(filename)-1] != 'b' ||
        filename[strlen(filename)-2] != '.')
    {
        printf("expected a .b file extension.\n");
        return 1;
    }

    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "can't open input file.\n");
        return 1;
    }

    strcpy(buf, filename);
    buf[strlen(filename)-2] = 0;
    strcat(buf, ".out2");

    FILE *fout = fopen(buf, "w");
    if (fout == NULL)
    {
        fclose(f);
        fprintf(stderr, "can't open output file.\n");
        return 1;
    }

    decb_init();

    int inoff = 0;
    for (;;)
    {
        if (inoff > OVERLAP)
        {
            fprintf(stderr, "This won't work.\n");
            return 1;
        }

        int len = fread(buf + inoff, 1, CHUNK_SIZE, f);
        if (len <= 0)
            break;

        len += inoff;
        inoff = 0;

        int res = KEEP_GOING;
        while (res == KEEP_GOING)
        {
            int outlen = sizeof(buf2);
            res = decb(buf, &inoff, len, buf2, &outlen);
            if (res == DECODE_ERROR)
            {
                fclose(f);
                fclose(fout);
                fprintf(stderr, "decode error\n");
                return 1;
            }
            fwrite(buf2, 1, outlen, fout);
        }

        memmove(buf, buf+inoff, len-inoff);
        inoff = len-inoff;
    }

    fclose(f);
    fclose(fout);
    return 0;
}
#endif
