#ifndef PROTOCOL_CODER_H
#define PROTOCOL_CODER_H

#include <QObject>

enum class Command
{
    GET_SERIAL_NUMBER, // 0x04
    GET_NAME, // 0x05
    GET_DEVICE_INFO,  // 0x06
    MEASURE, // 0x40
    LASER_ON,  // 0x41
    LASER_OFF,  // 0x42
    BACKLIGHT_ON, // 0x47
    BACKLIGHT_OFF,  // 0x48
    CONTROl, // 0x50
    GET_MEASUREMENTS,  // 0x51 PLR50C not working
    CLEAR_MEASUREMENTS, // 0x52  PLR50C not working
    READ_SETTINGS, // 0x53  PLR50C not working
    WRITE_SETTINGS, // 0x54  PLR50C not working
    GET_NEW_MEASUREMENT, // 0x55
    TRY_COMMAND, // 0xFF HACK!
};

enum class Status
{
    OK, // 0x00
    COMMUNICATION_TIMEOUT, // 0x01
    MODE_INVALID, // 0x02
    CHECKSUM_ERROR, // 0x03
    UNKOWN_COMMAND, // 0x04
    INVALID_ACCESSLEVEL, // 0x05
    INVALID_DATA_BYTES, // 0x06
    RESERVED, // 0x07
    HARDWARE_ERROR, // 0x08
    DEVICE_NOT_READY, // 0x10
    HAND_RAISED, // 0x20
    UNKOWN_ERROR
};

class ProtocolCoder:QObject
{
    Q_OBJECT
public:
    ProtocolCoder();
    QByteArray encode(Command command);
    void decodeResponse(Command command, QByteArray data);

private:
    uint8_t genCRC(QByteArray &data);
    std::string getStatus(Status status);
    Status decodeStatus(char status_byte);
    void decodeMeasure(QByteArray payload);
    void decodeGetDeviceInfo(QByteArray payload);
    void decodeGetSerialNumber(QByteArray payload);
    void decodeGetNewMeasurement(QByteArray payload);
    void decodeGetName(QByteArray payload);

    uint16_t command_counter = 0x00;

signals:

public slots:
};

#endif // PROTOCOL_CODER_H
