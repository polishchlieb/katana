#pragma once
#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <RtMidi.h>

class DeviceMidiPort {
public:
	typedef std::function<void(const std::vector<unsigned char>& data)> Callback;

	DeviceMidiPort(unsigned int inPort, unsigned int outPort);

	void send(const std::vector<unsigned char>& data);
	void join();
	void setCallback(const Callback& dataCallback);
private:
	std::thread inListener;
	RtMidiIn inPort;
	RtMidiOut outPort;
};