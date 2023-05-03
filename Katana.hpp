#pragma once
#include <array>
#include <future>
#include "DeviceMidiPort.hpp"
#include "constants.hpp"

class Katana {
public:
	Katana(unsigned int inputPort = 0, unsigned int outputPort = 1);
	~Katana();

	void setVolume(int value);
	void setChannel(Channel value);
	
	void setBassLevel(int value);
	void setMiddleLevel(int value);
	void setTrebleLevel(int value);

	void setBoosterStatus(bool value);
	void setBoosterLevel(int value);
	void setModStatus(bool value);
	void setModLevel(int value);
	void setModType(ModType value);
	void setFxStatus(bool value);
	void setFxLevel(int value);
	void setFxType(ModType value);
	void setDelayStatus(bool value);
	void setDelayTime(int value);
	void setDelayLevel(int value);
	void setReverbStatus(bool value);
	void setReverbLevel(int value);

	Channel getChannel();
private:
	DeviceMidiPort midi;

	struct ArrayHasher {
		std::size_t operator()(const std::array<unsigned char, 4>& arr) const {
			unsigned int result = 0;
			for (int i = 0; i < 4; ++i) {
				result <<= 8;
				result |= arr[i];
			}
			return std::hash<unsigned int>()(result);
		}
	};
	std::unordered_map<
		std::array<unsigned char, 4>,
		std::promise<std::vector<unsigned char>>,
		ArrayHasher
	> data;

	enum class Action : unsigned char {
		WRITE = 0x12,
		READ = 0x11
	};
	void send(Action action, std::array<unsigned char, 4> address, const std::vector<unsigned char>& value);

	void set(std::array<unsigned char, 4> address, const std::vector<unsigned char>& value);
	std::future<std::vector<unsigned char>> get(std::array<unsigned char, 4> address, std::array<unsigned char, 4> value);
	void handshake();

	void handleMessage(const std::vector<unsigned char>& data);
};
