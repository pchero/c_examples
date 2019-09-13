#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <getopt.h>

#define OFFSET 2208988800ULL

void ntp2tv(uint8_t ntp[8], struct timeval *tv)
{
    uint64_t aux = 0;
    uint8_t *p = ntp;
    int i;

    /* we get the ntp in network byte order, so we must
     * convert it to host byte order. */
    for (i = 0; i < sizeof(ntp) / 2; i++) {
        aux <<= 8;
        aux |= *p++;
    } /* for */

    /* now we have in aux the NTP seconds offset */
    aux -= OFFSET;
    tv->tv_sec = aux;

    /* let's go with the fraction of second */
    aux = 0;
    for (; i < sizeof(ntp); i++) {
        aux <<= 8;
        aux |= *p++;
    } /* for */

    /* now we have in aux the NTP fraction (0..2^32-1) */
    aux *= 1000000; /* multiply by 1e6 */
    aux >>= 32;     /* and divide by 2^32 */
    tv->tv_usec = aux;
} /* ntp2tv */

void tv2ntp(struct timeval *tv, uint8_t ntp[8])
{
    uint64_t aux = 0;
    uint8_t *p = ntp + sizeof(ntp);
    int i;

    aux = tv->tv_usec;
    aux <<= 32;
    aux /= 1000000;

    /* we set the ntp in network byte order */
    for (i = 0; i < sizeof(ntp)/2; i++) {
        *--p = aux & 0xff;
        aux >>= 8;
    } /* for */

    aux = tv->tv_sec;
    aux += OFFSET;

    /* let's go with the fraction of second */
    for (; i < sizeof(ntp); i++) {
        *--p = aux & 0xff;
        aux >>= 8;
    } /* for */

} /* ntp2tv */

size_t print_tv(struct timeval *t)
{
    return printf("%ld.%06ld\n", t->tv_sec, t->tv_usec);
}

size_t print_ntp(uint8_t ntp[8])
{
    int i;
    int res = 0;
    for (i = 0; i < sizeof(ntp); i++) {
        if (i == sizeof(ntp) / 2)
            res += printf(".");
        res += printf("%02x", ntp[i]);
    } /* for */
    res += printf("\n");
    return res;
} /* print_ntp */


int main(int argc, char *argv[])
{
    struct timeval t;
    uint8_t ntp[8];

    gettimeofday(&t, NULL);

    printf("tv2ntp\n");
    tv2ntp(&t, ntp);
    printf("tv : "); print_tv(&t);
    printf("ntp: "); print_ntp(ntp);

    printf("ntp2tv\n");
    ntp2tv(ntp, &t);
    printf("tv : "); print_tv(&t);
    printf("ntp: "); print_ntp(ntp);
}
