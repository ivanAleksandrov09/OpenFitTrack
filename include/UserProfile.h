#ifndef USERPROFILE_H
#define USERPROFILE_H

#include "Workout.h"
#include "PersonalRecord.h"
#include "Exercise.h"
#include <vector>
#include <string>
#include <map>

struct WeeklySummary
{
    std::string weekStart;
    std::string weekEnd;
    int totalWorkouts;
    float totalVolume;
    int totalReps;
    std::vector<std::string> musclesHit;
    std::vector<PersonalRecord> prsBeaten;
};

class FileManager;

class UserProfile
{
private:
    int id;
    std::string name;
    int age;
    std::vector<Workout> workouts;
    std::vector<Exercise> exercises;
    std::vector<PersonalRecord> prs;
    int nextId;

    friend class FileManager;

public:
    UserProfile(int id, const std::string &name, int age);

    // getters
    int getId() const;
    std::string getName() const;
    int getAge() const;
    const std::vector<Workout> &getWorkouts() const;
    const std::vector<Exercise> &getExercises() const;
    const std::vector<PersonalRecord> &getPRs() const;

    // exercise management
    Exercise &addExercise(const std::string &name,
                          const std::string &muscleGroup,
                          ExerciseType type);
    void removeExercise(int exerciseId);
    Exercise *findExercise(int exerciseId);

    // workout management
    Workout &addWorkout(const std::string &date,
                        int durationMin,
                        const std::string &notes);
    void removeWorkout(int workoutId);
    Workout *findWorkout(int workoutId);

    // set management
    void addSetToWorkout(int workoutId,
                         int exerciseId,
                         int reps, float weight, int restSec);

    // PR detection
    void detectPRs(int workoutId);
    const PersonalRecord *getPRForExercise(int exerciseId) const;

    // progress
    std::vector<std::pair<std::string, float>> getProgressForExercise(int exerciseId) const;

    // weekly summary
    WeeklySummary getWeeklySummary(const std::string &weekStart,
                                   const std::string &weekEnd) const;

    friend std::ostream &operator<<(std::ostream &os, const UserProfile &up);
    friend std::istream &operator>>(std::istream &is, UserProfile &up);
};

#endif