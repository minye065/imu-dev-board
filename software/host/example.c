#include <stdio.h>
#include <windows.h>
#include "imudevboard.h"

int main(void)
{
    imuData data;

    if (imuOpen() != 0)
    {
        printf("board not found\n");
        return 1;
    }

    while (1)
    {
        if (imuGetValues(&data) == 0)
        {
            printf("roll %7.2f  pitch %7.2f  yaw %7.2f\n", data.roll, data.pitch, data.yaw);
        }
        Sleep(50);
    }

    imuClose();
    return 0;
}
