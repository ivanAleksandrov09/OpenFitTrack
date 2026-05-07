#ifndef TRACKABLE_H
#define TRACKABLE_H

#include <iostream>
#include <string>

class Trackable
{
private:
    int id;
    std::string createdAt;

public:
    Trackable(int id, const std::string &createdAt);
    virtual ~Trackable() = default;

    virtual int getId() const = 0;
    virtual std::string serialize() const;

    friend std::ostream &operator<<(std::ostream &os, const Trackable &t);

protected:
    int getRawId() const;
    const std::string &getCreatedAt() const;
};

#endif