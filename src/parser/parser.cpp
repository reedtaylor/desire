#include "parser/parser.h"

#include <string>
#include <regex>
#include <glog/logging.h>

#include "parser/APIDataTypes.hpp"

#define DE1_MESSAGE_REGEX "\\[([A-Z])\\]([[:xdigit:]]*)"

bool Parser::IsWellFormed(const std::string message) {
  std::regex r(DE1_MESSAGE_REGEX);
  std::smatch matches;

  bool matched =  std::regex_search(message, matches, r);
  if (matched) {
    U16 command = matches[1].str()[0];
    U8 target_length_bytes = GetLengthForCommand(command);
    U8 actual_length_hexchars = matches[2].str().length();
    if (actual_length_hexchars != target_length_bytes * 2) {
      LOG(WARNING) << "command = " << command << " got " << actual_length_hexchars << " characters in hex, expected " << (int)target_length_bytes*2 << " characters";
    } else {
      return true;
    }
  } else {
    LOG(WARNING) << "Unparsable message: " << message;
  }
  return false;
}

U16 Parser::GetCommand(const std::string message) {
  std::regex r(DE1_MESSAGE_REGEX);
  std::smatch matches;
  
  bool matched =  std::regex_search(message, matches, r);
  if (matched) {
    U16 command = matches[1].str()[0];
    return command;
  } else {
    return (U16)0;
  }
}

std::string Parser::GetHexString(const std::string message) {
  std::regex r(DE1_MESSAGE_REGEX);
  std::smatch matches;
  
  bool matched =  std::regex_search(message, matches, r);
  if (matched) {
    std::string hexString = matches[2].str();
    return hexString;
  } else {
    return std::string("");
  }
}


bool Parser::Unpack(U8 *value, std::string hexString, U16 len) {
  
  for (U16 pos=0; pos<len; pos++) {
    U8 highNybble = CharToNybble((U8)hexString[(pos*2)]);
    U8 lowNybble = CharToNybble((U8)hexString[(pos*2) + 1]);
    U8 byte = (highNybble << 4) + lowNybble;
    value[len - pos - 1] = byte;
    //value[pos] = byte;
  }
  return true;
}
