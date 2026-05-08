#include <iostream>
#include "Exercise.h"

int main()
{
    Exercise ex(1, "2026-05-08", "Bench Press", "Chest", ExerciseType::STRENGTH);
    std::cout << ex << std::endl;
    std::cout << ex.serialize() << std::endl;
    return 0;
}