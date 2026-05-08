#ifndef SET_H
#define SET_H

#include "Trackable.h"

class Set : public Trackable
{
private:
    int reps;
    float weight;
    int restSec;

public:
    Set(int id, const std::string &createdAt,
        int reps, float weight, int restSec);

    int getId() const override;
    int getReps() const;
    float getWeight() const;
    int getRestSec() const;
    float getVolume() const;

    std::string serialize() const override;

    friend std::ostream &operator<<(std::ostream &os, const Set &s);
    friend std::istream &operator>>(std::istream &is, Set &s);
};

#endif