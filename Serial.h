#ifndef __SERIAL_H__
#define __SERIAL_H__

#define F_CPU 20000000ul / 6
#ifndef BUFF_SIZE
   #define BUFF_SIZE 64
#endif

#include <avr/interrupt.h>
#include <stddef.h>
#include <stdint.h>

/// Frame Configurations
void SERIAL_5N1();
void SERIAL_6N1();
void SERIAL_7N1();
void SERIAL_8N1();  /// default
void SERIAL_5N2();
void SERIAL_6N2();
void SERIAL_7N2();
void SERIAL_8N2();
void SERIAL_5E1();  /// even parity
void SERIAL_6E1();
void SERIAL_7E1();
void SERIAL_8E1();
void SERIAL_5E2();
void SERIAL_6E2();
void SERIAL_7E2();
void SERIAL_8E2();
void SERIAL_5O1();  /// odd parity
void SERIAL_6O1();
void SERIAL_7O1();
void SERIAL_8O1();
void SERIAL_5O2();
void SERIAL_6O2();
void SERIAL_7O2();
void SERIAL_8O2(); 

enum LookaheadMode {
   SKIP_ALL,
   SKIP_NONE,
   SKIP_WHITESPACE
};

extern "C" void USART0_RXC_vect(void) __attribute__ ((signal));

class _Serial
{
   friend void USART0_RXC_vect(void);
private:
   struct RingBuff {
      char buff[BUFF_SIZE];
      char * volatile ri = buff;
      char * volatile wi = buff;
      void write(char c) { *wi = c; wi + 1 > &buff[BUFF_SIZE] ? wi = buff : wi++; };
      char read() { char c = *ri; ri + 1 > &buff[BUFF_SIZE] ? ri = buff : ri++; return c;};
   } rb;
   void putc(char c);
public:
   bool available();
   bool availableForWrite();
   void begin(uint32_t speed, void (*config)() = SERIAL_8N1);
   void end();
   void flush();
   bool find(char target);
   bool find(char * target, size_t length);
   bool findUntil(char * target, char terminal);
   float parseFloat();
   float parseFloat(LookaheadMode lookahead);
   float parseFloat(LookaheadMode lookahead, char ignore);
   int8_t peek();
   int8_t read();
   size_t println(char * str);
   size_t println(char str);
   size_t print(char * str);
   size_t print(char str);
   size_t readBytes(char * buff, size_t length);
   size_t readBytesUntil(char terminator, char * buff, size_t length);
   size_t write(uint8_t val);
   size_t write(char * str);
   size_t write(char * buf, size_t length);
};

extern _Serial Serial;

#endif