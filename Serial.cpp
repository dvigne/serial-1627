#include "Serial.h"
#include <string.h>
#include <avr/io.h>
#include <util/atomic.h>

_Serial Serial;

void SERIAL_5N1() { USART0.CTRLC = 0x00; }
void SERIAL_6N1() { USART0.CTRLC = 0x01; }
void SERIAL_7N1() { USART0.CTRLC = 0x02; }
void SERIAL_8N1() { USART0.CTRLC = 0x03; }  /// default
void SERIAL_5N2() { USART0.CTRLC = 0X08; }
void SERIAL_6N2() { USART0.CTRLC = 0X09; }
void SERIAL_7N2() { USART0.CTRLC = 0x0A; }
void SERIAL_8N2() { USART0.CTRLC = 0x0B; }
void SERIAL_5E1() { USART0.CTRLC = 0x20; }  /// even parity
void SERIAL_6E1() { USART0.CTRLC = 0x21; }
void SERIAL_7E1() { USART0.CTRLC = 0x22; }
void SERIAL_8E1() { USART0.CTRLC = 0x23; }
void SERIAL_5E2() { USART0.CTRLC = 0x28; }
void SERIAL_6E2() { USART0.CTRLC = 0x29; }
void SERIAL_7E2() { USART0.CTRLC = 0x2A; }
void SERIAL_8E2() { USART0.CTRLC = 0x2B; }
void SERIAL_5O1() { USART0.CTRLC = 0x30; }  /// odd parity
void SERIAL_6O1() { USART0.CTRLC = 0x31; }
void SERIAL_7O1() { USART0.CTRLC = 0x32; }
void SERIAL_8O1() { USART0.CTRLC = 0x33; }
void SERIAL_5O2() { USART0.CTRLC = 0x38; }
void SERIAL_6O2() { USART0.CTRLC = 0x39; }
void SERIAL_7O2() { USART0.CTRLC = 0x3A; }
void SERIAL_8O2() { USART0.CTRLC = 0x3B; }

/// @brief Checks if serial data is available in the buffer
/// @return true if data is available
bool _Serial::available() {
   return (rb.ri != rb.wi);
}

/// @brief Checks if serial lines are available for transmit
/// @return true if ready for transmission
bool _Serial::availableForWrite() {
   return USART0.STATUS & USART_TXCIF_bm;
}

/// @brief Initialize USART interface
/// @param speed Set baud rate
/// @param config  Set frame configuration
void _Serial::begin(uint32_t speed, void (*config)()) {
   // Calculate baid rate and load into USART BAUD register
   USART0.BAUD = (F_CPU * 64.0) / (speed * 16.0);

   // Enable USART transmitter and receiver
   USART0.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;

   // Enable USART even when CPU is halted in break debug mode
   USART0.DBGCTRL |= USART_DBGRUN_bm;

   // Enable interrupts on RX events
   USART0.CTRLA |= USART_RXCIE_bm;

   // Enable global interrupts in the CPU
   sei();

   /// Call configuration subroutine
   config();

   // Configure Output Pins
   PORTB.DIRSET |= PIN2_bm;
}

/// @brief Disable USART transmitter and receiver and release pins
void _Serial::end() {
   // Disable USART transmitter and receiver
   USART0.CTRLB ^= USART_TXEN_bm | USART_RXEN_bm;

   // Disable USART debug control
   USART0.DBGCTRL ^= USART_DBGRUN_bm;

   // Release output pins and return to high-impedance state
   PORTB.DIRSET ^= PIN2_bm;

   // Disable interrupts on RX events
   USART0.CTRLA ^= USART_RXCIE_bm;

   // Disable global interrupts
   cli();

   // Release output pins
   PORTB.DIRSET ^= PIN2_bm;
}

/// @brief Clear the serial buffer
void _Serial::flush() {
   rb.ri = rb.wi;
}

/// @brief Reads data from the stream until the target character is found
/// @param target Charater to look for
/// @return true if found
/// @note This function is missing the timeout functionality and is therefore
/// not 1:1 with the original Arduino AVR core library. Proceed with caution.
bool _Serial::find(char target) {
   do
   {
      read();
   } while (peek() != target);
   return true;
}

/// @brief Reads datsa from stream uyntil target character is found
/// @param target Array of characters to look for
/// @param length Length of array
/// @return True if found
/// @note This function is missing the timeout functionality and is therefore
/// not 1:1 with the original Arduino AVR core library. Proceed with caution.
bool _Serial::find(char * target, size_t length) {
   // TODO
}

/// @brief Reads data from serial buffer until either target or termianl is found
/// @param target The char to search for
/// @param terminal The terminating character if found
/// @return True if the target tring is found before terminal
/// @note This function is missing the timeout functionality and is therefore
/// not 1:1 with the original Arduino AVR core library. Proceed with caution.
bool _Serial::findUntil(char * target, char terminal) {
   // TODO
}

/// @brief Returns the first valid floating point number from serial buffer
/// @return Floating point number
/// @note This function is missing the timeout functionality and is therefore
/// not 1:1 with the original Arduino AVR core library. Proceed with caution.
float _Serial::parseFloat() {
   // TODO
}

/// @brief Returns the first valid floating point number from serial buffer
/// Essentially the same as _Serial::parseFloat(), except it searches using the
/// strategy defined by `lookahead`
/// @param lookahead LookaheadMode strategy to use
/// @return Floating point number
float _Serial::parseFloat(LookaheadMode lookahead) {
   // TODO
}

/// @brief Returns the first valid floating point number from serial buffer
/// Essentially the same as _Serial::parseFloat(LookaheadMode lookahead), except it searches using the
/// strategy defined by `lookahead` and will ignore a character defined by `ignore`
/// @param lookahead LookaheadMode strategy to use
/// @param ignore Character to ignore
/// @return Floating point number
float _Serial::parseFloat(LookaheadMode lookahead, char ignore) {
   // TODO
}

/// @brief Read the last byte from the serial buffer without clearing it
/// @return Last byte if availabe, -1 if nothing to return
int8_t _Serial::peek() {
   if(available()) {
      return *rb.ri;
   }
   return -1;
}
/// @brief Print a string to the serial console with a newline terminator
/// @param str String to be printed
size_t _Serial::println(char * str) {
   print(str);
   putc('\n');
   return strlen(str) + 1;
}

/// @brief Print a single character to the serial console with a newline terminator
/// @param str Character to be printed
size_t _Serial::println(char str) {
   print(str);
   putc('\n');
   return 2;
}

/// @brief Print a string to the serial console without a newline terminator
/// @param str String to be printed
size_t _Serial::print(char * str) {
   for(size_t len = 0; len < strlen(str); len++) {
      putc(str[len]);
   }
   return strlen(str);
}
/// @brief Print a string to the serial console without a newline terminator
/// @param str String to be printed
size_t _Serial::print(char str) {
   putc(str);
   return 1;
}

/// @brief Write a single character to the transmission data register
/// @param c Character to write
void _Serial::putc(char c) {
   while(!(USART0.STATUS & USART_DREIF_bm));
   USART0.TXDATAL = c;
}

/// @brief Read 1 byte from the serial buffer and remove it
/// @return Byte value, -1 if nothing to return
int8_t _Serial::read() {
   if(available()) {
      return rb.read();
   }
   return -1;
}

/// @brief Read specified number of bytes from serial buffer
/// @param buff Array to store `length` bytes of data
/// @param length Number of bytes to read
/// @return Bytes successfully read
size_t _Serial::readBytes(char * buff, size_t length) {
   for (size_t i = 0; i < length; i++)
   {
      if(available()) {
         buff[i] = rb.read();
      }
      else {
         return i;
      }
   }
}
/// @brief Transmit raw binary value
/// @param val 
/// @return Transmitted byte count
size_t _Serial::write(uint8_t val) {
   putc(val);
   return 1;
}

/// @brief Transmit an array of binary data, really just an alias of print
size_t _Serial::write(char * str) {
   return print(str);
}

/// @brief Transmit arbitrary binary data
/// @param buf Buffer of dta to be transmitted
/// @param length Length of buffer
/// @return Transmitted bytes
size_t _Serial::write(char * buf, size_t length) {
   for (size_t i = 0; i < length; i++)
   {
      putc(buf[length]);
   }
   return length;
}


/// @brief Interrupt Service Routine for handling RX events
ISR(USART0_RXC_vect) {
   Serial.rb.write(USART0.RXDATAL);
}