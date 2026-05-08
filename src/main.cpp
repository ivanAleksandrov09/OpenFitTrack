#include <iostream>
#include "UserProfile.h"
#include "FileManager.h"

int main()
{
    FileManager fm("data");

    // create and save
    {
        UserProfile user(1, "Alex", 22);
        auto &bench = user.addExercise("Bench Press", "Chest", ExerciseType::STRENGTH);
        auto &squat = user.addExercise("Squat", "Legs", ExerciseType::STRENGTH);
        auto &w = user.addWorkout("2026-05-08", 60, "Good session");

        user.addSetToWorkout(w.getId(), bench.getId(), 8, 100.0f, 90);
        user.addSetToWorkout(w.getId(), bench.getId(), 6, 110.0f, 120);
        user.addSetToWorkout(w.getId(), squat.getId(), 5, 140.0f, 120);

        user.detectPRs(w.getId());
        fm.save(user);
    }

    // load and verify
    {
        UserProfile loaded = fm.load("Alex");
        std::cout << loaded << "\n";

        for (const auto &w : loaded.getWorkouts())
            std::cout << w << "\n";

        for (const auto &pr : loaded.getPRs())
            std::cout << pr << "\n";
    }

    return 0;
}