#include "Katana.hpp"
#include <format>
#include "addresses.hpp"

std::string toHex(int value) {
    return std::format("{:x}", value);
}

Katana::Katana(unsigned int inputPort, unsigned int outputPort)
    : midi{ inputPort, outputPort }
{
    using namespace std::placeholders;
    midi.setCallback(std::bind(&Katana::handleMessage, this, _1));

    handshake();
}

Katana::~Katana() {
    midi.join();
}

void Katana::setVolume(int value) {
    set(VOLUME_ADDRESS, { (unsigned char)value });
}

void Katana::setChannel(Channel value) {
    set(CHANNEL_ADDRESS, { 0x0, (unsigned char)value });
}

void Katana::setBassLevel(int value) {
    set(BASS_LEVEL_ADDRESS, { (unsigned char)value });
}

void Katana::setMiddleLevel(int value) {
    set(MIDDLE_LEVEL_ADDRESS, { (unsigned char)value });
}

void Katana::setTrebleLevel(int value) {
    set(TREBLE_LEVEL_ADDRESS, { (unsigned char)value });
}

void Katana::setBoosterStatus(bool value) {
    set(BOOSTER_STATUS_ADDRESS, { (unsigned char)value });
}

void Katana::setBoosterLevel(int value) {
    set(BOOSTER_LEVEL_ADDRESS, { (unsigned char)value });
}

void Katana::setModStatus(bool value) {
    set(MOD_STATUS_ADDRESS, { (unsigned char)value });
}

void Katana::setModLevel(int value) {
    set(MOD_LEVEL_ADDRESS, { (unsigned char)value });
}

void Katana::setModType(ModType value) {
    set(MOD_TYPE_ADDRESS, { (unsigned char)value });
}

void Katana::setFxStatus(bool value) {
    set(FX_STATUS_ADDRESS, { (unsigned char)value });
}

void Katana::setFxLevel(int value) {
    set(FX_LEVEL_ADDRESS, { (unsigned char)value });
}

void Katana::setFxType(ModType value) {
    set(FX_TYPE_ADDRESS, { (unsigned char)value });
}

void Katana::setDelayStatus(bool value) {
    set(DELAY_STATUS_ADDRESS, { (unsigned char)value });
}

void Katana::setDelayTime(int value) {
    set(DELAY_TIME_ADDRESS, { (unsigned char) (value / 128), (unsigned char) (value % 128) });
}

void Katana::setDelayLevel(int value) {
    set(DELAY_LEVEL_ADDRESS, { (unsigned char)value });
}

void Katana::setReverbStatus(bool value) {
    set(REVERB_STATUS_ADDRESS, { (unsigned char)value });
}

void Katana::setReverbLevel(int value) {
    set(REVERB_LEVEL_ADDRESS, { (unsigned char)value });
}

Channel Katana::getChannel() {
    auto result = get(CHANNEL_ADDRESS, { 0x0, 0x0, 0x0, 0x2 }).get();
    return (Channel) result[1];
}

void Katana::send(Action action, std::array<unsigned char, 4> address, const std::vector<unsigned char>& value) {
    std::vector<unsigned char> message;

    message.push_back(SYSEX_START);
    for (int i = 0; i < 6; ++i)
        message.push_back(KATANA_HEADER[i]);
    message.push_back((unsigned char) action);

    for (auto byte : address)
        message.push_back(byte);

    for (auto byte : value)
        message.push_back(byte);

    unsigned char sum = 0;
    for (int i = 8; i < message.size(); ++i)
        sum = (sum + message[i]) & 0x7F;
    message.push_back((128 - sum) & 0x7F);

    message.push_back(SYSEX_END);

    midi.send(message);
}

void Katana::set(std::array<unsigned char, 4> address, const std::vector<unsigned char>& value) {
    send(Action::WRITE, address, value);
}

std::future<std::vector<unsigned char>> Katana::get(std::array<unsigned char, 4> address, std::array<unsigned char, 4> value) {
    send(Action::READ, address, {value.begin(), value.end()});
    data[address] = std::promise<std::vector<unsigned char>>();
    return data[address].get_future();
}

void Katana::handshake() {
    // yes
    midi.send({ 0x50, 0x1, 0x14, 0x1b, 0xd7, 0x1 });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    midi.send({ 0xf0, 0x7e, 0x7f, 0x6, 0x1, 0xf7 });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    midi.send({ 0xf0, 0x7e, 0x7f, 0x6, 0x1, 0xf7, 0x0, 0x0, 0x20, 0x30, 0x1c, 0x1b, 0xd7, 0x1, 0x0 });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    midi.send({ 0xf0, 0x41, 0x0, 0x0, 0x0, 0x0, 0x33, 0x12, 0x7f, 0x0, 0x0, 0x1, 0x1, 0x7f, 0xf7 });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    midi.send({ 0xf0, 0x41, 0x0, 0x0, 0x0, 0x0, 0x33, 0x12, 0x7f, 0x0, 0x0, 0x1, 0x1, 0x7f, 0xf7, 0x1, 0x0, 0xf7 });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

template<typename T>
bool startsWith(const std::vector<T>& v1, const std::vector<T>& v2) {
    for (int i = 0; i < v2.size(); ++i) {
        if (v1[i] != v2[i])
            return false;
    }
    return true;
}

static void logMessage(const std::vector<unsigned char>& data) {
    std::cout << "\x1B[91mDEVICE: ";
    for (int i = 0; i < data.size(); ++i) {
        std::cout << "0x" << toHex(data[i]);
        if (i != data.size() - 1)
            std::cout << ", ";
    }

    std::cout << "\033[0m\t\t" << std::endl;
}

void Katana::handleMessage(const std::vector<unsigned char>& data) {
    if (!startsWith(data, { 0xf0, 0x41, 0x0, 0x0, 0x0, 0x0, 0x33, 0x12 }))
        return;

    std::array<unsigned char, 4> address = { data[8], data[9], data[10], data[11] };
    std::vector<unsigned char> value(data.begin() + 12, data.end() - 2);
    
    this->data[address].set_value(std::move(value));
}
