#include "parser/parser.h"

#include <string>
#include <regex>
#include <glog/logging.h>

#include "parser/APIDataTypes.hpp"

bool Parser::IsWellFormed(const std::string message) {
  std::regex r("\\[([A-Z])\\]([[:xdigit:]]*)");
  std::smatch matches;

  bool matched =  std::regex_search(message, matches, r);
  if (matched) {
    U16 command = matches[1].str()[0];
    U8 target_length_bytes = getLengthForCID(command);
    U8 actual_length_hexchars = matches[2].str().length();
    LOG(INFO) << "Hex string: " << matches[2].str();
    if (actual_length_hexchars != target_length_bytes * 2) {
      LOG(WARNING) << "command = " << command << " got " << actual_length_hexchars / 2 << " bytes in hex, expected " << (int)target_length_bytes << " bytes";
    } else {
      return true;
    }
  } else {
    LOG(WARNING) << "Unparsable message: " << message;
  }
  return false;
}
