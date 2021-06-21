#include "mbed.h"
#include "CommandParser.h"

// all of the template arguments below are optional, but it is useful to adjust them to save memory (by lowering the limits) or allow larger inputs (by increasing the limits)
// limit number of commands to at most 5
// limit number of arguments per command to at most 3
// limit length of command names to 10 characters
// limit size of all arguments to 15 bytes (e.g., the argument "\x41\x42\x43" uses 14 characters to represent the string but is actually only 3 bytes, 0x41, 0x42, and 0x43)
// limit size of response strings to 64 bytes
typedef CommandParser<5, 3, 10, 15, 64> MyCommandParser;

MyCommandParser parser;

int positionX = 0, positionY = 0;

void cmd_move(MyCommandParser::Argument *args, char *response) {
  positionX = args[0].asInt64;
  positionY = args[1].asInt64;
  printf("MOVING %d %d\n", positionX, positionY);
  snprintf(response, MyCommandParser::MAX_RESPONSE_SIZE, "moved to %d, %d", positionX, positionY);
}

void cmd_jump(MyCommandParser::Argument *args, char *response) {
  printf("JUMPING!\n");
  snprintf(response, MyCommandParser::MAX_RESPONSE_SIZE, "jumped at %d, %d", positionX, positionY);
}

void cmd_say(MyCommandParser::Argument *args, char *response) {
  printf("SAYING %s\n", args[0].asString);
  snprintf(response, MyCommandParser::MAX_RESPONSE_SIZE, "said %s at %d, %d", args[0].asString, positionX, positionY);
}

int main() {
  char response[MyCommandParser::MAX_RESPONSE_SIZE];
  
  printf("Start\n");

  if(!parser.registerCommand("move", "ii", &cmd_move)) // two int64_t arguments
    printf("error registering 'move'\n");
  if(!parser.registerCommand("jump", "", &cmd_jump)) // no arguments
    printf("error registering 'jump'\n");
  if(!parser.registerCommand("say", "s", &cmd_say)) // one string argument
    printf("error registering 'say'\n");

  printf("let's try a simple example...\n");
  parser.processCommand("move 45 -23", response);
  printf("%s\n",response);
  parser.processCommand("jump", response);
  printf("%s\n",response);
  parser.processCommand("say \"Hello, \\\"world!\\\"\"", response);
  printf("%s\n",response);
  parser.processCommand("say abc", response);
  printf("%s\n",response);

  printf("now let's try some invalid inputs...\n");
  parser.processCommand("invalid", response); // bad command
  printf("%s\n",response);
  parser.processCommand("move", response); // missing args
  printf("%s\n",response);
  parser.processCommand("jump 123", response); // extra args
  printf("%s\n",response);
  parser.processCommand("move 123abc 456", response); // invalid int64 argument
  printf("%s\n",response);
  parser.processCommand("say \"\\x\"", response); // invalid string argument
  printf("%s\n",response);
  parser.processCommand("say \"\\x5z\"", response); // invalid string escape
  printf("%s\n",response);
  parser.processCommand("say \"abc", response); // missing ending quote
  printf("%s\n",response);
  
  while(true) ;
}
