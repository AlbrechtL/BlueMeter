
#include <qdebug.h>

#include "protocol_coder.h"

/*
Source: https://www.eevblog.com/forum/projects/hacking-the-bosch-glm-20-laser-measuring-tape/msg1331649/#msg1331649
These commands work with Bosch PLR15 serialport

send frame:    [startbyte][command][length]([data])[checksum]
receive frame: [status][length][...][checksum]

Get serialnumber: C006004A
Get "firmware" version:C00400BA
Backlight ON:C0470020
Backlight OFF:C0480062
Laser ON:C0410096
Laser OFF:C042001E
Start measure:C04000EE

Example measure:
send C04000EE ->reply 00 04 13 0E 00 00 32
Change endianness
distance in mm=0x00000E13*0,05=180mm

Max measurement rate is about 4 times per second
*/

ProtocolCoder::ProtocolCoder()
{

}

uint8_t ProtocolCoder::genCRC(QByteArray &data)
{
    uint8_t crc = 0xAA;
    uint8_t poly = 0xA6;
    for (int i = 0; i < data.length(); i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ poly);
            else
                crc <<= 1;
        }
    }
    return crc;

}

std::string ProtocolCoder::getStatus(Status status)
{
    switch (status)
    {
    case Status::OK: return "OK";
    case Status::COMMUNICATION_TIMEOUT: return "COMMUNICATION_TIMEOUT";
    case Status::MODE_INVALID: return "MODE_INVALID";
    case Status::CHECKSUM_ERROR: return "CHECKSUM_ERROR";
    case Status::UNKOWN_COMMAND: return "UNKOWN_COMMAND";
    case Status::INVALID_ACCESSLEVEL: return "INVALID_ACCESSLEVEL";
    case Status::INVALID_DATA_BYTES: return "INVALID_DATA_BYTES";
    case Status::RESERVED: return "RESERVED";
    case Status::HARDWARE_ERROR: return "HARDWARE_ERROR";
    case Status::DEVICE_NOT_READY: return "DEVICE_NOT_READY";
    case Status::HAND_RAISED: return "HAND_RAISED";
    }
}

Status ProtocolCoder::decodeStatus(char status_byte)
{
    switch (status_byte)
    {
    case 0x00: return Status::OK;
    case 0x01: return Status::COMMUNICATION_TIMEOUT;
    case 0x02: return Status::MODE_INVALID;
    case 0x03: return Status::CHECKSUM_ERROR;
    case 0x04: return Status::UNKOWN_COMMAND;
    case 0x05: return Status::INVALID_ACCESSLEVEL;
    case 0x06: return Status::INVALID_DATA_BYTES;
    case 0x07: return Status::RESERVED;
    }

    if(status_byte & 0x08) return Status::HARDWARE_ERROR;
    if(status_byte & 0x10) return Status::DEVICE_NOT_READY;
    if(status_byte & 0x20) return Status::HAND_RAISED;

    return Status::UNKOWN_ERROR;
}

void ProtocolCoder::decodeMeasure(QByteArray payload)
{
    uint32_t value = *((uint32_t*) payload.constData()) * 0.05;

    qDebug() << "ProtocolCoder: decodeMeasure" << payload.toHex() << "measure" << value;
}

void ProtocolCoder::decodeGetDeviceInfo(QByteArray payload)
{
    qDebug() << "ProtocolCoder: decodeGetFirmwareVesion" << payload.toHex();

    QString serialNumberPart1 = payload.mid(0,4);
    uint32_t serialNumberPart2 = *((uint32_t*) payload.mid(4,4).constData());
    uint16_t swRevision = *((uint16_t*) payload.mid(8,2).constData());
    uint8_t swVersionMain = *((uint8_t*) payload.mid(10,1).constData());
    uint8_t swVersionSub = *((uint8_t*) payload.mid(11,1).constData());
    uint8_t swVersionBug = *((uint8_t*) payload.mid(12,1).constData());
    uint8_t hwPCBVersion = *((uint8_t*) payload.mid(13,1).constData());
    uint8_t hwPCBVariant = *((uint8_t*) payload.mid(14,1).constData());
    uint8_t hwPCBBug = *((uint8_t*) payload.mid(15,1).constData());
    uint8_t unknown = *((uint8_t*) payload.mid(16,1).constData());
    QString productNumber = payload.mid(17,12);

    qDebug() << "serialNumberPart1" << serialNumberPart1;
    qDebug() << "serialNumberPart2" << serialNumberPart2;
    qDebug() << "swRevision" << swRevision;
    qDebug() << "swVersionMain" << swVersionMain;
    qDebug() << "swVersionSub" << swVersionSub;
    qDebug() << "swVersionBug" << swVersionBug;
    qDebug() << "hwPCBVersion" << hwPCBVersion;
    qDebug() << "hwPCBVariant" << hwPCBVariant;
    qDebug() << "hwPCBBug" << hwPCBBug;
    qDebug() << "unknown" << unknown;
    qDebug() << "productNumber" << productNumber;

}

void ProtocolCoder::decodeGetSerialNumber(QByteArray payload)
{

}

void ProtocolCoder::decodeGetNewMeasurement(QByteArray payload)
{
    // First 2 bytes are unkonwn

    // It looks like there is a counter for measurements
    uint16_t measurement_count = *((uint16_t*) payload.mid(2,2).constData());

    // Measurement value
    float value = *((float*) payload.mid(4,4).constData());

    // Last bytes are also unkonwn

    qDebug() << "New measurement:" << value << "m" << "count:" << measurement_count;
}

void ProtocolCoder::decodeGetName(QByteArray payload)
{
    QString name = payload;
    qDebug() << "Device name:" << name;
}

QByteArray ProtocolCoder::encode(Command command)
{
    QByteArray data;

    // Start byte
    data[0] = (char) 0xC0;

    // Command byte and payload
    switch(command) {
    case Command::GET_SERIAL_NUMBER: data[1] = 0x04; break;
    case Command::GET_NAME: data[1] = 0x05; break;
    case Command::GET_DEVICE_INFO: data[1] = 0x06; break;
    case Command::MEASURE: data[1] = 0x40; break;
    case Command::LASER_ON: data[1] = 0x41; break;
    case Command::LASER_OFF: data[1] = 0x42; break;
    case Command::BACKLIGHT_ON: data[1] = 0x47; break;
    case Command::BACKLIGHT_OFF: data[1] = 0x48; break;
    case Command::CONTROl: data[1] = 0x50; break;
    case Command::GET_MEASUREMENTS: data[1] = 0x51; break;
    case Command::CLEAR_MEASUREMENTS: data[1] = 0x52; break;
    case Command::READ_SETTINGS: data[1] = 0x53; break;
    case Command::WRITE_SETTINGS: data[1] = 0x54; break;
    case Command::GET_NEW_MEASUREMENT: data[1] = 0x55; break;
    }

    if(command == Command::TRY_COMMAND) {
        data[1] = 0x21;
        command_counter++;
    }

    QByteArray payload;

    if(command == Command::GET_NEW_MEASUREMENT) {
        payload[0] = 0x01; // Enable
        payload[1] = 0x00;

        data[2] = payload.length();
        data.append(payload);
    }

    // In case of no payload add a fill byte
    if(data.length() == 2)
        data[2] = 0x00;

    // CRC8 byte
    data[data.length()] = genCRC(data);

    return data;
}

void ProtocolCoder::decodeResponse(Command command, QByteArray data)
{
    Status status;
    uint8_t len = 0;
    uint8_t crc;
    uint8_t recv_command = 0;
    QByteArray payload;
    QByteArray data_no_crc;

    if(data[0] == (char) 0xC0) { // Range meter sends a command
        status= Status::OK;
        recv_command = (uint8_t) data[1];
        len = data[2];
        crc = data[len + 3];
        payload = data.mid(3, len);
        data_no_crc = data.chopped(1);
    }
    else { // Just a response of a command
        status = decodeStatus(data[0]);
        len = data[1];
        crc = data[len + 2];
        payload = data.mid(2, len);
        data_no_crc = data.chopped(1);
    }

    qDebug() << "len:" << len;
    qDebug() << "payload:" << payload.toHex();

    if(genCRC(data_no_crc) != crc)
        throw std::string("Wrong CRC");

    if(status != Status::OK)
        throw std::string(getStatus(status));

    switch(command) {
    case Command::GET_NAME: decodeGetName(payload); break;
    case Command::GET_SERIAL_NUMBER: decodeGetSerialNumber(payload); break;
    case Command::GET_DEVICE_INFO: decodeGetDeviceInfo(payload); break;
    case Command::MEASURE: decodeMeasure(payload); break;
    case Command::GET_NEW_MEASUREMENT: decodeGetNewMeasurement(payload); break;
    default: throw std::string("Command decode not implemented");
    }
}
