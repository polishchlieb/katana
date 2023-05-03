#include "DeviceMidiPort.hpp"
#include <iostream>

DeviceMidiPort::DeviceMidiPort(unsigned int inPortNum, unsigned int outPortNum) {
	inPort.openPort(inPortNum);
	inPort.ignoreTypes(false, false, false);
	outPort.openPort(outPortNum);
}

void DeviceMidiPort::send(const std::vector<unsigned char>& data) {
	outPort.sendMessage(&data);
}

void DeviceMidiPort::join() {
	inListener.join();
}

void DeviceMidiPort::setCallback(const Callback& dataCallback) {
	inListener = std::thread{ [this, dataCallback]() {
		std::vector<unsigned char> data;

		while (true) {
			inPort.getMessage(&data);
			if (data.size() != 0)
				dataCallback(data);

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	} };
}
