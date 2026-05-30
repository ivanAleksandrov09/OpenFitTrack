#include "FileManager.h"
#include <iostream>
#include <sstream>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

FileManager::FileManager(const std::string &directory)
    : directory(directory)
{
#ifdef _WIN32
    _mkdir(directory.c_str());
#else
    mkdir(directory.c_str(), 0777);
#endif
}

std::string FileManager::profilePath(const std::string &name) const
{
    return directory + "/" + name + ".txt";
}

bool FileManager::exists(const std::string &name) const
{
    std::ifstream f(profilePath(name));
    return f.good();
}

// ── Save ───────────────────────────────────────────────────────────────────

void FileManager::saveExercises(std::ofstream &file, const UserProfile &up) const
{
    file << up.exercises.size() << "\n";
    for (const auto &e : up.exercises)
    {
        file << e.getId() << "\n"
             << e.getCreatedAt() << "\n"
             << e.getName() << "\n"
             << e.getMuscleGroup() << "\n"
             << static_cast<int>(e.getType()) << "\n";
    }
}

void FileManager::saveWorkouts(std::ofstream &file, const UserProfile &up) const
{
    file << up.workouts.size() << "\n";
    for (const auto &w : up.workouts)
    {
        file << w.getId() << "\n"
             << w.getCreatedAt() << "\n"
             << w.getDate() << "\n"
             << w.getDurationMin() << "\n"
             << w.getNotes() << "\n";

        const auto &sets = w.getSets();
        file << sets.size() << "\n";
        for (const auto &ws : sets)
        {
            file << ws.exercise.getId() << "\n"
                 << ws.set.getId() << "\n"
                 << ws.set.getCreatedAt() << "\n"
                 << ws.set.getReps() << "\n"
                 << ws.set.getWeight() << "\n"
                 << ws.set.getRestSec() << "\n";
        }
    }
}

void FileManager::saveRoutines(std::ofstream &file, const UserProfile &up) const
{
    file << up.routines.size() << "\n";
    for (const auto &r : up.routines)
    {
        file << r.getId() << "\n"
             << r.getName() << "\n";

        const auto &entries = r.getEntries();
        file << entries.size() << "\n";
        for (const auto &re : entries)
        {
            file << re.exercise.getId() << "\n"
                 << re.sets << "\n"
                 << re.defaultRestSec << "\n";
        }
    }
}

void FileManager::save(const UserProfile &up) const
{
    std::ofstream file(profilePath(up.name));
    if (!file.is_open())
    {
        std::cerr << "Failed to save profile: " << up.name << "\n";
        return;
    }

    file << up.id << "\n"
         << up.name << "\n"
         << up.age << "\n"
         << up.nextId << "\n";

    saveExercises(file, up);
    saveWorkouts(file, up);
    saveRoutines(file, up);

    std::cout << "Profile saved: " << profilePath(up.name) << "\n";
}

// ── Load ───────────────────────────────────────────────────────────────────

void FileManager::loadExercises(std::ifstream &file, UserProfile &up, int count) const
{
    for (int i = 0; i < count; i++)
    {
        int id, type;
        std::string createdAt, name, muscleGroup;

        file >> id;
        file.ignore();
        std::getline(file, createdAt);
        std::getline(file, name);
        std::getline(file, muscleGroup);
        file >> type;
        file.ignore();

        up.exercises.emplace_back(id, createdAt, name,
                                  muscleGroup,
                                  static_cast<ExerciseType>(type));
    }
}

void FileManager::loadWorkouts(std::ifstream &file, UserProfile &up, int count) const
{
    for (int i = 0; i < count; i++)
    {
        int id, durationMin, setCount;
        std::string createdAt, date, notes;

        file >> id;
        file.ignore();
        std::getline(file, createdAt);
        std::getline(file, date);
        file >> durationMin;
        file.ignore();
        std::getline(file, notes);

        Workout w(id, createdAt, date, durationMin, notes);

        file >> setCount;
        file.ignore();
        for (int j = 0; j < setCount; j++)
        {
            int exerciseId, setId, reps, restSec;
            float weight;
            std::string setCreatedAt;

            file >> exerciseId;
            file.ignore();
            file >> setId;
            file.ignore();
            std::getline(file, setCreatedAt);
            file >> reps;
            file.ignore();
            file >> weight;
            file.ignore();
            file >> restSec;
            file.ignore();

            Exercise *e = up.findExercise(exerciseId);
            if (e)
            {
                Set s(setId, setCreatedAt, reps, weight, restSec);
                w.addSet(*e, s);
            }
        }

        up.workouts.push_back(w);
    }
}

void FileManager::loadRoutines(std::ifstream &file, UserProfile &up, int count) const
{
    for (int i = 0; i < count; i++)
    {
        int id;
        std::string name;

        file >> id;
        file.ignore();
        std::getline(file, name);

        Routine r(id, name);

        int entryCount;
        file >> entryCount;
        file.ignore();
        for (int j = 0; j < entryCount; j++)
        {
            int exerciseId, sets, defaultRestSec;
            file >> exerciseId;
            file.ignore();
            file >> sets;
            file.ignore();
            file >> defaultRestSec;
            file.ignore();

            Exercise *e = up.findExercise(exerciseId);
            if (e)
                r.addExercise(*e, sets, defaultRestSec);
        }

        up.routines.push_back(r);
    }
}

UserProfile FileManager::load(const std::string &name) const
{
    std::ifstream file(profilePath(name));
    if (!file.is_open())
    {
        std::cerr << "Profile not found: " << name << "\n";
        return UserProfile(0, name, 0);
    }

    int id, age, nextId;
    std::string loadedName;

    file >> id;
    file.ignore();
    std::getline(file, loadedName);
    file >> age;
    file.ignore();
    file >> nextId;
    file.ignore();

    UserProfile up(id, loadedName, age);
    up.nextId = nextId;

    int exerciseCount, workoutCount, routineCount;
    file >> exerciseCount;
    file.ignore();
    loadExercises(file, up, exerciseCount);

    file >> workoutCount;
    file.ignore();
    loadWorkouts(file, up, workoutCount);

    file >> routineCount;
    file.ignore();
    loadRoutines(file, up, routineCount);

    // regenerate PRs from loaded workouts
    for (const auto &w : up.workouts)
        up.detectPRs(w.getId());

    std::cout << "Profile loaded: " << profilePath(name) << "\n";
    return up;
}

#include <filesystem>

std::vector<std::string> FileManager::listProfiles() const
{
    std::vector<std::string> names;
    if (!std::filesystem::exists(directory))
        return names;

    for (const auto &entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.path().extension() == ".txt")
        {
            names.push_back(entry.path().stem().string());
        }
    }
    return names;
}

std::ostream &operator<<(std::ostream &os, const FileManager &fm)
{
    os << "[FileManager] directory: " << fm.directory;
    return os;
}