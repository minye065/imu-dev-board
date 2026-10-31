#pragma once
#include <math.h>

struct Madgwick
{
    float q0 = 1.0f;
    float q1 = 0.0f;
    float q2 = 0.0f;
    float q3 = 0.0f;
    float beta = 0.1f;

    void update(float gx, float gy, float gz, float ax, float ay, float az, float dt)
    {
        const float degToRad = 0.01745329f;
        gx *= degToRad;
        gy *= degToRad;
        gz *= degToRad;

        float qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
        float qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
        float qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
        float qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

        float accNorm = sqrtf(ax * ax + ay * ay + az * az);
        if (accNorm > 0.0f)
        {
            ax /= accNorm;
            ay /= accNorm;
            az /= accNorm;

            float s0 = 4.0f * q0 * q2 * q2 + 2.0f * q2 * ax + 4.0f * q0 * q1 * q1 - 2.0f * q1 * ay;
            float s1 = 4.0f * q1 * q3 * q3 - 2.0f * q3 * ax + 4.0f * q0 * q0 * q1 - 2.0f * q0 * ay - 4.0f * q1
                + 8.0f * q1 * q1 * q1 + 8.0f * q1 * q2 * q2 + 4.0f * q1 * az;
            float s2 = 4.0f * q0 * q0 * q2 + 2.0f * q0 * ax + 4.0f * q2 * q3 * q3 - 2.0f * q3 * ay - 4.0f * q2
                + 8.0f * q2 * q1 * q1 + 8.0f * q2 * q2 * q2 + 4.0f * q2 * az;
            float s3 = 4.0f * q1 * q1 * q3 - 2.0f * q1 * ax + 4.0f * q2 * q2 * q3 - 2.0f * q2 * ay;

            float sNorm = sqrtf(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
            if (sNorm > 0.0f)
            {
                qDot1 -= beta * s0 / sNorm;
                qDot2 -= beta * s1 / sNorm;
                qDot3 -= beta * s2 / sNorm;
                qDot4 -= beta * s3 / sNorm;
            }
        }

        q0 += qDot1 * dt;
        q1 += qDot2 * dt;
        q2 += qDot3 * dt;
        q3 += qDot4 * dt;

        float qNorm = sqrtf(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
        q0 /= qNorm;
        q1 /= qNorm;
        q2 /= qNorm;
        q3 /= qNorm;
    }

    void getAngles(float &roll, float &pitch, float &yaw)
    {
        const float radToDeg = 57.29578f;
        roll = atan2f(2.0f * (q0 * q1 + q2 * q3), 1.0f - 2.0f * (q1 * q1 + q2 * q2)) * radToDeg;
        pitch = asinf(2.0f * (q0 * q2 - q3 * q1)) * radToDeg;
        yaw = atan2f(2.0f * (q0 * q3 + q1 * q2), 1.0f - 2.0f * (q2 * q2 + q3 * q3)) * radToDeg;
    }
};
