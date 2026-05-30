#ifndef ROUTINE_H
#define ROUTINE_H

#include "Exercise.h"
#include <vector>
#include <string>

struct RoutineExercise
{
    Exercise exercise;
    int sets;
    int defaultRestSec;
};

class Routine
{
private:
    int id;
    std::string name;
    std::vector<RoutineExercise> entries;

public:
    Routine(int id, const std::string &name);

    int getId() const;
    std::string getName() const;
    const std::vector<RoutineExercise> &getEntries() const;

    void addExercise(const Exercise &exercise, int sets, int defaultRestSec);
    void removeExercise(int exerciseId);

    friend std::ostream &operator<<(std::ostream &os, const Routine &r);
};

#endif