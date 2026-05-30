#include "Routine.h"
#include <algorithm>

Routine::Routine(int id, const std::string &name)
    : id(id), name(name) {}

int Routine::getId() const { return id; }
std::string Routine::getName() const { return name; }
const std::vector<RoutineExercise> &Routine::getEntries() const { return entries; }

void Routine::addExercise(const Exercise &exercise, int sets, int defaultRestSec)
{
    entries.push_back({exercise, sets, defaultRestSec});
}

void Routine::removeExercise(int exerciseId)
{
    entries.erase(
        std::remove_if(entries.begin(), entries.end(),
                       [exerciseId](const RoutineExercise &re)
                       {
                           return re.exercise.getId() == exerciseId;
                       }),
        entries.end());
}

std::ostream &operator<<(std::ostream &os, const Routine &r)
{
    os << "[Routine #" << r.id << "] " << r.name << "\n";
    for (const auto &re : r.entries)
    {
        os << "  " << re.exercise.getName()
           << " - " << re.sets << " sets"
           << " | rest: " << re.defaultRestSec << "s\n";
    }
    return os;
}
