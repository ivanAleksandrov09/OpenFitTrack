#include "PersonalRecord.h"

PersonalRecord::PersonalRecord(int id, const std::string &createdAt,
                               const Exercise &exercise,
                               float weight, int reps,
                               const std::string &date)
    : Trackable(id, createdAt), exercise(exercise),
      weight(weight), reps(reps), date(date) {}

int PersonalRecord::getId() const
{
    return getRawId();
}

const Exercise &PersonalRecord::getExercise() const
{
    return exercise;
}

float PersonalRecord::getWeight() const
{
    return weight;
}

int PersonalRecord::getReps() const
{
    return reps;
}

std::string PersonalRecord::getDate() const
{
    return date;
}

std::string PersonalRecord::serialize() const
{
    return Trackable::serialize() + ",exerciseId=" + std::to_string(exercise.getId()) + ",exerciseName=" + exercise.getName() + ",weight=" + std::to_string(weight) + ",reps=" + std::to_string(reps) + ",date=" + date;
}

std::ostream &operator<<(std::ostream &os, const PersonalRecord &pr)
{
    os << "[PR] " << pr.exercise.getName()
       << " | " << pr.weight << "kg"
       << " x " << pr.reps << " reps"
       << " | set on " << pr.date;
    return os;
}