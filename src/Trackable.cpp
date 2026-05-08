#include "Trackable.h"

Trackable::Trackable(int id, const std::string &createdAt)
    : id(id), createdAt(createdAt) {}

int Trackable::getRawId() const
{
    return id;
}

std::string Trackable::getCreatedAt() const
{
    return createdAt;
}

std::string Trackable::serialize() const
{
    return "id=" + std::to_string(id) + ",createdAt=" + createdAt;
}

std::ostream &operator<<(std::ostream &os, const Trackable &t)
{
    os << t.serialize();
    return os;
}