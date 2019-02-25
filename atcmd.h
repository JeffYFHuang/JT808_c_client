#include <stdint.h>
#define QUEC_AT_ENABLE 0

#if AT_ENABLE

#include "ql_oe.h"

#define QUEC_AT_PORT    "/dev/smd8"

uint8_t getImei(uint8_t *mobile);
int Ql_SendAT(char* atCmd, char* finalRsp, char* rsp ,long timeout_ms);

#endif

void toBcd(uint8_t *bcd, long long num);
uint8_t ascII2Bcd(uint8_t asc);
uint8_t* string2Bcd(uint8_t * dis, uint8_t * str);
