#include <stdint.h>
#include <inttypes.h>
int main(void)
{
uint8_t TX_buffer [4] = {0};
int32_t temp = 920;
 printf("%" PRIu32 "\n",temp);
TX_buffer[3] = (uint8_t) ((temp & 0xFF000000) >> 24 );
TX_buffer[2] = (uint8_t) ((temp & 0x00FF0000) >> 16 );
TX_buffer[1] = (uint8_t) ((temp & 0x0000FF00) >> 8  );
TX_buffer[0] = (uint8_t) ((temp & 0X000000FF)       );

printf("%" PRIu8, TX_buffer[0]);
printf(" ");
printf("%" PRIu8, TX_buffer[1]);
printf(" ");
printf("%" PRIu8, TX_buffer[2]);
printf(" ");
printf("%" PRIu8 "\n", TX_buffer[3]);


int32_t  tempreture  = ((int32_t)(TX_buffer[3]) << 24)
              + ((int32_t)(TX_buffer[2]) << 16)
              + ((int32_t)(TX_buffer[1]) << 8)
              + ((int32_t)(TX_buffer[0]));

printf("%" PRIu32 "\n",tempreture);

}
