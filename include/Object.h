#pragma once

class Object {
 public:
    int x;
    int y;
    int vel_x;
    int vel_y;

    friend std::ostream& operator<<(std::ostream& os, const Object& obj) {
        os << obj.x << " " << obj.y << " " << obj.vel_x << " " << obj.vel_y << "\n";
        return os;
    }
};
