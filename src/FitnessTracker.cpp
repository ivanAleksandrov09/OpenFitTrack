#include "FitnessTracker.h"
#include <iostream>
#include <limits>
#include <algorithm>

FitnessTracker::FitnessTracker()
    : activeProfile(nullptr), fileManager("data") {}

// ── Helpers ────────────────────────────────────────────────────────────────

void FitnessTracker::printSeparator(char c, int width)
{
    std::cout << std::string(width, c) << "\n";
}

void FitnessTracker::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int FitnessTracker::promptInt(const std::string &prompt)
{
    int val;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> val)
        {
            std::cin.ignore();
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input, try again.\n";
    }
}

float FitnessTracker::promptFloat(const std::string &prompt)
{
    float val;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> val)
        {
            std::cin.ignore();
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input, try again.\n";
    }
}

std::string FitnessTracker::promptString(const std::string &prompt)
{
    std::string val;
    std::cout << prompt;
    std::getline(std::cin, val);
    return val;
}

std::string FitnessTracker::promptDate(const std::string &prompt)
{
    while (true)
    {
        std::string val = promptString(prompt);

        if (val.size() != 10 || val[4] != '-' || val[7] != '-')
        {
            std::cout << "Invalid format. Use YYYY-MM-DD.\n";
            continue;
        }

        int year, month, day;
        try
        {
            year = std::stoi(val.substr(0, 4));
            month = std::stoi(val.substr(5, 2));
            day = std::stoi(val.substr(8, 2));
        }
        catch (...)
        {
            std::cout << "Invalid date.\n";
            continue;
        }

        if (month < 1 || month > 12)
        {
            std::cout << "Month must be 1-12.\n";
            continue;
        }

        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (leap)
            daysInMonth[1] = 29;

        if (day < 1 || day > daysInMonth[month - 1])
        {
            std::cout << "Invalid day for that month.\n";
            continue;
        }

        return val;
    }
}
// ── Profile actions ────────────────────────────────────────────────────────

void FitnessTracker::actionCreateProfile()
{
    std::string name = promptString("Profile name: ");
    if (fileManager.exists(name))
    {
        std::cout << "Profile already exists. Loading it instead.\n";
        profiles.push_back(fileManager.load(name));
    }
    else
    {
        int age = promptInt("Age: ");
        profiles.emplace_back((int)profiles.size() + 1, name, age);
        std::cout << "Profile created: " << name << "\n";
    }
    activeProfile = &profiles.back();
}

void FitnessTracker::actionSwitchProfile()
{
    if (profiles.empty())
    {
        std::cout << "No profiles loaded.\n";
        return;
    }
    printSeparator();
    for (size_t i = 0; i < profiles.size(); i++)
        std::cout << "  " << i + 1 << ". " << profiles[i].getName() << "\n";
    printSeparator();

    int choice = promptInt("Select profile: ");
    if (choice < 1 || choice > (int)profiles.size())
    {
        std::cout << "Invalid choice.\n";
        return;
    }
    activeProfile = &profiles[choice - 1];
    std::cout << "Switched to: " << activeProfile->getName() << "\n";
}

// ── Exercise actions ───────────────────────────────────────────────────────

void FitnessTracker::actionListExercises()
{
    const auto &exercises = activeProfile->getExercises();
    if (exercises.empty())
    {
        std::cout << "No exercises yet.\n";
        return;
    }
    printSeparator();
    for (const auto &e : exercises)
        std::cout << "  " << e << "\n";
    printSeparator();
}

void FitnessTracker::actionAddExercise()
{
    std::string name = promptString("Exercise name: ");
    std::string muscleGroup = promptString("Muscle group: ");
    int type = promptInt("Type (0=Strength, 1=Cardio, 2=Flexibility): ");

    auto &e = activeProfile->addExercise(name, muscleGroup,
                                         static_cast<ExerciseType>(type));
    std::cout << "Added: " << e << "\n";
}

void FitnessTracker::actionRemoveExercise()
{
    actionListExercises();
    int id = promptInt("Exercise ID to remove: ");
    if (activeProfile->findExercise(id) == nullptr)
    {
        std::cout << "Invalid exercise ID " << id << "\n";
        return;
    }

    activeProfile->removeExercise(id);
    std::cout << "Removed exercise #" << id << "\n";
}

// ── Workout actions ────────────────────────────────────────────────────────

void FitnessTracker::actionListWorkouts()
{
    const auto &workouts = activeProfile->getWorkouts();
    if (workouts.empty())
    {
        std::cout << "No workouts yet.\n";
        return;
    }
    printSeparator();
    for (const auto &w : workouts)
        std::cout << "  [#" << w.getId() << "] "
                  << w.getDate() << " | "
                  << w.getDurationMin() << " min | "
                  << w.getSets().size() << " sets\n";
    printSeparator();
}

void FitnessTracker::actionAddWorkout()
{
    std::string date = promptDate("Date (YYYY-MM-DD): ");
    int duration = promptInt("Duration (minutes): ");
    std::string notes = promptString("Notes: ");

    auto &w = activeProfile->addWorkout(date, duration, notes);
    std::cout << "Workout created: #" << w.getId() << "\n";
}

void FitnessTracker::actionRemoveWorkout()
{
    actionListWorkouts();
    int id = promptInt("Workout ID to remove: ");
    if (activeProfile->findWorkout(id) == nullptr)
    {
        std::cout << "Invalid workout ID # " << id << "\n";
        return;
    }

    activeProfile->removeWorkout(id);
    std::cout << "Removed workout #" << id << "\n";
}

void FitnessTracker::actionViewWorkout()
{
    actionListWorkouts();
    int id = promptInt("Workout ID to view: ");
    Workout *w = activeProfile->findWorkout(id);
    if (!w)
    {
        std::cout << "Not found.\n";
        return;
    }
    std::cout << *w << "\n";
}

void FitnessTracker::actionAddSetToWorkout()
{
    actionListWorkouts();
    int wId = promptInt("Workout ID: ");

    if (activeProfile->findWorkout(wId) == nullptr)
    {
        std::cout << "Invalid workout ID # " << wId << "\n";
        return;
    }

    actionListExercises();
    int eId = promptInt("Exercise ID: ");

    if (activeProfile->findExercise(eId) == nullptr)
    {
        std::cout << "Invalid exercise ID # " << eId << "\n";
        return;
    }

    int reps = promptInt("Reps: ");
    float weight = promptFloat("Weight (kg): ");
    int restSec = promptInt("Rest (seconds): ");

    activeProfile->addSetToWorkout(wId, eId, reps, weight, restSec);
    activeProfile->detectPRs(wId);
    std::cout << "Set added.\n";
}

// ── Routine actions ───────────────────────────────────────────────────────────────

void FitnessTracker::actionListRoutines()
{
    const auto &routines = activeProfile->getRoutines();
    if (routines.empty())
    {
        std::cout << "No routines yet.\n";
        return;
    }
    printSeparator();
    for (const auto &r : routines)
        std::cout << "  [#" << r.getId() << "] " << r.getName()
                  << " - " << r.getEntries().size() << " exercises\n";
    printSeparator();
}

void FitnessTracker::actionViewRoutine()
{
    actionListRoutines();
    int id = promptInt("Routine ID to view: ");
    Routine *r = activeProfile->findRoutine(id);
    if (!r)
    {
        std::cout << "Not found.\n";
        return;
    }
    std::cout << *r << "\n";
}

void FitnessTracker::actionCreateRoutine()
{
    std::string name = promptString("Routine name: ");
    Routine &r = activeProfile->addRoutine(name);
    std::cout << "Routine created: #" << r.getId() << " " << r.getName() << "\n";

    while (true)
    {
        actionListExercises();
        std::cout << "  0. Done adding exercises\n";
        int eId = promptInt("Exercise ID to add (0 to finish): ");
        if (eId == 0)
            break;

        Exercise *e = activeProfile->findExercise(eId);
        if (!e)
        {
            std::cout << "Exercise not found.\n";
            continue;
        }

        int sets = promptInt("Number of sets: ");
        int restSec = promptInt("Default rest (seconds): ");

        activeProfile->addExerciseToRoutine(r.getId(), eId, sets, restSec);
        std::cout << "  Added: " << e->getName()
                  << " x" << sets << " sets | rest: " << restSec << "s\n";
    }

    std::cout << "\nRoutine saved:\n"
              << r << "\n";
}

void FitnessTracker::actionRemoveRoutine()
{
    actionListRoutines();
    int id = promptInt("Routine ID to remove: ");

    Routine *r = activeProfile->findRoutine(id);
    if (!r)
    {
        std::cout << "Not found\n";
        return;
    }

    activeProfile->removeRoutine(id);
    std::cout << "Removed routine #" << id << "\n";
}

void FitnessTracker::actionAddExerciseToRoutine()
{
    actionListRoutines();
    int rId = promptInt("Routine ID: ");
    Routine *r = activeProfile->findRoutine(rId);
    if (!r)
    {
        std::cout << "Not found.\n";
        return;
    }

    actionListExercises();
    int eId = promptInt("Exercise ID: ");
    Exercise *e = activeProfile->findExercise(eId);
    if (!e)
    {
        std::cout << "Not found.\n";
        return;
    }

    int sets = promptInt("Number of sets: ");
    int restSec = promptInt("Default rest (seconds): ");

    activeProfile->addExerciseToRoutine(rId, eId, sets, restSec);
    std::cout << "Added " << e->getName() << " to " << r->getName() << "\n";
}

void FitnessTracker::actionRemoveExerciseFromRoutine()
{
    actionListRoutines();
    int rId = promptInt("Routine ID: ");
    Routine *r = activeProfile->findRoutine(rId);
    if (!r)
    {
        std::cout << "Not found.\n";
        return;
    }

    std::cout << *r << "\n";
    int eId = promptInt("Exercise ID to remove: ");

    Exercise *e = activeProfile->findExercise(eId);
    if (!e)
    {
        std::cout << "Not found.\n";
        return;
    }

    activeProfile->removeExerciseFromRoutine(rId, eId);
    std::cout << "Removed exercise #" << eId << " from routine.\n";
}

void FitnessTracker::menuRoutines()
{
    while (true)
    {
        printSeparator('=');
        std::cout << "  Routines\n";
        printSeparator();
        std::cout << "  1. List routines\n";
        std::cout << "  2. View routine\n";
        std::cout << "  3. Create routine\n";
        std::cout << "  4. Add exercise to routine\n";
        std::cout << "  5. Remove exercise from routine\n";
        std::cout << "  6. Remove routine\n";
        std::cout << "  0. Back\n";
        printSeparator('=');

        int choice = promptInt("> ");
        switch (choice)
        {
        case 1:
            actionListRoutines();
            break;
        case 2:
            actionViewRoutine();
            break;
        case 3:
            actionCreateRoutine();
            break;
        case 4:
            actionAddExerciseToRoutine();
            break;
        case 5:
            actionRemoveExerciseFromRoutine();
            break;
        case 6:
            actionRemoveRoutine();
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid option.\n";
        }
    }
}

// ── Progress ───────────────────────────────────────────────────────────────

void FitnessTracker::menuProgress()
{
    actionListExercises();
    int eId = promptInt("Exercise ID: ");

    Exercise *e = activeProfile->findExercise(eId);
    if (!e)
    {
        std::cout << "Exercise not found.\n";
        return;
    }

    auto history = activeProfile->getProgressForExercise(eId);
    if (history.empty())
    {
        std::cout << "No data yet.\n";
        return;
    }

    printSeparator('=');
    std::cout << "Progress: " << e->getName() << "\n";
    printSeparator();
    for (const auto &[date, weight] : history)
        std::cout << "  " << date << "  ->  " << weight << " kg\n";

    const PersonalRecord *pr = activeProfile->getPRForExercise(eId);
    if (pr)
        std::cout << "\nCurrent PR: " << *pr << "\n";
    printSeparator('=');
}

// ── Weekly summary ─────────────────────────────────────────────────────────

void FitnessTracker::menuWeeklySummary()
{
    time_t now = time(0);
    time_t week_ago = now - 7 * 24 * 60 * 60;

    tm *t1 = localtime(&week_ago);
    char startBuf[11];
    snprintf(startBuf, sizeof(startBuf), "%04d-%02d-%02d",
             1900 + t1->tm_year, 1 + t1->tm_mon, t1->tm_mday);

    tm *t2 = localtime(&now);
    char endBuf[11];
    snprintf(endBuf, sizeof(endBuf), "%04d-%02d-%02d",
             1900 + t2->tm_year, 1 + t2->tm_mon, t2->tm_mday);

    auto s = activeProfile->getWeeklySummary(startBuf, endBuf);

    printSeparator('=');
    std::cout << "Weekly Summary: " << s.weekStart << " to " << s.weekEnd << "\n";
    printSeparator();
    std::cout << "  Workouts : " << s.totalWorkouts << "\n";
    std::cout << "  Volume   : " << s.totalVolume << " kg\n";
    std::cout << "  Reps     : " << s.totalReps << "\n";
    std::cout << "  Muscles  : ";
    for (const auto &m : s.musclesHit)
        std::cout << m << "  ";
    std::cout << "\n";
    if (!s.prsBeaten.empty())
    {
        std::cout << "  PRs set  :\n";
        for (const auto &pr : s.prsBeaten)
            std::cout << "    " << pr << "\n";
    }
    printSeparator('=');
}

// ── Sub-menus ──────────────────────────────────────────────────────────────

void FitnessTracker::menuExercises()
{
    while (true)
    {
        printSeparator('=');
        std::cout << "  Exercises\n";
        printSeparator();
        std::cout << "  1. List exercises\n";
        std::cout << "  2. Add exercise\n";
        std::cout << "  3. Remove exercise\n";
        std::cout << "  0. Back\n";
        printSeparator('=');

        int choice = promptInt("> ");
        switch (choice)
        {
        case 1:
            actionListExercises();
            break;
        case 2:
            actionAddExercise();
            break;
        case 3:
            actionRemoveExercise();
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid option.\n";
        }
    }
}

void FitnessTracker::menuWorkouts()
{
    while (true)
    {
        printSeparator('=');
        std::cout << "  Workouts\n";
        printSeparator();
        std::cout << "  1. List workouts\n";
        std::cout << "  2. Add workout\n";
        std::cout << "  3. View workout\n";
        std::cout << "  4. Add set to workout\n";
        std::cout << "  5. Remove workout\n";
        std::cout << "  0. Back\n";
        printSeparator('=');

        int choice = promptInt("> ");
        switch (choice)
        {
        case 1:
            actionListWorkouts();
            break;
        case 2:
            actionAddWorkout();
            break;
        case 3:
            actionViewWorkout();
            break;
        case 4:
            actionAddSetToWorkout();
            break;
        case 5:
            actionRemoveWorkout();
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid option.\n";
        }
    }
}

// ── Main menu ──────────────────────────────────────────────────────────────

void FitnessTracker::menuMain()
{
    while (true)
    {
        printSeparator('=');
        std::cout << "  Fitness Tracker";
        if (activeProfile)
            std::cout << "  [" << activeProfile->getName() << "]\n";
        else
            std::cout << "\n";
        printSeparator();
        std::cout << "  1. Exercises\n";
        std::cout << "  2. Workouts\n";
        std::cout << "  3. Routines\n";
        std::cout << "  4. Progress\n";
        std::cout << "  5. Weekly summary\n";
        std::cout << "  6. Switch profile\n";
        std::cout << "  7. Save\n";
        std::cout << "  0. Exit\n";
        printSeparator('=');

        int choice = promptInt("> ");

        if (choice != 0 && !activeProfile && choice != 5)
        {
            std::cout << "No active profile. Please select or create one first.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            menuExercises();
            break;
        case 2:
            menuWorkouts();
            break;
        case 3:
            menuRoutines();
            break;
        case 4:
            menuProgress();
            break;
        case 5:
            menuWeeklySummary();
            break;
        case 6:
            actionSwitchProfile();
            break;
        case 7:
            fileManager.save(*activeProfile);
            break;
        case 0:
            if (activeProfile)
                fileManager.save(*activeProfile);
            std::cout << "Goodbye.\n";
            return;
        default:
            std::cout << "Invalid option.\n";
        }
    }
}

void FitnessTracker::run()
{
    std::cout << "Welcome to Fitness Tracker\n";

    // load all existing profiles
    auto savedProfiles = fileManager.listProfiles();
    for (const auto &name : savedProfiles)
    {
        std::cout << "Loading profile: " << name << "\n";
        profiles.push_back(fileManager.load(name));
    }

    // ask which profile to use
    if (!profiles.empty())
    {
        printSeparator();
        std::cout << "Existing profiles:\n";
        for (size_t i = 0; i < profiles.size(); i++)
            std::cout << "  " << i + 1 << ". " << profiles[i].getName() << "\n";
        std::cout << "  " << profiles.size() + 1 << ". Create new profile\n";
        printSeparator();

        int choice = promptInt("Select profile: ");
        if (choice >= 1 && choice <= (int)profiles.size())
        {
            activeProfile = &profiles[choice - 1];
        }
        else
        {
            actionCreateProfile();
        }
    }
    else
    {
        std::cout << "No saved profiles found.\n";
        actionCreateProfile();
    }

    menuMain();
}