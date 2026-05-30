#include "FitnessTracker.h"
#include "Routine.h"
#include <iostream>

// FitnessTracker tracker;
// tracker.run();

int main()
{
    Exercise bench(1, "2026-05-08", "Bench Press", "Chest", ExerciseType::STRENGTH);
    Exercise squat(2, "2026-05-08", "Squat", "Legs", ExerciseType::STRENGTH);

    Routine r(1, "Push/Pull A");
    r.addExercise(bench, 3, 90);
    r.addExercise(squat, 4, 120);

    std::cout << r << "\n";
    return 0;
}
