#ifndef FITNESSTRACKER_H
#define FITNESSTRACKER_H

#include "UserProfile.h"
#include "FileManager.h"
#include <vector>
#include <string>

class FitnessTracker
{
private:
    std::vector<UserProfile> profiles;
    UserProfile *activeProfile;
    FileManager fileManager;

    // menu screens
    void menuMain();
    void menuExercises();
    void menuWorkouts();
    void menuProgress();
    void menuWeeklySummary();

    // exercise actions
    void actionListExercises();
    void actionAddExercise();
    void actionRemoveExercise();

    // workout actions
    void actionListWorkouts();
    void actionAddWorkout();
    void actionRemoveWorkout();
    void actionViewWorkout();
    void actionAddSetToWorkout();

    // routine actions
    void menuRoutines();
    void actionListRoutines();
    void actionCreateRoutine();
    void actionRemoveRoutine();
    void actionAddExerciseToRoutine();
    void actionRemoveExerciseFromRoutine();
    void actionViewRoutine();
    void actionStartWorkoutFromRoutine();

    // profile actions
    void actionSwitchProfile();
    void actionCreateProfile();

    // helpers
    int promptInt(const std::string &prompt);
    int promptIntDefault(const std::string &prompt, int defaultVal);
    float promptFloat(const std::string &prompt);
    std::string promptString(const std::string &prompt);
    std::string promptDate(const std::string &prompt);
    void printSeparator(char c = '-', int width = 40);
    void clearScreen();

public:
    FitnessTracker();
    void run();
};

#endif