#include "imudevboard.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#define REPLY_HEADER 0xA5

static HANDLE port = INVALID_HANDLE_VALUE;

static int readExact(unsigned char *buffer, DWORD count)
{
    DWORD total = 0;
    while (total < count)
    {
        DWORD got = 0;
        if (!ReadFile(port, buffer + total, count - total, &got, NULL) || got == 0)
        {
            return -1;
        }
        total += got;
    }
    return 0;
}

static int sendCommand(unsigned char command, unsigned char *payload, DWORD payloadSize)
{
    DWORD written = 0;
    unsigned char header = 0;
    if (port == INVALID_HANDLE_VALUE)
    {
        return -1;
    }
    PurgeComm(port, PURGE_RXCLEAR | PURGE_TXCLEAR);
    if (!WriteFile(port, &command, 1, &written, NULL) || written != 1)
    {
        return -1;
    }
    if (readExact(&header, 1) != 0 || header != REPLY_HEADER)
    {
        return -1;
    }
    if (payloadSize > 0)
    {
        return readExact(payload, payloadSize);
    }
    return 0;
}

static int getFloats(unsigned char command, float *out, int count)
{
    unsigned char raw[12];
    if (sendCommand(command, raw, count * sizeof(float)) != 0)
    {
        return -1;
    }
    memcpy(out, raw, count * sizeof(float));
    return 0;
}

int imuOpenPort(const char *portName)
{
    char path[32];
    DCB dcb;
    COMMTIMEOUTS timeouts;
    unsigned char id = 0;

    imuClose();
    snprintf(path, sizeof(path), "\\\\.\\%s", portName);
    port = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (port == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    memset(&dcb, 0, sizeof(dcb));
    dcb.DCBlength = sizeof(dcb);
    GetCommState(port, &dcb);
    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    SetCommState(port, &dcb);

    memset(&timeouts, 0, sizeof(timeouts));
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 200;
    timeouts.WriteTotalTimeoutConstant = 200;
    SetCommTimeouts(port, &timeouts);

    if (sendCommand('P', &id, 1) != 0 || id != 'I')
    {
        imuClose();
        return -1;
    }
    return 0;
}

int imuOpen(void)
{
    char name[16];
    int i;
    for (i = 1; i <= 256; i++)
    {
        snprintf(name, sizeof(name), "COM%d", i);
        if (imuOpenPort(name) == 0)
        {
            return 0;
        }
    }
    return -1;
}

void imuClose(void)
{
    if (port != INVALID_HANDLE_VALUE)
    {
        CloseHandle(port);
        port = INVALID_HANDLE_VALUE;
    }
}

int imuGetValues(imuData *internalData)
{
   float a[3];
    if (getFloats('A', a, 3) != 0)
    {
        return(-1);
    }
    internalData->roll = a[0];
    internalData->pitch = a[1];
    internalData->yaw = a[2];
    float b[3];
    if (getFloats('C', b, 3) != 0)
    {
        return(-1);
    }
    internalData->xAcceleration = b[0];
    internalData->yAcceleration = b[1];
    internalData->zAcceleration = b[2];
    float c[3];
    if (getFloats('G', c, 3) != 0)
    {
        return(-1);
    }
    internalData->x = c[0];
    internalData->y = c[1];
    internalData->z = c[2];


    return(0);
}



float imuGetTemp(void)
{
    float celsius;
    getFloats('T', &celsius, 1);
    return celsius;
}

int imuZero(void)
{
    return sendCommand('Z', NULL, 0);
}
