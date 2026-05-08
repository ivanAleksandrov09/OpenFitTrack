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

    // Public convenience: return createdAt as a string for consumers
    std::string getCreatedAt() const;

    friend std::ostream &operator<<(std::ostream &os, const Trackable &t);

protected:
    int getRawId() const;
};

#endif