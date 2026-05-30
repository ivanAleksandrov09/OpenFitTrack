#ifndef WORKOUT_H
#define WORKOUT_H

#include "Trackable.h"
#include "Set.h"
#include "Exercise.h"
#include <vector>

struct WorkoutSet
{
    Exercise exercise;
    Set set;
};

class Workout : public Trackable
{
private:
    std::string date;
    int durationMin;
    std::string notes;
    std::vector<WorkoutSet> sets;

public:
    Workout(int id, const std::string &createdAt,
            const std::string &date,
            int durationMin,
            const std::string &notes);

    int getId() const override;
    std::string getDate() const;
    int getDurationMin() const;
    std::string getNotes() const;
    const std::vector<WorkoutSet> &getSets() const;

    void addSet(const Exercise &exercise, const Set &set);
    void removeSet(int setId);
    float getTotalVolume() const;
    int getTotalReps() const;
    void setDurationMin(int durationMin) { this->durationMin = durationMin; }

    std::string serialize() const override;

    friend std::ostream &operator<<(std::ostream &os, const Workout &w);
    friend std::istream &operator>>(std::istream &is, Workout &w);
};

#endif