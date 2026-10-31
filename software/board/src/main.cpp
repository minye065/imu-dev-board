#include <Arduino.h>
#include <SPI.h>
#include <ICM42688.h>
#include "madgwick.h"

const uint8_t pinMiso = 16;
const uint8_t pinCs = 17;
const uint8_t pinSck = 18;
const uint8_t pinMosi = 19;

const uint8_t replyHeader = 0xA5;
const uint32_t samplePeriodUs = 5000;
const int calibrationSamples = 400;

ICM42688 imu(SPI, pinCs);
Madgwick filter;

float gyroBiasX = 0.0f;
float gyroBiasY = 0.0f;
float gyroBiasZ = 0.0f;
float rollOffset = 0.0f;
float pitchOffset = 0.0f;
float yawOffset = 0.0f;
uint32_t lastSampleUs = 0;

void sendFloats(const float *values, int count)
{
    Serial.write(replyHeader);
    Serial.write((const uint8_t *)values, count * sizeof(float));
}

void calibrateGyro()
{
    float sumX = 0.0f;
    float sumY = 0.0f;
    float sumZ = 0.0f;
    for (int i = 0; i < calibrationSamples; i++)
    {
        imu.getAGT();
        sumX += imu.gyrX();
        sumY += imu.gyrY();
        sumZ += imu.gyrZ();
        delay(5);
    }
    gyroBiasX = sumX / calibrationSamples;
    gyroBiasY = sumY / calibrationSamples;
    gyroBiasZ = sumZ / calibrationSamples;
}

void handleCommand(uint8_t command)
{
    float values[3];
    if (command == 'P')
    {
        Serial.write(replyHeader);
        Serial.write('I');
    }
    else if (command == 'A')
    {
        filter.getAngles(values[0], values[1], values[2]);
        values[0] -= rollOffset;
        values[1] -= pitchOffset;
        values[2] -= yawOffset;
        sendFloats(values, 3);
    }
    else if (command == 'C')
    {
        values[0] = imu.accX();
        values[1] = imu.accY();
        values[2] = imu.accZ();
        sendFloats(values, 3);
    }
    else if (command == 'G')
    {
        values[0] = imu.gyrX() - gyroBiasX;
        values[1] = imu.gyrY() - gyroBiasY;
        values[2] = imu.gyrZ() - gyroBiasZ;
        sendFloats(values, 3);
    }
    else if (command == 'T')
    {
        values[0] = imu.temp();
        sendFloats(values, 1);
    }
    else if (command == 'Z')
    {
        filter.getAngles(rollOffset, pitchOffset, yawOffset);
        Serial.write(replyHeader);
    }
}

void setup()
{
    Serial.begin(115200);

    SPI.setRX(pinMiso);
    SPI.setSCK(pinSck);
    SPI.setTX(pinMosi);

    while (imu.begin() < 0)
    {
        delay(500);
    }

    calibrateGyro();
    lastSampleUs = micros();
}

void loop()
{
    uint32_t nowUs = micros();
    if (nowUs - lastSampleUs >= samplePeriodUs)
    {
        float dt = (nowUs - lastSampleUs) * 1e-6f;
        lastSampleUs = nowUs;
        imu.getAGT();
        filter.update(imu.gyrX() - gyroBiasX, imu.gyrY() - gyroBiasY, imu.gyrZ() - gyroBiasZ,
                      imu.accX(), imu.accY(), imu.accZ(), dt);
    }

    while (Serial.available() > 0)
    {
        handleCommand((uint8_t)Serial.read());
    }
}
