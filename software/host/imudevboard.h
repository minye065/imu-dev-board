#ifndef IMUDEVBOARD_H
#define IMUDEVBOARD_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct imuData
{
    float x;
    float y;
    float z;
    float roll;
    float pitch;
    float yaw;
    float xAcceleration;
    float yAcceleration;
    float zAcceleration;
}imuData;

int imuOpen(void);
int imuOpenPort(const char *portName);
void imuClose(void);

int imuGetValues(imuData *internalData);
float imuGetTemp(void);
int imuZero(void);


#ifdef __cplusplus
}
#endif

#endif
