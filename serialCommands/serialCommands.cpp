#include "mbed.h"
#include "CommandParser.h"

typedef CommandParser<> MyCommandParser;

#define BUF_LEN 80

uint8_t idx=0;
uint8_t buffer0[BUF_LEN];
uint8_t buffer1[BUF_LEN];
volatile uint8_t NewData = 0;

UnbufferedSerial rser(USBTX, USBRX, MBED_CONF_PLATFORM_STDIO_BAUD_RATE);
MyCommandParser parser;

void serialCb() {
  char ch;
  rser.read(&ch, 1);
  if (ch == 10 || ch == 13) {
    if (idx > 0) {
      NewData = 1;
      memcpy(buffer1, buffer0, idx);
      buffer1[idx++] = 0;
    } else {
      NewData = 0;
    }
    idx = 0;
  } else {
    if (idx < BUF_LEN) {
      buffer0[idx++] = ch;
    }
    NewData = 0;
  }
}

void cmd_test(MyCommandParser::Argument *args, char *response) {
  printf("string: %s\n", args[0].asString);
  printf("double: %f\n", args[1].asDouble);
  printf("int64: %d\n", (int32_t)args[2].asInt64); // NOTE: on older AVR-based boards, Serial doesn't support printing 64-bit values, so we'll cast it down to 32-bit
  printf("uint64: %d\n", (uint32_t)args[3].asUInt64); // NOTE: on older AVR-based boards, Serial doesn't support printing 64-bit values, so we'll cast it down to 32-bit
  strlcpy(response, "success", MyCommandParser::MAX_RESPONSE_SIZE);
}

int main() {
  char response[MyCommandParser::MAX_RESPONSE_SIZE];
  
  printf("Start\n");

  parser.registerCommand("TEST", "sdiu", "example: TEST \"def\" -1.234e5 -123 123\n", &cmd_test);

  rser.attach(&serialCb, SerialBase::RxIrq);

  while(true) {
    while(!NewData) ;
    parser.processCommand((const char*)buffer1, response);
    printf("%s\n", response);
    NewData = 0;
  }
}
