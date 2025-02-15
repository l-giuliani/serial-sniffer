#include "utils.h"

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>

namespace NUtils {

/**
 * @brief time to log str
*/
static char *FTimeToLog( char *buff) {
	struct	tm  tt;
	struct	timeb  tb;

	ftime(&tb);
	localtime_r( &tb.time, &tt );
  	tt.tm_year += 1900;
  	tt.tm_mon  += 1;

	sprintf( buff, "%04d-%02d-%02d %02d:%02d:%02d:%03d",
		tt.tm_year,tt.tm_mon,tt.tm_mday,
		tt.tm_hour,tt.tm_min,tt.tm_sec,tb.millitm );
	return( buff );
}

/**
 * @brief print hex log on buffer
*/
static void printbuf( char *title, char *buf, short len, char* out, int* size ) {
#define BUFL	360
	char	s[BUFL], *p;
	int 	i, j, k, l;
	char	log_time[BUFL];
    const char  nibble2ascii[]="0123456789ABCDEF";
	*size = sprintf(out, "\n%s len = %04x  Time=%s", title, len, FTimeToLog( log_time) );
/* ==== prova ======
Logger(PRV_FILE,"%s len = %04x", title, len );
 ==== prova ======*/
	k = j = 0;
	memset( s, ' ', BUFL );
	for(i=0;i<len;i++)
	{
		s[3*j] = nibble2ascii[(buf[i] & 0xf0)>>4];
		s[3*j+1] = nibble2ascii[(buf[i] & 0x0f)];
		s[3*j+2] = ' ';
		j++;
		if( j == 16 )
		{
			p = &s[3*j];
			p += 3;
			for( l = 0; l<16; l++ )
			{
				if((buf[k] >= 0x20)&&(buf[k]<=0x7F))
					*p = buf[k];
				else
					*p = '.';
				k++;
				p++;
			}
			*p = '\0';
			*size += sprintf(&out[*size], "\n%03x0  %s", i/16, s );
			j = 0;
			k = i+1;
			memset( s, ' ', BUFL );
		}
	}
	if( len % 16 )
	{
		p = &s[3*16];
		p+=3;
		while( k < len )
		{
			if((buf[k] >= 0x20)&&(buf[k]<=0x7F))
				*p = buf[k];
			else
				*p = '.';
			k++;
			p++;
		}
		*p = '\0';
		*size += sprintf(&out[*size], "\n%03x0  %s", i/16, s );
	}
	*size += sprintf(&out[*size], "\n" );
}

/**
 * @brief perform hex formatting
*/
std::string HexFormatter::format(const std::string& str) {
    char buff[4096];
    int len;
    printbuf( (char*)"\0", (char*)str.c_str(), str.size(), buff, &len);
    std::string out(buff, len);
    return out;
}

}