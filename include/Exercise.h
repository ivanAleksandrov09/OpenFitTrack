#ifndef EXERCISE_H
#define EXERCISE_H

#include "Trackable.h"

enum class ExerciseType
{
    STRENGTH,
    CARDIO,
    FLEXIBILITY
};

class Exercise : public Trackable
{
private:
    std::string name;
    std::string muscleGroup;

protected:
    ExerciseType type;

public:
    Exercise(int id, const std::string &createdAt,
             const std::string &name,
             const std::string &muscleGroup,
             ExerciseType type);

    int getId() const override;
    std::string getName() const;
    std::string getMuscleGroup() const;
    ExerciseType getType() const;
    std::string getTypeString() const;

    std::string serialize() const override;

    friend std::ostream &operator<<(std::ostream &os, const Exercise &e);
    friend std::istream &operator>>(std::istream &is, Exercise &e);
};

#endif