/*
 * APIDataTypes.hpp
 *
 *  Created on: Aug 26, 2016
 *      Author: ray
 */

#ifndef APIDATATYPES_HPP_
#define APIDATATYPES_HPP_

//#include "parser/types.hpp"
#include <cstdint>

typedef uint8_t U8 ;
typedef int8_t S8;

typedef uint16_t U16;
typedef int16_t S16;

typedef uint32_t U32;
typedef int32_t S32;

typedef uint64_t U64;
typedef int64_t S64;

#define PACKEDATTR __attribute__((__packed__))
/**
 * This is a 16 bit struct that we store an 8.8 fractional number within.
 * We wrap this simple 16 bit type in a struct to defeat C's self-defeating
 * lack of type checking between typedefs of first class types. No struct =
 * no type checking.
 */
struct PACKEDATTR U16P8 {
  U16 value;
};

struct PACKEDATTR S16P8 {
  S16 value;
};

struct PACKEDATTR U16P0 {
  U16 value;
};

struct PACKEDATTR U16P12 {
  U16 value;
};

struct PACKEDATTR U10P0 {
  U16 value;
};

struct PACKEDATTR U8P4 {
  U8 value;
};

struct PACKEDATTR U24P0 {
  U8 Hi;
  U8 Mid;
  U8 Lo;
};

struct PACKEDATTR U24P16 {
  U8 Hi;
  U8 Mid;
  U8 Lo;
};

struct PACKEDATTR U32P0 {
  U32 value;
};

struct PACKEDATTR S32P16 {
  S32 value;
};


struct PACKEDATTR U8P0 {
  U8 value;
};

struct PACKEDATTR F8_1_7 {
  // Most sigbificant bit: 0 - Exponent is 1/10
  //                       1 - Exponent is 1
  // Remainder: mantissa
  // So we can represent 0 - 12.7 in tenths of a second, and 0-127 seconds
  U8 value;
};

struct PACKEDATTR F32_24_8 {
  S32 value;
};

struct PACKEDATTR U8P1 {
  U8 value;
};

enum T_Enum_Models : U32 {
  Model_Plus  = 1,
  Model_Pro   = 2,
  Model_CAFE  = 4,
};

enum T_Enum_Config : U32 {
  LowPowerHeater = 0x1,    // Heaters are 1500W (as against 1900W 120V heater)
  CFG_Refill     = 0x2,    // We can refill
  Voltage_120V   = 0x4,    // We are a 120V machine
};

struct PACKEDATTR T_FWVersion {
  U8P0   APIVersion;   // Incremented if we make a change that breaks things
  F8_1_7 Release;      // 8-bit Float giving release tag for this firmware
  U16P0  Commits;      // The number of commits in git since repo started for this version.
  U8P0   Changes;      // The number of commits since the tag shown above
  U8P0   BLESha[4];    // First 7 nybbles of sha hash of last commit, followed by flag nybble in least
                       // significant nybble. Bit 0 (LSB) of nybble = dirty bit
};

struct PACKEDATTR T_Versions {
  T_FWVersion BLEVersion; // Version data for BLE module
  T_FWVersion LVVersion;  // Version data from LV module
};

enum T_Enum_BoardVersion : U8 {
  PRE_H = 0, // Board rev prior to rev H
  H     = 1  // Board rev H.
};

struct PACKEDATTR T_Temperatures {
  struct PACKEDATTR T_Current {
    // 8.8 FixedPoint representation
    // Range is 0 - 255 deg C, with 256 steps between each degree
    U16P8 WaterHeater;
    U16P8 SteamHeater;
    U16P8 GroupHeater;
    U16P8 ColdWater;   // The temperature of the water at the cold pump
  } Current;
  struct PACKEDATTR T_Target {
    U16P8 WaterHeater;
    U16P8 SteamHeater;
    U16P8 GroupHeater;
    U16P8 ColdWater;   // The temperature of the water at the cold pump
  } Target;
};

enum T_Enum_SteamSetting : U8 {
  FastStart = 0x80,  // Start the steam quickly and at higher pressure
  SlowStart = 0x00,  // Start the steam slowly and at lower pressure (ie. No Bit set)
  HighPower = 0x40,  // Run the steam at higher pressure
  LowPower  = 0x00,  // Run the steam at lower pressure
};

struct PACKEDATTR T_ShotSettings {
  // Settings for steam and hot water
  T_Enum_SteamSetting SteamSettings; // Defines the steam shot
  U8P0  TargetSteamTemp; // Valid range is 140 - 160
  U8P0  TargetSteamLength;  // Length in seconds of steam
  U8P0  TargetHotWaterTemp; // Temperature of the mixed hot water
  U8P0  TargetHotWaterVol;     // How much water we'll need for hot water (so we know if we have enough)
  U8P0  TargetHotWaterLength;  // (DE1 only) Length of time for a shot (water vol is ignored)
  U8P0  TargetEspressoVol;     // So we know if we have enough water
  U16P8 TargetGroupTemp;       // So we know what to set the group to
};

enum T_Enum_API_MachineStates : U8 {
  // These are reported states guaranteed by the API. The firmware will lie like a
  // cheap watch to maintain the illusion of these states.
  Sleep,      // 0 Everything is off
  GoingToSleep, // 1
  Idle,       // 2 Heaters are controlled, tank water will be heated if required.
  Busy,       // 3 Firmware is doing something you can't interrupt (eg. cooling down water heater after a shot, calibrating sensors on startup).
  Espresso,   // 4 Making espresso
  Steam,      // 5 Making steam
  HotWater,   // 6 Making hot water
  ShortCal,   // 7 Running a short calibration
  SelfTest,   // 8 Checking as much as possible within the firmware. Probably only used during manufacture or repair.
  LongCal,    // 9 Long and involved calibration, possibly involving user interaction. (See substates below, for cases like that).
  Descale,    // A Descale the whole bang-tooty
  FatalError, // B Something has gone horribly wrong
  Init,       // C Machine has not been run yet
  NoRequest,  // D State for T_RequestedState. Means nothing is specifically requested
  SkipToNext, // E In Espresso, skip to next frame. Others, go to Idle if possible
  HotWaterRinse, // F Produce hot water at whatever temperature is available
  SteamRinse,    // 10 Produce a blast of steam
  Refill,        // 11 Attempting, or needs, a refill.
  Clean,         // 12 Clean group head
  InBootLoader,  // 13 The main firmware has not run for some reason. Bootloader is active.
  AirPurge,      // 14 Air purge.
  SchedIdle      // 15 Scheduled wake up idle state
};

enum T_Enum_API_Substates : U8 {
  NoState,          // 0 State is not relevant
  HeatWaterTank,    // 1 Cold water is not hot enough. Heating hot water tank.
  HeatWaterHeater,  // 2 Warm up hot water heater for shot.
  StabilizeMixTemp, // 3 Stabilize mix temp and get entire water path up to temperature.
  PreInfuse,        // 4 Espresso only. Hot Water and Steam will skip this state.
  Pour,             // 5 Not used in Steam
  Flush,            // 6 Espresso only, atm
  Steaming,         // 7 Steam only
  DescaleInit,      // Starting descale
  DescaleFillGroup, // get some descaling solution into the group and let it sit
  DescaleReturn,    // descaling internals
  DescaleGroup,     // descaling group
  DescaleSteam,     // descaling steam
  CleanInit,        // Starting clean
  CleanFillGroup,   // Fill the group
  CleanSoak,        // Wait for 60 seconds so we soak the group head
  CleanGroup,       // Flush through group
  PausedRefill,     // Have we given up on a refill
  PausedSteam,      // Are we paused in steam?

  // Error states
  Error_NaN=200,        // Something died with a NaN
  Error_Inf=201,        // Something died with an Inf
  Error_Generic=202,    // An error for which we have no more specific description
  Error_ACC=203,        // ACC not responding, unlocked, or incorrectly programmed
  Error_TSensor=204,    // We are getting an error that is probably a broken temperature sensor
  Error_PSensor=205,    // Pressure sensor error
  Error_WLevel=206,     // Water level sensor error
  Error_DIP=207,        // DIP switches told us to wait in the error state.
  Error_Assertion=208,  // Assertion failed
  Error_Unsafe=209,     // Unsafe value assigned to variable
  Error_InvalidParm=210,// Invalid parameter passed to function
  Error_Flash=211,      // Error accessing external flash
  Error_OOM=212,        // Could not allocate memory
  Error_Deadline=213    // Realtime deadline missed
};

struct PACKEDATTR T_SubState {
  // A valid substate for any T_Enum_API_MachineStates that have reportable sub states
  // Regardless of the major state, a zero here means there are no substates. Any valid states will start at 1
  T_Enum_API_Substates SubState;
};

struct PACKEDATTR T_StateInfo {
  T_Enum_API_MachineStates State;
  T_Enum_API_Substates SubState;
};

/**
 * This is the state requested by the external Bluetooth stack. The firmware
 * will immediately start working towards this state.
 *
 * Readable and writeable.
 *
 * Read T_MajorState to find out what the current state is.
 */
struct PACKEDATTR T_RequestedState {
  T_Enum_API_MachineStates RequestedState;
};

struct PACKEDATTR T_WaterLevels {
  U16P8 Level;          // Writes to Level will be ignored
  U16P8 StartFillLevel; // Writes to this set the water level refill trigger point.
                        // Invalid writes will be clipped to valid levels
                        // To disable refill, set StartFillLevel to 0.0
};

struct PACKEDATTR T_AllSensors {
  // The whole bang tooty
  U16P8 ColdWater;
  U16P8 HotWater;
  U16P8 MixedWater;
  U16P8 EspressoWater;
  U16P8 InCaseAmbient;
  U16P8 HeatSink;
  U16P8 SteamHeater;
  U16P8 WaterHeater;
  // There may be a few others here in time...
};

struct PACKEDATTR T_StoredShots {
  U16 NumberOfStoredShots;
};

enum T_E_TotalVOrWFlags {
  UseVolume = 0x0000,
  UseWeight = 0x8000
};

struct T_TotalVOrW {
  // Store a value between 0 and 1023 in there, ORed with T_E_TotalVOrWFlags
  // The top 6 bits are reserved for flags. The bottom ten are either a weight
  // or volume
  U10P0 Total;
};

enum T_E_FrameFlags : U8 {
  // Don't use C BitFields here because they are affected by endianness of the machine

  // FrameFlag of zero and pressure of 0 means end of shot, unless we are at the tenth frame, in which case it's the end of shot no matter what
  CtrlF       = 0x01, // Are we in Pressure or Flow priority mode?
  DoCompare   = 0x02, // Do a compare, early exit current frame if compare true
  DC_GT       = 0x04, // If we are doing a compare, then 0 = less than, 1 = greater than
  DC_CompF    = 0x08, // Compare Pressure or Flow?
  TMixTemp    = 0x10, // Disable shower head temperature compensation. Target Mix Temp instead.
  Interpolate = 0x20, // Hard jump to target value, or ramp?
  IgnoreLimit = 0x40, // Ignore minimum pressure and max flow settings

  DontInterpolate = 0, // Don't interpolate, just go to or hold target value
  CtrlP = 0,
  DC_CompP = 0,
  DC_LT = 0,
  TBasketTemp = 0       // Target the basket temp, not the mix temp
};

struct PACKEDATTR T_ShotFrame {
  U8P0   Flag;       // See T_E_FrameFlags
  U8P4   SetVal;     // SetVal is a 4.4 fixed point number, setting either pressure or flow rate, as per mode
  U8P1   Temp;       // Temperature in 0.5 C steps from 0 - 127.5
  F8_1_7 FrameLen;   // FrameLen is the length of this frame in seconds. It's a 1/7 bit floating point number as described in the F8_1_7 a struct
  U8P4   TriggerVal; // Trigger value. Could be a flow or pressure.
  U10P0  MaxVol;     // Exit current frame if the volume/weight exceeds this value. 0 means ignore
};

struct PACKEDATTR T_ShotDescHeader {
  U8P0 HeaderV;           // Set to 1 for this type of shot description
  U8P0 NumberOfFrames;    // Total number of frames.
  U8P0 NumberOfPreinfuseFrames; // Number of frames that are preinfusion
  U8P4 MinimumPressure;   // In flow priority modes, this is the minimum pressure we'll allow
  U8P4 MaximumFlow;       // In pressure priority modes, this is the maximum flow rate we'll allow
};

struct PACKEDATTR T_Deprecated {
  U8P0 Deprecated[20];
};

struct PACKEDATTR T_ShotDesc {
  T_ShotDescHeader Header;
  T_ShotFrame      Frames[20];
};

struct PACKEDATTR T_HeaderWrite {
  T_ShotDescHeader Header;
};

struct PACKEDATTR T_FrameWrite {
  U8P0        FrameToWrite;
  T_ShotFrame Frame;
};

struct PACKEDATTR T_ShotState {
  // Instant snapshot during a shot
  U16P12 GroupPressure; // Pressure at group
  U16P12 GroupFlow;     // Estimated Flow at group
  U16P8  MixTemp;       // Water Temperature entering group
  U24P16 HeadTemp;      // Temperature of water at showerhead
  U16P8  SetMixTemp;    // Set temperature. 0 if no target.
  U16P8  SetHeadTemp;   // Set shower head temp. 0 if no target.
  U8P4   SetGroupPressure; // Set pressure. 0 if not set.
  U8P4   SetGroupFlow;  // Set flow. 0 if not set.
  U8P0   FrameNumber;   // Frame we are currently in.
  U8P0   SteamTemp;     // Steam metal temp
};

struct PACKEDATTR T_ShotSample {
  U16 SampleTime;   // Time since start of shot, in halfcycles
  T_ShotState State;
};

struct PACKEDATTR T_ShotData {
  U64 TimeStamp;
  U32 ShotCount;  // This is the nth shot ever made on this machine
  U16 SampleCount;
  U8P0 StructVersion; // The version of this structure
  T_ShotDesc Description; // What the shot was intended to do
  T_ShotSample Samples[1000]; //Variable length, but maximum length is 1000 samples
};


struct PACKEDATTR T_ShotDirectory {
  U32 StoredShots; // The number of shots that we have stored
  U32 TotalShots;  // Total shots ever
  U64 OldestShot;  // Timestamp of oldest shot
  U64 NewestShot;  // Timestamp of newest shot
};

struct PACKEDATTR T_FWImageInfo {
  U32 Version; // Version of the image. 0 means no image
  U64 Hash;    // Used in case we accidentally release two images with the same version
};

struct PACKEDATTR T_FirmwareImages {
  U8P0 NumberOfImages;
  U8P0 CurrentImageForBoot; // Image to load on boot
  T_FWImageInfo Images[4];
};

struct PACKEDATTR T_MoveMMRWindow {
  U32 Offset;
  U8P0 Len; // Valid values are from 1 to 16
};

/**
 * In order to bypass all the BLE restrictions, I'll mostly be adding new things using the
 * Memory Mapped Region interface.
 *
 * The idea is that everything is mapped into a memory map. Clients will read the region
 * as necessary and work from that.
 *
 * To read a MMR write the address you want to read to the Address field, then wait for a
 * notify with the data you asked for, tagged by address.
 */
struct PACKEDATTR T_ReadFromMMR {
  U8P0  Len;       // Length of data to read, in words-1. ie. 0 = 4 bytes, 1 = 8 bytes, 255 = 2014 bytes, etc.
  U24P0 Address;   // Address of window. Will autoincrement if set up in MapRequest
  U8P0  Data[16];  // If data reaches past the end of a region, bytes will be zero filled
};

struct PACKEDATTR T_WriteToMMR {
  U8P0  Len;       // Length of data
  U24P0 Address;   // Address within the MMR
  U8P0  Data[16];  // Data, zero padded
};

struct PACKEDATTR T_ShotMapRequest {
  // Request that a shot be put in the memory mapped region for later reading
  U16P0 WindowIncrement; // Every time the MMR is read, add this byte offset to the MMR base address
  U16P0 ShotToMap;
};

struct PACKEDATTR T_FWMapRequest {
  // Request that a firmware image be put in the memory mapped region for later reading
  U16P0 WindowIncrement; // Every time the MMR is read, add this byte offset to the MMR base address
  U8P0  FWToErase;        // If this field is non-zero, erase the firmware slot in question. (1 or 2)
                          // Stays non-zero until firmware is erased
  U8P0  FWToMap;          // Either 1 or 2

  // How to use FirstError:
  // Enable Notify for this characteristic
  // Write 0xFFFFFF to FirstError, FWToMap should be set to the required image. FWErase should not be set.
  // A notify will later arrive with the first address that needs repairing.
  // Write 0xFFFFFE in order to update FirstError to the next block that needs repairing.
  // 0xFFFFFF resets the search address to 0, 0xFFFFFE does not.
  // If there are no remaining errors, the notified FirstError value will be 0xFFFFFD
  U24P0 FirstError;
};

struct PACKEDATTR T_DeleteShotRange {
  U32 FirstShot;
  U32 LastShot; // Deletes shots in the range, inclusive of the first and last values given
};

struct PACKEDATTR T_SetTime {
  U64 TimeStamp;
};

struct PACKEDATTR T_TestReq {
  U8P0 TestToReq; // Request a change to a characteristic from MemMap.def. 0 = First characteristic
};

enum T_E_CalTargets : U8 {
  CalFlow = 0,     // Flow, ratiometric, units don't matter.
  CalPressure = 1, // Pressure, ratiometric, units don't matter.
  CalTemp = 2,     // Temperature, offset, units matter. Use Celcius.
  CalError = 255   // Used to tell the app that it did something wrong.
};

enum T_E_CalCommand : U8 {
  Read  = 0,  // Read a calibration value
  Write = 1,  // Write a calibration value
  Reset = 2,  // Reset a calibration value to factory setting
  ReadFactory = 3  // Read the factory value of a calibration value
};

struct PACKEDATTR T_Calibration {
  U32P0 WriteKey;   // Set this to 0xCAFEFOOD or your write will be ignored
  U8P0  CalCommand; // See T_E_CalCommand
  U8P0  CalTarget;  // See T_E_CaTargets

  S32P16 DE1ReportedVal; // The value reported by the DE1
  S32P16 MeasuredVal;    // The external calibrated value we measured

  // To read values:
  //   Enable NOTIFY on this characteristic
  //   Write zeros to all fields except the CalTarget
  //   Wait for a notify with the information you have requested.
  //   Ratiometric values will have 1.0 in the "DE1ReportedVal" field, and the measured value will be relative to that.
  //   Offset values will have 0.0 in the reported value, and the measured value will be relative to that.

  // To read factory values:
  //   Do the same as for a read, but use ReadFactory as the CalCommand

  // To write values:
  //   Set WriteKey to 0xCAFEF00D
  //   Set CalCommand to "Write"
  //   Set CalTarget to the target you want to change
  //   Set DE1Reported value to the value reported by the DE1
  //   Set Measured value to the value you measured
  //   A notify will be sent to show the results of the attempted write, if notifies are enabled

  // To reset values:
  //   Set WriteKey to 0xCAFEF00D
  //   Set CalCommand to "Reset"
  //   Set CalTarget to the target you want to change
  //   Leave everything else set to zero
  //   A notify will be sent to show the results of the attempted reset, if notifies are enabled
};


#endif /* APIDATATYPES_HPP_ */
