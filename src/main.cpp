#include <iostream>
#include "Workout.h"

int main()
{
    Workout w(1, "2026-05-08", "2026-05-08", 60, "Felt strong today");

    Exercise bench(1, "2026-05-08", "Bench Press", "Chest", ExerciseType::STRENGTH);
    Exercise squat(2, "2026-05-08", "Squat", "Legs", ExerciseType::STRENGTH);

    Set s1(1, "2026-05-08", 8, 100.0f, 90);
    Set s2(2, "2026-05-08", 5, 140.0f, 120);
    Set s3(3, "2026-05-08", 8, 100.0f, 90);

    std::cin >> s1;

    w.addSet(bench, s1);
    w.addSet(bench, s2);
    w.addSet(squat, s3);

    std::cout << w;
    std::cout << "Total volume: " << w.getTotalVolume() << "kg\n";
    std::cout << "Total reps:   " << w.getTotalReps() << "\n";

    return 0;
}