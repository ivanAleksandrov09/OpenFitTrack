#include <iostream>
#include "UserProfile.h"

int main()
{
    UserProfile user(1, "Alex", 22);
    user.addExercise("Bench Press", "Chest", ExerciseType::STRENGTH);
    user.addExercise("Squat", "Legs", ExerciseType::STRENGTH);

    user.addRoutine("Push Day");
    user.addExerciseToRoutine(3, 1, 3, 90);  // 3 sets, 90s rest
    user.addExerciseToRoutine(3, 2, 4, 120); // 4 sets, 120s rest

    for (const auto &r : user.getRoutines())
        std::cout << r << "\n";

    return 0;
}