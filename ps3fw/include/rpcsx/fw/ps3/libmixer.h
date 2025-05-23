#pragma once

#include "Emu/Memory/vm_ptr.h"

// Error Codes
enum CellLibmixerError : u32
{
	CELL_LIBMIXER_ERROR_NOT_INITIALIZED = 0x80310002,
	CELL_LIBMIXER_ERROR_INVALID_PARAMATER = 0x80310003,
	CELL_LIBMIXER_ERROR_NO_MEMORY = 0x80310005,
	CELL_LIBMIXER_ERROR_ALREADY_EXIST = 0x80310006,
	CELL_LIBMIXER_ERROR_FULL = 0x80310007,
	CELL_LIBMIXER_ERROR_NOT_EXIST = 0x80310008,
	CELL_LIBMIXER_ERROR_TYPE_MISMATCH = 0x80310009,
	CELL_LIBMIXER_ERROR_NOT_FOUND = 0x8031000a,
};

enum
{
	CELL_SURBUS_LEFT = 0,
	CELL_SURBUS_RIGHT = 1,
	CELL_SURBUS_CENTER = 2,
	CELL_SURBUS_LFE = 3,
	CELL_SURBUS_LEFT_SUR = 4,
	CELL_SURBUS_RIGHT_SUR = 5,
	CELL_SURBUS_LEFT_EXT = 6,
	CELL_SURBUS_RIGHT_EXT = 7,
	CELL_SURBUS_REVERB_LEFT = 8,
	CELL_SURBUS_REVERB_RIGHT = 9,
	CELL_SURBUS_REVERB_LEFT_SUR = 10,
	CELL_SURBUS_REVERB_RIGHT_SUR = 11,
};

enum
{
	CELL_SURMIXER_PARAM_TOTALLEVEL_LINEAR = 8,
	CELL_SURMIXER_PARAM_REVERBLEVEL_LINEAR = 9,
	CELL_SURMIXER_PARAM_TOTALMUTE = 12,

	CELL_SURMIXER_PARAM_TOTALLEVEL = 40,  // in dB
	CELL_SURMIXER_PARAM_REVERBLEVEL = 41, // in dB
};

static const float CELL_SURMIXER_CONT_MUTEON = 1.0;
static const float CELL_SURMIXER_CONT_MUTEOFF = 0.0;

enum
{
	CELL_SURMIXER_CONT_DBSWITCHON = 1,
	CELL_SURMIXER_CONT_DBSWITCHOFF = 0,
	CELL_SURMIXER_CONT_PAUSE_OFF = 0,
	CELL_SURMIXER_CONT_PAUSE_ON = 1,
	CELL_SURMIXER_CONT_PAUSE_ON_IMMEDIATE = 2,
};

enum
{
	CELL_SURMIXER_CHSTRIP_TYPE1A = 1,
	CELL_SURMIXER_CHSTRIP_TYPE2A = 2,
	CELL_SURMIXER_CHSTRIP_TYPE6A = 3,
	CELL_SURMIXER_CHSTRIP_TYPE8A = 4,

	CELL_SURMIXER_CH1PARAM_LEVEL = 0,
	CELL_SURMIXER_CH1PARAM_EXPRESSIONLEVEL = 1,
	CELL_SURMIXER_CH1PARAM_CENTERLEVEL = 2,
	CELL_SURMIXER_CH1PARAM_REVERBSENDLEVEL = 3,
	CELL_SURMIXER_CH1PARAM_MUTE = 4,
	CELL_SURMIXER_CH1PARAM_REVSENDPOSITION = 5,
	CELL_SURMIXER_CH1PARAM_POSITION = 6,

	CELL_SURMIXER_CH2PARAM_LEVEL = 0,
	CELL_SURMIXER_CH2PARAM_EXPRESSIONLEVEL = 1,
	CELL_SURMIXER_CH2PARAM_CENTERLEVEL = 2,
	CELL_SURMIXER_CH2PARAM_REVERBSENDLEVEL = 3,
	CELL_SURMIXER_CH2PARAM_MUTE = 4,
	CELL_SURMIXER_CH2PARAM_REVSENDPOSITION = 5,
	CELL_SURMIXER_CH2PARAM_POSITION = 6,

	CELL_SURMIXER_CH6PARAM_LEVEL = 0,
	CELL_SURMIXER_CH6PARAM_EXPRESSIONLEVEL = 1,
	CELL_SURMIXER_CH6PARAM_REVERBSENDLEVEL = 2,
	CELL_SURMIXER_CH6PARAM_CENTER_REVERBSENDLEVEL = 3,
	CELL_SURMIXER_CH6PARAM_MUTE = 4,

	CELL_SURMIXER_CH8PARAM_LEVEL = 0,
	CELL_SURMIXER_CH8PARAM_EXPRESSIONLEVEL = 1,
	CELL_SURMIXER_CH8PARAM_REVERBSENDLEVEL = 2,
	CELL_SURMIXER_CH8PARAM_CENTER_REVERBSENDLEVEL = 3,
	CELL_SURMIXER_CH8PARAM_MUTE = 4,

	CELL_SURMIXER_CH1CONT_MUTE_OFF = 0,
	CELL_SURMIXER_CH1CONT_MUTE_ON = 1,
	CELL_SURMIXER_CH2CONT_MUTE_OFF = 0,
	CELL_SURMIXER_CH2CONT_MUTE_ON = 1,
	CELL_SURMIXER_CH6CONT_MUTE_OFF = 0,
	CELL_SURMIXER_CH6CONT_MUTE_ON = 1,
	CELL_SURMIXER_CH8CONT_MUTE_OFF = 0,
	CELL_SURMIXER_CH8CONT_MUTE_ON = 1,
};

enum
{
	CELL_SSPLAYER_ONESHOT = 0,
	CELL_SSPLAYER_ONESHOT_CONT = 2,
	CELL_SSPLAYER_LOOP_ON = 16,

	CELL_SSPLAYER_STATE_ERROR = 0xffffffff,
	CELL_SSPLAYER_STATE_NOTREADY = 0x88888888,
	CELL_SSPLAYER_STATE_OFF = 0x00,
	CELL_SSPLAYER_STATE_PAUSE = 0x01,
	CELL_SSPLAYER_STATE_CLOSING = 0x08,
	CELL_SSPLAYER_STATE_ON = 0x20,
};

using CellSurMixerNotifyCallbackFunction = s32(vm::ptr<void> arg, u32 counter, u32 samples);

struct CellSSPlayerConfig
{
	be_t<u32> channels;
	be_t<u32> outputMode;
};

struct CellSSPlayerWaveParam
{
	be_t<u32> addr;
	be_t<s32> format;
	be_t<u32> samples;
	be_t<u32> loopStartOffset;
	be_t<u32> startOffset;
};

struct CellSSPlayerCommonParam
{
	be_t<u32> loopMode;
	be_t<u32> attackMode;
};

struct CellSurMixerPosition
{
	be_t<float> x;
	be_t<float> y;
	be_t<float> z;
};

struct CellSSPlayerRuntimeInfo
{
	be_t<float> level;
	be_t<float> speed;
	CellSurMixerPosition position;
};

struct CellSurMixerConfig
{
	be_t<s32> priority;
	be_t<u32> chStrips1;
	be_t<u32> chStrips2;
	be_t<u32> chStrips6;
	be_t<u32> chStrips8;
};

struct CellSurMixerChStripParam
{
	be_t<u32> param;
	be_t<u32> attribute_addr;
	be_t<s32> dBSwitch;
	be_t<float> floatVal;
	be_t<s32> intVal;
};
