#include <avr/io.h>
#include <util/delay.h>

//INSTRUCTION_PACKET
#define HEADER 0xFF
#define INSTRUCTION 0x03

//ID
uint8_t ID[12]={1,2,3,4,5,6,7,8,9,10,11,12};

//uart_session
void UART_SET(){
  UCSR0A|=(1<<U2X0);
  //Baud_rate
  UBRR0H=(uint8_t)(1>>8);
  UBRR0L=(uint8_t)1;
  //frame_set
  UCSR0C|=(3<<UCSZ00);
  //ENABLE
  UCSR0B|=(1<<TXEN0)|(1<<RXEN0);

}

//uart_send
void uart_t(uint8_t *pack){

  uint8_t len_=sizeof(pack)/sizeof(*pack);
  for(uint8_t i=0;i<9;i++){
    while(!(UCSR0A&(1<<UDRE0)));
    UDR0=pack[i];
  }

}


//ax12a_protocol
void pos_goal(uint16_t pos_val ){
  //velo_set
  uint8_t checksum=9+4+3+32+(uint8_t)20;
  uint8_t packet_velo[]={HEADER, HEADER, ID_1,5,INSTRUCTION,0x20,200,~checksum};
  uart_t(packet_velo);
  checksum=3+4+3+32+(uint8_t)20;
  uint8_t packet_2_velo[]={HEADER, HEADER, ID_2,5,INSTRUCTION,0x20,200,~checksum};
  uart_t(packet_2_velo);

  _delay_ms(1000);
  //pos_set
  checksum=9+5+3+30+(uint8_t)(pos_val&0xFF)+(uint8_t)(pos_val>>8);
  uint8_t packet[]={HEADER, HEADER, ID_1,5,INSTRUCTION,0x1E,uint8_t(pos_val&0xFF),(uint8_t)(pos_val>>8),~checksum};
  uart_t(packet);
  checksum=3+5+3+30+(uint8_t)(pos_val&0xFF)+(uint8_t)(pos_val>>8);
  uint8_t packet_2[]={HEADER, HEADER, ID_2,5,INSTRUCTION,0x1E,uint8_t(pos_val&0xFF),(uint8_t)(pos_val>>8),~checksum};
  uart_t(packet_2);
}



//main-section
int main(){

  UART_SET();//init
  //pos_goal(750);

  while(1){
     pos_goal(512);
     _delay_ms(1000);
     pos_goal(750
     );
  }

  return 0;
}