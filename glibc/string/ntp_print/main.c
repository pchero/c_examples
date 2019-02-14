#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

static void timeval2ntp(struct timeval tv, unsigned int *msw, unsigned int *lsw);
static void ntp2timeval(unsigned int msw, unsigned int lsw, struct timeval *tv);

static struct timeval convert_rtt2tv(unsigned int rtt);

ssize_t format_timeval(struct timeval *tv, char *buf, size_t sz);

static void test_rtt(void);

int main(int argc, char** argv) 
{
    unsigned int msw;
    unsigned int lsw;
    unsigned int lsr;
    unsigned int lsr_a;
    unsigned int delay;
    unsigned int rtt;
    struct timeval tv;
    struct timeval now;
    char tmp[1024];


    msw = 0xe00ddf9b;
    lsw = 0x0f1a2980;
    lsr = 0xdf982367;
    delay = 191037;
    printf("%u, %u\n", msw, lsw);

    // test ntp2timeval
    ntp2timeval(msw, lsw, &tv);
    
    format_timeval(&tv, tmp, sizeof(tmp));

    // strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", tv);
    // Feb 13, 2019 00:13:15.058992952 CET
    printf("%s\n", tmp);
    printf("test %f\n", (double)lsw/65536);

    msw = 0;
    lsw = 0;
    timeval2ntp(tv, &msw, &lsw);
    printf("%u, %u\n", msw, lsw);

    // test
    gettimeofday(&now, NULL);
	timeval2ntp(now, &msw, &lsw);
    lsr_a = ((msw & 0x0000ffff) << 16) | ((lsw & 0xffff0000) >> 16);

    rtt = lsr_a - lsr - delay;
    convert_rtt2tv(rtt);


    test_rtt();

    return 0;
}

static void test_rtt(void)
{
    unsigned int msw;
    unsigned int lsw;
    unsigned int lsr;
    unsigned int lsr_a;
    unsigned int dlsr;
    unsigned int rtt;
    struct timeval now;
    struct timeval rtt_tv;
    double tmp_double;
    unsigned int dlsr_sec;
    unsigned int dlsr_frac;

    // init value
    dlsr = 191037;
    lsr = 3751289703;
	timeval2ntp(now, &msw, &lsw);

    // test 1(asterisk)
    // get current compact timestamp(ntp)
    lsr_a = ((msw & 0x0000ffff) << 16) | ((lsw & 0xffff0000) >> 16);
    rtt = lsr_a - lsr - dlsr;
    rtt_tv = convert_rtt2tv(rtt);
    printf("test1: %ld.%ld\n", rtt_tv.tv_sec, rtt_tv.tv_usec);


    // new way 1
    lsr_a = ((msw & 0x0000ffff) << 16) | ((lsw & 0xffff0000) >> 16);
    rtt = lsr_a - lsr;
    rtt_tv = convert_rtt2tv(rtt);
    printf("test2: %ld.%ld\n", rtt_tv.tv_sec, rtt_tv.tv_usec);

	tmp_double = dlsr / 65536;
	dlsr_sec = tmp_double;
	dlsr_frac = tmp_double - dlsr_sec;

    printf("test2: %ld, %u\n", rtt_tv.tv_usec, dlsr_frac * 1000000);
	rtt_tv.tv_sec -= dlsr_sec;
	rtt_tv.tv_usec -= dlsr_frac * 1000000;
    printf("test2: %ld.%ld\n", rtt_tv.tv_sec, rtt_tv.tv_usec);


    // test 2
    lsr_a = ((msw & 0x0000ffff) << 16) | ((lsw & 0xffff0000) >> 16);

    struct timeval lsr_a_tv;
    lsr_a_tv = convert_rtt2tv(lsr_a);

    struct timeval lsr_tv;
    lsr_tv = convert_rtt2tv(lsr);

    rtt_tv.tv_sec = lsr_a_tv.tv_sec - lsr_tv.tv_sec;
    rtt_tv.tv_usec = lsr_a_tv.tv_usec - lsr_tv.tv_usec;
    printf("test3: %ld.%ld\n", rtt_tv.tv_sec, rtt_tv.tv_usec);

	tmp_double = dlsr / 65536;
	dlsr_sec = tmp_double;
	dlsr_frac = tmp_double - dlsr_sec;

	rtt_tv.tv_sec -= dlsr_sec;
	rtt_tv.tv_usec -= dlsr_frac * 1000000;
    printf("test3: %ld.%ld\n", rtt_tv.tv_sec, rtt_tv.tv_usec);


}

static struct timeval convert_rtt2tv(unsigned int rtt)
{
    struct timeval rtt_tv;

    unsigned int rtt_msw;
    unsigned int rtt_lsw;

    rtt_msw = (rtt & 0xffff0000) >> 16;
	rtt_lsw = (rtt & 0x0000ffff);

    rtt_tv.tv_sec = rtt_msw;
    rtt_tv.tv_usec = (rtt_lsw * 15625) >> 10;;

    return rtt_tv;
}

static void timeval2ntp(struct timeval tv, unsigned int *msw, unsigned int *lsw)
{
	unsigned int sec, usec, frac;
	sec = tv.tv_sec + 2208988800u; /* Sec between 1900 and 1970 */
	usec = tv.tv_usec;
	/*
	 * Convert usec to 0.32 bit fixed point without overflow.
	 *
	 * = usec * 2^32 / 10^6
	 * = usec * 2^32 / (2^6 * 5^6)
	 * = usec * 2^26 / 5^6
	 *
	 * The usec value needs 20 bits to represent 999999 usec.  So
	 * splitting the 2^26 to get the most precision using 32 bit
	 * values gives:
	 *
	 * = ((usec * 2^12) / 5^6) * 2^14
	 *
	 * Splitting the division into two stages preserves all the
	 * available significant bits of usec over doing the division
	 * all at once.
	 *
	 * = ((((usec * 2^12) / 5^3) * 2^7) / 5^3) * 2^7
	 */
	frac = ((((usec << 12) / 125) << 7) / 125) << 7;
	*msw = sec;
	*lsw = frac;
}

static void ntp2timeval(unsigned int msw, unsigned int lsw, struct timeval *tv)
{
	tv->tv_sec = msw - 2208988800u;
	/* Reverse the sequence in timeval2ntp() */
	tv->tv_usec = ((((lsw >> 7) * 125) >> 7) * 125) >> 12;
}

ssize_t format_timeval(struct timeval *tv, char *buf, size_t sz)
{
  ssize_t written = -1;
  struct tm *gm = gmtime(&tv->tv_sec);

  if (gm)
  {
    written = (ssize_t)strftime(buf, sz, "%Y-%m-%dT%H:%M:%S", gm);
    if ((written > 0) && ((size_t)written < sz))
    {
      int w = snprintf(buf+written, sz-(size_t)written, ".%06ldZ", tv->tv_usec);
      written = (w > 0) ? written + w : -1;
    }
  }
  return written;
}
