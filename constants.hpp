#pragma once

constexpr unsigned char SYSEX_START = 0xf0;
constexpr unsigned char KATANA_HEADER[] = { 0x41, 0x0, 0x0, 0x0, 0x0, 0x33 };
constexpr unsigned char WRITE = 0x12;
constexpr unsigned char SYSEX_END = 0xf7;

enum class Channel : unsigned char {
	A1 = 0x1,
	A2 = 0x2,
	B1 = 0x5,
	B2 = 0x6,
	PANEL = 0x0
};

enum class ModType : unsigned char {
	CHORUS = 0x1d,
	FLANGER = 0x14,
	PHASER = 0x13,
	UNI_V = 0x17,
	TREMOLO = 0x15,
	VIBRATO = 0x1a,
	ROTARY = 0x16,
	RING_MOD = 0x1b,
	SLOW_GEAR = 0xa,
	SLICER = 0x19,
	COMP = 0x3,
	LIMITER = 0x4,
	T_WAH = 0x0,
	AUTO_WAH = 0x1,
	PEDAL_WAH = 0x2,
	GRAPHIC_EQ = 0x6,
	PARAMETRIC_EQ = 0x7,
	GUITAR_SIM = 0x9,
	AC_GUITAR_SIM = 0x1f,
	AC_PROCESSOR = 0x12,
	WAVE_SYNTH = 0xc,
	OCTAVE = 0xe,
	HEAVY_OCTAVE = 0x27,
	PITCH_SHIFTER = 0xf,
	HARMONIST = 0x10,
	HUMANIZER = 0x1c,
	PHASER_90E = 0x23,
	FLANGER_117E = 0x24,
	WAH_95E = 0x25,
	DC_30 = 0x26,
	PEDAL_BEND = 0x28
};