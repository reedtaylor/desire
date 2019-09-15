#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>

#include "parser/APIDataTypes.hpp"

class Parser {
public:
  // Check that the message has an expected format
  // and that the data represented as a hex string has the correct length
  // for the command type
  bool IsWellFormed(const std::string message);

  // extract the command type from the message (as a single uppercase char)
  //const char GetCommand(std::string message);

  // unpack the hex string into a specific struct
  //template <typename T> Unpack(T *value, std::string message);

private:

U8 getLengthForCID(U16 CID) {
  switch (CID) {
    case ((U16) 'A'):
      return sizeof(T_Versions); // R, Versions, See T_Versions
    case ((U16) 'B'):
      return sizeof(T_RequestedState); // RW, RequestedState, See T_RequestedState
    case ((U16) 'C'):
      return sizeof(T_SetTime); // RW, SetTime, Set current time
    case ((U16) 'D'):
      return sizeof(T_ShotDirectory); // R, ShotDirectory, View shot directory
    case ((U16) 'E'):
      return sizeof(T_ReadFromMMR); // RW, ReadFromMMR, Read bytes from data mapped into the memory mapped region.
    case ((U16) 'F'):
      return sizeof(T_WriteToMMR); // W, WriteToMMR, Write bytes to memory mapped region
    case ((U16) 'G'):
      return sizeof(T_ShotMapRequest); // W, ShotMapRequest, Map a shot so that it may be read/written
    case ((U16) 'H'):
      return sizeof(T_DeleteShotRange); // W, DeleteShotRange, Delete all shots in the range given
    case ((U16) 'I'):
      return sizeof(T_FWMapRequest); // W, FWMapRequest, Map a firmware image into MMR. Cannot be done with the boot image
    case ((U16) 'J'):
      return sizeof(T_Temperatures); // R, Temperatures, See T_Temperatures
    case ((U16) 'K'):
      return sizeof(T_ShotSettings); // RW, ShotSettings, See T_ShotSettings
    case ((U16) 'L'):
      return sizeof(T_Deprecated); // RW, Deprecated, Was T_ShotDesc. Now deprecated.
    case ((U16) 'M'):
      return sizeof(T_ShotSample); // R, ShotSample, Use to monitor a running shot. See T_ShotSample
    case ((U16) 'N'):
      return sizeof(T_StateInfo); // R, StateInfo, The current state of the DE1
    case ((U16) 'O'):
      return sizeof(T_HeaderWrite); // RW, HeaderWrite, Use this to change a header in the current shot description
    case ((U16) 'P'):
      return sizeof(T_FrameWrite); // RW, FrameWrite, Use this to change a single frame in the current shot description
    case ((U16) 'Q'):
      return sizeof(T_WaterLevels); // RW, WaterLevels, Use this to adjust and read water level settings
    case ((U16) 'R'):
      return sizeof(T_Calibration); // RW, Calibration, Use this to adjust and read calibration
  default:
    return 0;
  }
};

  
};



#endif

