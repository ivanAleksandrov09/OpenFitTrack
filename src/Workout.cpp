#include "Workout.h"
#include <algorithm>

Workout::Workout(int id, const std::string &createdAt,
                 const std::string &date,
                 int durationMin,
                 const std::string &notes)
    : Trackable(id, createdAt), date(date), durationMin(durationMin), notes(notes) {}

int Workout::getId() const
{
    return getRawId();
}

std::string Workout::getDate() const
{
    return date;
}

int Workout::getDurationMin() const
{
    return durationMin;
}

std::string Workout::getNotes() const
{
    return notes;
}

const std::vector<WorkoutSet> &Workout::getSets() const
{
    return sets;
}

void Workout::addSet(const Exercise &exercise, const Set &set)
{
    sets.push_back({exercise, set});
}

void Workout::removeSet(int setId)
{
    sets.erase(
        std::remove_if(sets.begin(), sets.end(),
                       [setId](const WorkoutSet &ws)
                       {
                           return ws.set.getId() == setId;
                       }),
        sets.end());
}

float Workout::getTotalVolume() const
{
    float total = 0.0f;
    for (const auto &ws : sets)
    {
        total += ws.set.getVolume();
    }
    return total;
}

int Workout::getTotalReps() const
{
    int total = 0;
    for (const auto &ws : sets)
    {
        total += ws.set.getReps();
    }
    return total;
}

std::string Workout::serialize() const
{
    std::string result = Trackable::serialize() + ",date=" + date + ",durationMin=" + std::to_string(durationMin) + ",notes=" + notes + ",sets=[";

    for (size_t i = 0; i < sets.size(); i++)
    {
        result += "{" + sets[i].exercise.serialize() + "|" + sets[i].set.serialize() + "}";
        if (i < sets.size() - 1)
            result += ",";
    }
    result += "]";
    return result;
}

std::ostream &operator<<(std::ostream &os, const Workout &w)
{
    os << "[Workout #" << w.getId() << "] "
       << w.date << " | "
       << w.durationMin << " min | "
       << "volume: " << w.getTotalVolume() << "kg | "
       << "sets: " << w.sets.size() << "\n";

    for (const auto &ws : w.sets)
    {
        os << "  " << ws.exercise.getName() << " -> " << ws.set << "\n";
    }

    if (!w.notes.empty())
    {
        os << "  Notes: " << w.notes << "\n";
    }

    return os;
}

std::istream &operator>>(std::istream &is, Workout &w)
{
    std::cout << "Date (YYYY-MM-DD): ";
    std::getline(is, w.date);

    std::cout << "Duration (minutes): ";
    is >> w.durationMin;
    is.ignore();

    std::cout << "Notes: ";
    std::getline(is, w.notes);

    return is;
}