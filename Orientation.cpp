#include "Orientation.h"

#define M_PI 3.141592

Unigine::Math::quat ToQuaternion(EulerAngles ea) // roll (x), pitch (Y), yaw (z)
{
    // Abbreviations for the various angular functions

    double cr = cos(ea.pitch * 0.5);
    double sr = sin(ea.pitch * 0.5);
    double cp = cos(ea.roll * 0.5);
    double sp = sin(ea.roll * 0.5);
    double cy = cos(ea.yaw * 0.5);
    double sy = sin(ea.yaw * 0.5);

    Unigine::Math::quat q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}

// this implementation assumes normalized quaternion
// converts to Euler angles in 3-2-1 sequence
EulerAngles ToEulerAngles(Unigine::Math::quat q) {
    EulerAngles angles{};

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.pitch = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
    double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
    angles.roll = 2 * std::atan2(sinp, cosp) - M_PI / 2;

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.yaw = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}