#include "Set.h"

Set::Set(int id, const std::string &createdAt,
         int reps, float weight, int restSec)
    : Trackable(id, createdAt), reps(reps), weight(weight), restSec(restSec) {}

int Set::getId() const
{
    return getRawId();
}

int Set::getReps() const
{
    return reps;
}

float Set::getWeight() const
{
    return weight;
}

int Set::getRestSec() const
{
    return restSec;
}

float Set::getVolume() const
{
    return static_cast<float>(reps) * weight;
}

std::string Set::serialize() const
{
    return Trackable::serialize() + ",reps=" + std::to_string(reps) + ",weight=" + std::to_string(weight) + ",restSec=" + std::to_string(restSec);
}

std::ostream &operator<<(std::ostream &os, const Set &s)
{
    os << "[Set #" << s.getId() << "] "
       << s.reps << " reps @ "
       << s.weight << "kg | "
       << "rest: " << s.restSec << "s | "
       << "volume: " << s.getVolume() << "kg";
    return os;
}

std::istream &operator>>(std::istream &is, Set &s)
{
    std::cout << "Reps: ";
    is >> s.reps;

    std::cout << "Weight (kg): ";
    is >> s.weight;

    std::cout << "Rest (seconds): ";
    is >> s.restSec;

    is.ignore();
    return is;
}