#include <stdlib.h>
#include <string.h>
#include "atcmd.h"
#if QUEC_AT_ENABLE

static int smd_fd = -1;

uint8_t getImei(uint8_t *mobile) {
    smd_fd = open(QUEC_AT_PORT, O_RDWR | O_NONBLOCK | O_NOCTTY);
    printf("< open(\"%s\")=%d >\n", QUEC_AT_PORT, smd_fd);

    char buf[100] = {0};
    Ql_SendAT("ATE0", "OK", buf, 3000);
    Ql_SendAT("AT+GSN", "OK", buf, 3000);
    printf("GSN: %s\r\n", buf);

    int i,j;
    for(i = 0; buf[i] != '\0'; ++i)
    {
        while (!( (buf[i] >= '0' && buf[i] <= '9') || buf[i] == '\0') )
        {
            for(j = i; buf[j] != '\0'; ++j)
            {
            	buf[j] = buf[j+1];
            }
            buf[j] = '\0';
        }
    }

    char * pEnd;
    toBcd(mobile, strtoll(buf+3, pEnd, 10));
    close(smd_fd);
}

int Ql_SendAT(char* atCmd, char* finalRsp, char* strResponse, long timeout_ms)
{
    int iRet;
    int iLen;
    fd_set fds;
    int rdLen;
#define lenToRead 100
    char strAT[100];
    char strFinalRsp[100];
//    char strResponse[100];
    struct timeval timeout = {0, 0};
    bool bRcvFinalRsp = FALSE;

    memset(strAT, 0x0, sizeof(strAT));
    iLen = sizeof(atCmd);
    strncpy(strAT, atCmd, iLen);

    sprintf(strFinalRsp, "\r\n%s", finalRsp);
    
	timeout.tv_sec  = timeout_ms / 1000;
	timeout.tv_usec = timeout_ms % 1000;
    
    
    // Add <cr><lf> if needed
    iLen = strlen(atCmd);
    if ((atCmd[iLen-1] != '\r') && (atCmd[iLen-1] != '\n'))
    {
        iLen = sprintf(strAT, "%s\r\n", atCmd); 
        strAT[iLen] = 0;
    }

    // Send AT
    iRet = write(smd_fd, strAT, iLen);
    printf(">>Send AT: \"%s\", iRet=%d\n", atCmd, iRet);
    
    // Wait for the response
	while (1)
	{
		FD_ZERO(&fds); 
		FD_SET(smd_fd, &fds); 

		printf("timeout.tv_sec=%d, timeout.tv_usec: %d \n", (int)timeout.tv_sec, (int)timeout.tv_usec);
		switch (select(smd_fd + 1, &fds, NULL, NULL, &timeout))
		//switch (select(smd_fd + 1, &fds, NULL, NULL, NULL))	// block mode
		{
		case -1: 
			printf("< select error >\n");
			return -1;

		case 0:
			printf("< time out >\n");
			return 1; 

		default: 
			if (FD_ISSET(smd_fd, &fds)) 
			{
				do {
					memset(strResponse, 0x0, sizeof(strResponse));
					rdLen = read(smd_fd, strResponse, lenToRead);
					printf(">>Read response/urc, len=%d, content:\n%s\n", rdLen, strResponse);
					//printf("rcv:%s", strResponse);
					//printf("final rsp:%s", strFinalRsp);
					if ((rdLen > 0) && strstr(strResponse, strFinalRsp))
					{
					    if (strstr(strResponse, strFinalRsp)     // final OK response
					       || strstr(strResponse, "+CME ERROR:") // +CME ERROR
					       || strstr(strResponse, "+CMS ERROR:") // +CMS ERROR
					       || strstr(strResponse, "ERROR"))      // Unknown ERROR
					    {
					        //printf("\n< match >\n");
					        bRcvFinalRsp = TRUE;
					    }else{
					        printf("\n< not final rsp >\n");
					    }
					}
				} while ((rdLen > 0) && (lenToRead == rdLen));
			}else{
				printf("FD is missed\n");
			}
			break;
		}

		// Found the final response , return back
		if (bRcvFinalRsp)
		{
		    break;
		}	
   	}
   	return 0;
}
#endif

void toBcd(uint8_t *bcd, long long num) {
    int digits = 0;

    long long quot = num;
    while (quot != 0) {
      quot /= 10;
      digits++;
    }

    int len = digits % 2 == 0 ? digits / 2 : digits / 2 + 1;

    //uint8_t *bcd = malloc(6*sizeof(uint8_t));

    uint8_t digit;
    int i;
    for (i = digits; i > 0; i--) {
      digit = (uint8_t) (num % 10);
      if (i % 2 == 0) {
        bcd[i / 2 - 1] = digit;
      } else {
        bcd[i / 2] |= digit << 4;
      }
      num /= 10;
    }

    return;
}

uint8_t ascII2Bcd(uint8_t asc) {
	if ((asc >= '0') && (asc <= '9'))
			return (uint8_t) (asc - '0');
	else if ((asc >= 'A') && (asc <= 'F'))
			return (uint8_t) (asc - 'A' + 10);
	else if ((asc >= 'a') && (asc <= 'f'))
			return (uint8_t) (asc - 'a' + 10);
	else
			return (uint8_t) (asc - 48);
}

uint8_t* string2Bcd(uint8_t * dis, uint8_t * str) {
	uint8_t len = strlen(str);
	//uint8_t *ret = malloc(strlen(strr)*sizeof(uint8_t) / 2);
	uint8_t *bs = str;
	int i;
	for (i = 0; i < len; i++) {
			uint8_t high = ascII2Bcd(bs[2 * i]);
			uint8_t low = ascII2Bcd(bs[2 * i + 1]);

			dis[i] = (uint8_t) ((high << 4) | low);
	}
	return dis;
}
