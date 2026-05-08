#ifndef PERSONALRECORD_H
#define PERSONALRECORD_H

#include "Trackable.h"
#include "Exercise.h"

class PersonalRecord : public Trackable
{
private:
    Exercise exercise;
    float weight;
    int reps;
    std::string date;

public:
    PersonalRecord(int id, const std::string &createdAt,
                   const Exercise &exercise,
                   float weight, int reps,
                   const std::string &date);

    int getId() const override;
    const Exercise &getExercise() const;
    float getWeight() const;
    int getReps() const;
    std::string getDate() const;

    std::string serialize() const override;

    friend std::ostream &operator<<(std::ostream &os, const PersonalRecord &pr);
};

#endif