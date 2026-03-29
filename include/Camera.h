#include <cmath>
#include "MathTypes.h"

class Camera {
public:
    Camera() {
        position = {0.f, 0.f, 3.f};
        yaw = -90.f;
        pitch = 0.f;
    }

    Camera(const Vec3& pos, float y, float p): position(pos), yaw(y), pitch(p) {}

    Vec3 GetForward() const {
        float yawRad = yaw * PI / 180;
        float pitchRad = pitch * PI / 180;

        Vec3 dir;
        dir.x = cos(yawRad) * cos(pitchRad);
        dir.y = cos(pitchRad);
        dir.z = cos(yawRad) * sin(pitchRad);
        dir.normalize();
        return dir;
    }

    Vec3 GetPosition() const {
        return position;
    }
private:
    Vec3 position;
    float yaw;
    float pitch;

    float speed;
    float sensitivity;
};
