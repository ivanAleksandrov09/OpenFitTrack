#include "Exercise.h"

Exercise::Exercise(int id, const std::string &createdAt,
                   const std::string &name,
                   const std::string &muscleGroup,
                   ExerciseType type)
    : Trackable(id, createdAt), name(name), muscleGroup(muscleGroup), type(type) {}

int Exercise::getId() const
{
    return getRawId();
}

std::string Exercise::getName() const
{
    return name;
}

std::string Exercise::getMuscleGroup() const
{
    return muscleGroup;
}

ExerciseType Exercise::getType() const
{
    return type;
}

std::string Exercise::getTypeString() const
{
    switch (type)
    {
    case ExerciseType::STRENGTH:
        return "Strength";
    case ExerciseType::CARDIO:
        return "Cardio";
    case ExerciseType::FLEXIBILITY:
        return "Flexibility";
    default:
        return "Unknown";
    }
}

std::string Exercise::serialize() const
{
    return Trackable::serialize() + ",name=" + name + ",muscleGroup=" + muscleGroup + ",type=" + getTypeString();
}

std::ostream &operator<<(std::ostream &os, const Exercise &e)
{
    os << "[Exercise #" << e.getId() << "] "
       << e.name << " | "
       << e.muscleGroup << " | "
       << e.getTypeString();
    return os;
}

std::istream &operator>>(std::istream &is, Exercise &e)
{
    std::cout << "Name: ";
    std::getline(is, e.name);
    std::cout << "Muscle group: ";
    std::getline(is, e.muscleGroup);

    int t;
    std::cout << "Type (0=Strength, 1=Cardio, 2=Flexibility): ";
    is >> t;
    is.ignore();
    e.type = static_cast<ExerciseType>(t);

    return is;
}