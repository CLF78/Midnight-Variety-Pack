#pragma once

class Rating {
public:
    virtual ~Rating();

    u16 points;
};
size_assert(Rating, 0x8);
