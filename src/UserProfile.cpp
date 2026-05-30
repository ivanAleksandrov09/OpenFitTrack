#include "UserProfile.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>
#include <set>

UserProfile::UserProfile(int id, const std::string &name, int age)
    : id(id), name(name), age(age), nextId(1)
{
    exercises.reserve(64);
    workouts.reserve(64);
    routines.reserve(64);
}

int UserProfile::getId() const { return id; }
std::string UserProfile::getName() const { return name; }
int UserProfile::getAge() const { return age; }
const std::vector<Workout> &UserProfile::getWorkouts() const { return workouts; }
const std::vector<Exercise> &UserProfile::getExercises() const { return exercises; }
const std::vector<PersonalRecord> &UserProfile::getPRs() const { return prs; }

// ── Exercise management ────────────────────────────────────────────────────

Exercise &UserProfile::addExercise(const std::string &name,
                                   const std::string &muscleGroup,
                                   ExerciseType type)
{
    exercises.emplace_back(nextId++, getCurrentDate(), name, muscleGroup, type);
    return exercises.back();
}

void UserProfile::removeExercise(int exerciseId)
{
    exercises.erase(
        std::remove_if(exercises.begin(), exercises.end(),
                       [exerciseId](const Exercise &e)
                       { return e.getId() == exerciseId; }),
        exercises.end());
}

Exercise *UserProfile::findExercise(int exerciseId)
{
    for (auto &e : exercises)
        if (e.getId() == exerciseId)
            return &e;
    return nullptr;
}

// ── Workout management ─────────────────────────────────────────────────────

Workout &UserProfile::addWorkout(const std::string &date,
                                 int durationMin,
                                 const std::string &notes)
{
    workouts.emplace_back(nextId++, getCurrentDate(), date, durationMin, notes);
    return workouts.back();
}

void UserProfile::removeWorkout(int workoutId)
{
    workouts.erase(
        std::remove_if(workouts.begin(), workouts.end(),
                       [workoutId](const Workout &w)
                       { return w.getId() == workoutId; }),
        workouts.end());
}

Workout *UserProfile::findWorkout(int workoutId)
{
    for (auto &w : workouts)
        if (w.getId() == workoutId)
            return &w;
    return nullptr;
}

// ── Set management ─────────────────────────────────────────────────────────

void UserProfile::addSetToWorkout(int workoutId, int exerciseId,
                                  int reps, float weight, int restSec)
{
    Workout *w = findWorkout(workoutId);
    Exercise *e = findExercise(exerciseId);
    if (!w || !e)
    {
        std::cerr << "Workout or Exercise not found.\n";
        return;
    }
    Set s(nextId++, getCurrentDate(), reps, weight, restSec);
    w->addSet(*e, s);
}

// ── PR detection ───────────────────────────────────────────────────────────

void UserProfile::detectPRs(int workoutId)
{
    Workout *w = findWorkout(workoutId);
    if (!w)
        return;

    for (const auto &ws : w->getSets())
    {
        int exId = ws.exercise.getId();
        float newWeight = ws.set.getWeight();
        int newReps = ws.set.getReps();

        // find existing PR for this exercise
        auto it = std::find_if(prs.begin(), prs.end(),
                               [exId](const PersonalRecord &pr)
                               {
                                   return pr.getExercise().getId() == exId;
                               });

        if (it == prs.end())
        {
            // no PR yet - this is the first record
            prs.emplace_back(nextId++, getCurrentDate(),
                             ws.exercise, newWeight, newReps, w->getDate());
            std::cout << "  New PR: " << prs.back() << "\n";
        }
        else if (newWeight > it->getWeight())
        {
            // beat existing PR
            *it = PersonalRecord(it->getId(), getCurrentDate(),
                                 ws.exercise, newWeight, newReps, w->getDate());
            std::cout << "  PR beaten: " << *it << "\n";
        }
    }
}

const PersonalRecord *UserProfile::getPRForExercise(int exerciseId) const
{
    for (const auto &pr : prs)
        if (pr.getExercise().getId() == exerciseId)
            return &pr;
    return nullptr;
}

// ── Progress ───────────────────────────────────────────────────────────────

std::vector<std::pair<std::string, float>>
UserProfile::getProgressForExercise(int exerciseId) const
{
    std::vector<std::pair<std::string, float>> history;

    for (const auto &w : workouts)
    {
        float maxWeight = 0.0f;
        bool found = false;
        for (const auto &ws : w.getSets())
        {
            if (ws.exercise.getId() == exerciseId)
            {
                maxWeight = std::max(maxWeight, ws.set.getWeight());
                found = true;
            }
        }
        if (found)
            history.emplace_back(w.getDate(), maxWeight);
    }

    std::sort(history.begin(), history.end());
    return history;
}

// ── Weekly summary ─────────────────────────────────────────────────────────

WeeklySummary UserProfile::getWeeklySummary(const std::string &weekStart,
                                            const std::string &weekEnd) const
{
    WeeklySummary summary;
    summary.weekStart = weekStart;
    summary.weekEnd = weekEnd;
    summary.totalWorkouts = 0;
    summary.totalVolume = 0.0f;
    summary.totalReps = 0;

    std::set<std::string> muscles;

    for (const auto &w : workouts)
    {
        if (w.getDate() >= weekStart && w.getDate() <= weekEnd)
        {
            summary.totalWorkouts++;
            summary.totalVolume += w.getTotalVolume();
            summary.totalReps += w.getTotalReps();

            for (const auto &ws : w.getSets())
                muscles.insert(ws.exercise.getMuscleGroup());
        }
    }

    for (const auto &pr : prs)
        if (pr.getDate() >= weekStart && pr.getDate() <= weekEnd)
            summary.prsBeaten.push_back(pr);

    summary.musclesHit.assign(muscles.begin(), muscles.end());
    return summary;
}

// ── Routine management ─────────────────────────────────────────────────────────

const std::vector<Routine> &UserProfile::getRoutines() const { return routines; }

Routine &UserProfile::addRoutine(const std::string &name)
{
    routines.emplace_back(nextId++, name);
    return routines.back();
}

void UserProfile::removeRoutine(int routineId)
{
    routines.erase(
        std::remove_if(routines.begin(), routines.end(),
                       [routineId](const Routine &r)
                       { return r.getId() == routineId; }),
        routines.end());
}

Routine *UserProfile::findRoutine(int routineId)
{
    for (auto &r : routines)
        if (r.getId() == routineId)
            return &r;
    return nullptr;
}

void UserProfile::addExerciseToRoutine(int routineId, int exerciseId,
                                       int sets, int defaultRestSec)
{
    Routine *r = findRoutine(routineId);
    Exercise *e = findExercise(exerciseId);
    if (!r || !e)
    {
        std::cerr << "Routine or Exercise not found.\n";
        return;
    }
    r->addExercise(*e, sets, defaultRestSec);
}

void UserProfile::removeExerciseFromRoutine(int routineId, int exerciseId)
{
    Routine *r = findRoutine(routineId);
    if (!r)
    {
        std::cerr << "Routine not found.\n";
        return;
    }
    r->removeExercise(exerciseId);
}

// ── Operators ──────────────────────────────────────────────────────────────

std::ostream &operator<<(std::ostream &os, const UserProfile &up)
{
    os << "=== " << up.name << " (age " << up.age << ") ===\n";
    os << "Exercises: " << up.exercises.size() << "\n";
    os << "Workouts:  " << up.workouts.size() << "\n";
    os << "PRs:       " << up.prs.size() << "\n";
    return os;
}

std::istream &operator>>(std::istream &is, UserProfile &up)
{
    std::cout << "Name: ";
    std::getline(is, up.name);
    std::cout << "Age: ";
    is >> up.age;
    is.ignore();
    return is;
}