#include "FitnessTracker.h"
#include "CLI.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>

FitnessTracker::FitnessTracker()
    : activeProfile(nullptr), fileManager("data") {}

// ── Helpers ────────────────────────────────────────────────────────────────

void FitnessTracker::printSeparator(char c, int width)
{
    if (c == '=')
        CLI::separatorThick();
    else if (c == '-')
        CLI::separatorThin();
    else
        std::cout << std::string(width, c) << "\n";
}

void FitnessTracker::clearScreen()
{
    CLI::clear();
}

bool FitnessTracker::promptConfirm(const std::string &prompt, bool defaultYes)
{
    std::string input;
    CLI::prompt(prompt);
    std::getline(std::cin, input);
    if (input.empty())
        return defaultYes;
    return (input == "y" || input == "Y");
}

int FitnessTracker::promptInt(const std::string &prompt)
{
    int val;
    while (true)
    {
        CLI::prompt(prompt);
        if (std::cin >> val)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        CLI::warning("Invalid input, try again.");
    }
}

int FitnessTracker::promptIntDefault(const std::string &prompt, int defaultVal)
{
    std::string input;
    CLI::prompt(prompt);
    std::getline(std::cin, input);
    if (input.empty())
        return defaultVal;
    try
    {
        return std::stoi(input);
    }
    catch (...)
    {
        CLI::warning("Invalid input, using default: " + std::to_string(defaultVal));
        return defaultVal;
    }
}

int FitnessTracker::promptIntOptional(const std::string &prompt, bool &skipped)
{
    while (true)
    {
        std::string input;
        CLI::prompt(prompt);
        std::getline(std::cin, input);

        if (input.empty())
        {
            skipped = true;
            return 0;
        }

        try
        {
            skipped = false;
            return std::stoi(input);
        }
        catch (...)
        {
            CLI::warning("Invalid input, try again.");
        }
    }
}

float FitnessTracker::promptFloat(const std::string &prompt)
{
    float val;
    while (true)
    {
        CLI::prompt(prompt);
        if (std::cin >> val)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        CLI::warning("Invalid input, try again.");
    }
}

std::string FitnessTracker::promptString(const std::string &prompt)
{
    std::string val;
    CLI::prompt(prompt);
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
            CLI::warning("Invalid format. Use YYYY-MM-DD.");
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
            CLI::warning("Invalid date.");
            continue;
        }

        if (month < 1 || month > 12)
        {
            CLI::warning("Month must be 1-12.");
            continue;
        }

        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (leap)
            daysInMonth[1] = 29;

        if (day < 1 || day > daysInMonth[month - 1])
        {
            CLI::warning("Invalid day for that month.");
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
        CLI::info("Profile already exists. Loading it instead.");
        profiles.push_back(fileManager.load(name));
    }
    else
    {
        int age = promptInt("Age: ");
        profiles.emplace_back((int)profiles.size() + 1, name, age);
        CLI::success("Profile created: " + name);
    }
    activeProfile = &profiles.back();
}

void FitnessTracker::actionSwitchProfile()
{
    if (profiles.empty())
    {
        CLI::warning("No profiles loaded.");
        return;
    }
    std::vector<MenuItem> items;
    for (size_t i = 0; i < profiles.size(); i++)
        items.push_back({static_cast<int>(i + 1), profiles[i].getName(), ""});
    CLI::printMenu("Select Profile", items);

    int choice = promptInt("Select profile: ");
    if (choice < 1 || choice > (int)profiles.size())
    {
        CLI::warning("Invalid choice.");
        return;
    }
    activeProfile = &profiles[choice - 1];
    CLI::success("Switched to: " + activeProfile->getName());
}

// ── Exercise actions ───────────────────────────────────────────────────────

void FitnessTracker::actionListExercises()
{
    const auto &exercises = activeProfile->getExercises();
    if (exercises.empty())
    {
        CLI::warning("No exercises yet.");
        return;
    }
    CLI::separatorThin();
    for (const auto &e : exercises)
    {
        std::ostringstream oss;
        oss << e;
        CLI::printBullet(oss.str());
    }
    CLI::separatorThin();
}

void FitnessTracker::actionAddExercise()
{
    std::string name = promptString("Exercise name: ");
    std::string muscleGroup = promptString("Muscle group: ");
    int type = promptInt("Type (0=Strength, 1=Cardio, 2=Flexibility): ");

    auto &e = activeProfile->addExercise(name, muscleGroup,
                                         static_cast<ExerciseType>(type));
    std::ostringstream oss;
    oss << e;
    CLI::success("Added: " + oss.str());
}

void FitnessTracker::actionRemoveExercise()
{
    actionListExercises();
    int id = promptInt("Exercise ID to remove: ");
    if (activeProfile->findExercise(id) == nullptr)
    {
        CLI::error("Exercise not found.");
        return;
    }

    activeProfile->removeExercise(id);
    CLI::success("Removed exercise #" + std::to_string(id));
}

// ── Workout actions ────────────────────────────────────────────────────────

void FitnessTracker::actionListWorkouts()
{
    const auto &workouts = activeProfile->getWorkouts();
    if (workouts.empty())
    {
        CLI::warning("No workouts yet.");
        return;
    }
    CLI::separatorThin();
    for (const auto &w : workouts)
        CLI::printRow("[#" + std::to_string(w.getId()) + "] " + w.getDate(),
                      std::to_string(w.getDurationMin()) + " min | " +
                          std::to_string(w.getSets().size()) + " sets");
    CLI::separatorThin();
}

void FitnessTracker::actionAddWorkout()
{
    std::string date = promptDate("Date (YYYY-MM-DD): ");
    int duration = promptInt("Duration (minutes): ");
    std::string notes = promptString("Notes: ");

    auto &w = activeProfile->addWorkout(date, duration, notes);
    CLI::success("Workout created: #" + std::to_string(w.getId()));
}

void FitnessTracker::actionRemoveWorkout()
{
    actionListWorkouts();
    int id = promptInt("Workout ID to remove: ");
    if (activeProfile->findWorkout(id) == nullptr)
    {
        CLI::error("Workout not found.");
        return;
    }

    activeProfile->removeWorkout(id);
    CLI::success("Removed workout #" + std::to_string(id));
}

void FitnessTracker::actionViewWorkout()
{
    actionListWorkouts();
    int id = promptInt("Workout ID to view: ");
    Workout *w = activeProfile->findWorkout(id);
    if (!w)
    {
        CLI::error("Workout not found.");
        return;
    }
    std::ostringstream oss;
    oss << *w;
    CLI::info(oss.str());
}

void FitnessTracker::actionAddSetToWorkout()
{
    actionListWorkouts();
    int wId = promptInt("Workout ID: ");

    if (activeProfile->findWorkout(wId) == nullptr)
    {
        CLI::error("Workout not found.");
        return;
    }

    actionListExercises();
    int eId = promptInt("Exercise ID: ");

    if (activeProfile->findExercise(eId) == nullptr)
    {
        CLI::error("Exercise not found.");
        return;
    }

    int reps = promptInt("Reps: ");
    float weight = promptFloat("Weight (kg): ");
    int restSec = promptInt("Rest (seconds): ");

    activeProfile->addSetToWorkout(wId, eId, reps, weight, restSec);
    activeProfile->detectPRs(wId);
    CLI::success("Set added.");
}

// ── Routine actions ───────────────────────────────────────────────────────────────

void FitnessTracker::actionListRoutines()
{
    const auto &routines = activeProfile->getRoutines();
    if (routines.empty())
    {
        CLI::warning("No routines yet.");
        return;
    }
    CLI::separatorThin();
    for (const auto &r : routines)
        CLI::printRow("[#" + std::to_string(r.getId()) + "] " + r.getName(),
                      std::to_string(r.getEntries().size()) + " exercises");
    CLI::separatorThin();
}

void FitnessTracker::actionViewRoutine()
{
    actionListRoutines();
    int id = promptInt("Routine ID to view: ");
    Routine *r = activeProfile->findRoutine(id);
    if (!r)
    {
        CLI::error("Routine not found.");
        return;
    }
    std::ostringstream oss;
    oss << *r;
    CLI::info(oss.str());
}

void FitnessTracker::actionCreateRoutine()
{
    std::string name = promptString("Routine name: ");
    Routine &r = activeProfile->addRoutine(name);
    CLI::success("Routine created: #" + std::to_string(r.getId()) + " " + r.getName());

    while (true)
    {
        actionListExercises();
        CLI::printItem("0", "Done adding exercises");
        int eId = promptInt("Exercise ID to add (0 to finish): ");
        if (eId == 0)
            break;

        Exercise *e = activeProfile->findExercise(eId);
        if (!e)
        {
            CLI::error("Exercise not found.");
            continue;
        }

        int sets = promptInt("Number of sets: ");
        int restSec = promptInt("Default rest (seconds): ");

        activeProfile->addExerciseToRoutine(r.getId(), eId, sets, restSec);
        CLI::info("Added: " + e->getName() + " x" + std::to_string(sets) +
                  " sets | rest: " + std::to_string(restSec) + "s");
    }

    std::ostringstream oss;
    oss << r;
    CLI::success("Routine saved.");
    CLI::info(oss.str());
}

void FitnessTracker::actionRemoveRoutine()
{
    actionListRoutines();
    int id = promptInt("Routine ID to remove: ");

    Routine *r = activeProfile->findRoutine(id);
    if (!r)
    {
        CLI::error("Routine not found.");
        return;
    }

    activeProfile->removeRoutine(id);
    CLI::success("Removed routine #" + std::to_string(id));
}

void FitnessTracker::actionAddExerciseToRoutine()
{
    actionListRoutines();
    int rId = promptInt("Routine ID: ");
    Routine *r = activeProfile->findRoutine(rId);
    if (!r)
    {
        CLI::error("Routine not found.");
        return;
    }

    actionListExercises();
    int eId = promptInt("Exercise ID: ");
    Exercise *e = activeProfile->findExercise(eId);
    if (!e)
    {
        CLI::error("Exercise not found.");
        return;
    }

    int sets = promptInt("Number of sets: ");
    int restSec = promptInt("Default rest (seconds): ");

    activeProfile->addExerciseToRoutine(rId, eId, sets, restSec);
    CLI::success("Added " + e->getName() + " to " + r->getName());
}

void FitnessTracker::actionRemoveExerciseFromRoutine()
{
    actionListRoutines();
    int rId = promptInt("Routine ID: ");
    Routine *r = activeProfile->findRoutine(rId);
    if (!r)
    {
        CLI::error("Routine not found.");
        return;
    }

    std::cout << *r << "\n";
    int eId = promptInt("Exercise ID to remove: ");

    Exercise *e = activeProfile->findExercise(eId);
    if (!e)
    {
        CLI::error("Exercise not found.");
        return;
    }

    activeProfile->removeExerciseFromRoutine(rId, eId);
    CLI::success("Removed exercise #" + std::to_string(eId) + " from routine.");
}

void FitnessTracker::actionStartWorkoutFromRoutine()
{
    actionListRoutines();
    int rId = promptInt("Routine ID: ");
    Routine *routine = activeProfile->findRoutine(rId);
    if (!routine)
    {
        CLI::error("Routine not found.");
        return;
    }

    std::string date = promptDate("Date (YYYY-MM-DD): ");
    std::string notes = promptString("Notes (optional): ");

    bool autoTime = false;
    int fixedDuration = 0;

    std::string durInput;
    CLI::prompt("Duration in minutes (Enter to calculate automatically): ");
    std::getline(std::cin, durInput);

    if (durInput.empty())
    {
        autoTime = true;
    }
    else
    {
        try
        {
            fixedDuration = std::stoi(durInput);
        }
        catch (...)
        {
            CLI::warning("Invalid input, calculating automatically.");
            autoTime = true;
        }
    }

    time_t startTime = time(0);
    Workout &w = activeProfile->addWorkout(date, fixedDuration, notes);
    int wId = w.getId();

    printSeparator('=');
    CLI::info("Starting: " + routine->getName());

    for (const auto &re : routine->getEntries())
    {
        printSeparator();
        CLI::printRow(re.exercise.getName(), std::to_string(re.sets) + " sets");
        printSeparator();

        for (int i = 1; i <= re.sets; i++)
        {
            CLI::printItem(std::to_string(i), "Set");

            bool skipped = false;
            int reps = promptIntOptional("    Reps or time (Enter to skip remaining): ", skipped);

            if (skipped)
            {
                bool confirm = promptConfirm(
                    "    Skip remaining sets for " + re.exercise.getName() +
                        "? [Y/n]: ",
                    true);
                if (confirm)
                {
                    break;
                }
                else
                {
                    // re-prompt this same set
                    i--;
                    continue;
                }
            }

            int weight = 0;
            if (re.exercise.getType() == ExerciseType::STRENGTH)
            {
                weight = promptFloat("    Weight (kg): ");
            }
            int rest = promptIntDefault(
                "    Rest (s) [" + std::to_string(re.defaultRestSec) + "]: ",
                re.defaultRestSec);

            activeProfile->addSetToWorkout(wId, re.exercise.getId(),
                                           reps, weight, rest);
        }
    }

    activeProfile->detectPRs(wId);

    printSeparator('=');
    CLI::success("Workout complete!");

    if (autoTime)
    {
        time_t endTime = time(0);
        int computedMinutes = static_cast<int>(difftime(endTime, startTime) / 60);
        if (computedMinutes < 1)
            computedMinutes = 1;
        activeProfile->findWorkout(wId)->setDurationMin(computedMinutes);
        CLI::info("Duration: " + std::to_string(computedMinutes) + " min");
    }
    else
    {
        CLI::info("Duration: " + std::to_string(fixedDuration) + " min");
    }
    {
        std::ostringstream oss;
        oss << *activeProfile->findWorkout(wId);
        CLI::info(oss.str());
    }

    const auto &prs = activeProfile->getPRs();
    bool anyNew = false;
    for (const auto &pr : prs)
    {
        if (pr.getDate() == date)
        {
            if (!anyNew)
            {
                CLI::pr("PRs set today");
                anyNew = true;
            }
            std::ostringstream oss;
            oss << pr;
            CLI::pr(oss.str());
        }
    }
}

void FitnessTracker::menuRoutines()
{
    while (true)
    {
        CLI::printMenu("Routines", {
                                       {1, "List routines", ""},
                                       {2, "View routine", ""},
                                       {3, "Create routine", ""},
                                       {4, "Add exercise to routine", ""},
                                       {5, "Remove exercise from routine", ""},
                                       {6, "Remove routine", ""},
                                       {0, "Back", ""},
                                   });

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
            CLI::warning("Invalid option.");
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
        CLI::error("Exercise not found.");
        return;
    }

    auto history = activeProfile->getProgressForExercise(eId);
    if (history.empty())
    {
        CLI::warning("No data yet.");
        return;
    }

    CLI::printHeader("Progress: " + e->getName());
    float maxWeight = history.back().second;
    for (const auto &[date, weight] : history)
        CLI::printProgressBar(date, weight, maxWeight);

    const PersonalRecord *pr = activeProfile->getPRForExercise(eId);
    if (pr)
    {
        std::ostringstream oss;
        oss << *pr;
        CLI::pr("Current PR: " + oss.str());
    }
    CLI::separatorThick();
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

    std::string muscles;
    for (size_t i = 0; i < s.musclesHit.size(); i++)
    {
        if (i)
            muscles += "  ";
        muscles += s.musclesHit[i];
    }

    CLI::printSummaryBox("Weekly Summary - " + s.weekStart + " to " + s.weekEnd, {
                                                                                     {"Workouts", std::to_string(s.totalWorkouts)},
                                                                                     {"Volume", std::to_string(s.totalVolume) + " kg"},
                                                                                     {"Total reps", std::to_string(s.totalReps)},
                                                                                     {"Muscles", muscles},
                                                                                 });

    if (!s.prsBeaten.empty())
    {
        CLI::separatorThin();
        CLI::info("PRs set this week");
        for (const auto &pr : s.prsBeaten)
        {
            std::ostringstream oss;
            oss << pr;
            CLI::pr(oss.str());
        }
        CLI::separatorThin();
    }
}

// ── Sub-menus ──────────────────────────────────────────────────────────────

void FitnessTracker::menuExercises()
{
    while (true)
    {
        CLI::printMenu("Exercises", {
                                        {1, "List exercises", ""},
                                        {2, "Add exercise", ""},
                                        {3, "Remove exercise", ""},
                                        {0, "Back", ""},
                                    });

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
            CLI::warning("Invalid option.");
        }
    }
}

void FitnessTracker::menuWorkouts()
{
    while (true)
    {
        CLI::printMenu("Workouts", {
                                       {1, "List workouts", ""},
                                       {2, "Add workout", ""},
                                       {3, "Start workout from routine", ""},
                                       {4, "View workout", ""},
                                       {5, "Add set to workout", ""},
                                       {6, "Remove workout", ""},
                                       {0, "Back", ""},
                                   });

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
            actionStartWorkoutFromRoutine();
            break;
        case 4:
            actionViewWorkout();
            break;
        case 5:
            actionAddSetToWorkout();
            break;
        case 6:
            actionRemoveWorkout();
            break;
        case 0:
            return;
        default:
            CLI::warning("Invalid option.");
        }
    }
}

// ── Main menu ──────────────────────────────────────────────────────────────

void FitnessTracker::menuMain()
{
    while (true)
    {
        std::string title = "Fitness Tracker";
        if (activeProfile)
            title += "  [" + activeProfile->getName() + "]";
        CLI::printMenu(title, {
                                  {1, "Exercises", ""},
                                  {2, "Workouts", ""},
                                  {3, "Routines", ""},
                                  {4, "Progress", ""},
                                  {5, "Weekly summary", ""},
                                  {6, "Switch profile", ""},
                                  {7, "Save", ""},
                                  {0, "Exit", ""},
                              });

        int choice = promptInt("> ");

        if (choice != 0 && !activeProfile && choice != 5)
        {
            CLI::warning("No active profile. Please select or create one first.");
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
            CLI::info("Goodbye.");
            return;
        default:
            CLI::warning("Invalid option.");
        }
    }
}

void FitnessTracker::run()
{
    CLI::printBanner();
    CLI::info("Welcome to Fitness Tracker");

    // load all existing profiles
    auto savedProfiles = fileManager.listProfiles();
    for (const auto &name : savedProfiles)
    {
        CLI::info("Loading profile: " + name);
        profiles.push_back(fileManager.load(name));
    }

    // ask which profile to use
    if (!profiles.empty())
    {
        std::vector<MenuItem> items;
        for (size_t i = 0; i < profiles.size(); i++)
            items.push_back({static_cast<int>(i + 1), profiles[i].getName(), ""});
        items.push_back({static_cast<int>(profiles.size() + 1), "Create new profile", ""});
        CLI::printMenu("Existing profiles", items);

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
        CLI::warning("No saved profiles found.");
        actionCreateProfile();
    }

    menuMain();
}