#include <iostream>
#include "PersonalRecord.h"

int main()
{
    Exercise bench(1, "2026-01-01", "Bench Press", "Chest", ExerciseType::STRENGTH);

    PersonalRecord pr(1, "2026-05-08", bench, 120.0f, 3, "2026-05-08");

    std::cout << pr << "\n";
    std::cout << pr.serialize() << "\n";

    return 0;
}