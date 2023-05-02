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
void SERIAL_5N1(); ///< Configure serial for 5 data bits, no parity, 1 stop bits 
void SERIAL_6N1(); ///< Configure serial for 6 data bits, no parity, 1 stop bits
void SERIAL_7N1(); ///< Configure serial for 7 data bits, no parity, 1 stop bits
void SERIAL_8N1(); ///< Configure serial for 8 data bits, no parity, 1 stop bits (this is the default configuration)
void SERIAL_5N2(); ///< Configure serial for 5 data bits, no parity, 2 stop bits
void SERIAL_6N2(); ///< Configure serial for 6 data bits, no parity, 2 stop bits
void SERIAL_7N2(); ///< Configure serial for 7 data bits, no parity, 2 stop bits
void SERIAL_8N2(); ///< Configure serial for 8 data bits, no parity, 2 stop bits
void SERIAL_5E1(); ///< Configure serial for 5 data bits, even parity, 1 stop bits
void SERIAL_6E1(); ///< Configure serial for 6 data bits, even parity, 1 stop bits
void SERIAL_7E1(); ///< Configure serial for 7 data bits, even parity, 1 stop bits
void SERIAL_8E1(); ///< Configure serial for 8 data bits, even parity, 1 stop bits
void SERIAL_5E2(); ///< Configure serial for 5 data bits, even parity, 2 stop bits
void SERIAL_6E2(); ///< Configure serial for 6 data bits, even parity, 2 stop bits
void SERIAL_7E2(); ///< Configure serial for 7 data bits, even parity, 2 stop bits
void SERIAL_8E2(); ///< Configure serial for 8 data bits, even parity, 2 stop bits
void SERIAL_5O1(); ///< Configure serial for 5 data bits, odd parity, 1 stop bits
void SERIAL_6O1(); ///< Configure serial for 6 data bits, odd parity, 1 stop bits
void SERIAL_7O1(); ///< Configure serial for 7 data bits, odd parity, 1 stop bits
void SERIAL_8O1(); ///< Configure serial for 8 data bits, odd parity, 1 stop bits
void SERIAL_5O2(); ///< Configure serial for 5 data bits, odd parity, 2 stop bits
void SERIAL_6O2(); ///< Configure serial for 6 data bits, odd parity, 2 stop bits
void SERIAL_7O2(); ///< Configure serial for 7 data bits, odd parity, 2 stop bits
void SERIAL_8O2(); ///< Configure serial for 8 data bits, odd parity, 2 stop bits

/// @enum LookaheadMode
/// Lookahead mode for find and float parsing functions
enum LookaheadMode {
   SKIP_ALL,        ///< Skip all characters
   SKIP_NONE,       ///< Try to parse everything
   SKIP_WHITESPACE  ///< Skip whitespace characters only
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