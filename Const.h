#pragma once

#define TRACK_COUNT 4
#define MAX_BARS 8
#define MAX_STEPS_PER_BAR 16

//Every CC parameters are storable, except all before CC_START
#define CC_START 40

#define _allTrackEnum(ParamName) \
ParamName##1, \
ParamName##2, \
ParamName##3, \
ParamName##4

#define _allTrackCase(ParamName) \
case ParamName##1 : \
case ParamName##2 : \
case ParamName##3 : \
case ParamName##4 

enum Param {
  channelSelect,
  _allTrackEnum(barCount),
  _allTrackEnum(stepCount),
  _allTrackEnum(arpState),

  count
};

enum CCList {
  Record = 10,
  ClearAll,
  _allTrackEnum(ClearTrack)
};
