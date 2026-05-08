#include <iostream>
#include "UserProfile.h"

int main()
{
    UserProfile user(1, "Alex", 22);

    auto &bench = user.addExercise("Bench Press", "Chest", ExerciseType::STRENGTH);
    auto &squat = user.addExercise("Squat", "Legs", ExerciseType::STRENGTH);

    auto &w = user.addWorkout("2026-05-08", 60, "Good session");
    user.addSetToWorkout(w.getId(), bench.getId(), 8, 100.0f, 90);
    user.addSetToWorkout(w.getId(), bench.getId(), 6, 110.0f, 120);
    user.addSetToWorkout(w.getId(), squat.getId(), 5, 140.0f, 120);

    std::cout << w << "\n";

    std::cout << "Detecting PRs...\n";
    user.detectPRs(w.getId());

    std::cout << "\n"
              << user << "\n";

    auto progress = user.getProgressForExercise(bench.getId());
    std::cout << "Bench progress:\n";
    for (const auto &[date, weight] : progress)
        std::cout << "  " << date << " -> " << weight << "kg\n";

    auto summary = user.getWeeklySummary("2026-05-01", "2026-05-31");
    std::cout << "\nWeekly summary:\n";
    std::cout << "  Workouts: " << summary.totalWorkouts << "\n";
    std::cout << "  Volume:   " << summary.totalVolume << "kg\n";
    std::cout << "  Reps:     " << summary.totalReps << "\n";
    std::cout << "  Muscles:  ";
    for (const auto &m : summary.musclesHit)
        std::cout << m << " ";
    std::cout << "\n";

    return 0;
}