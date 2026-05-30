#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "UserProfile.h"
#include <fstream>
#include <string>

class FileManager
{
private:
    std::string directory;

    std::string profilePath(const std::string &name) const;
    void saveExercises(std::ofstream &file, const UserProfile &up) const;
    void saveWorkouts(std::ofstream &file, const UserProfile &up) const;
    void savePRs(std::ofstream &file, const UserProfile &up) const;
    void saveRoutines(std::ofstream &file, const UserProfile &up) const;

    void loadExercises(std::ifstream &file, UserProfile &up, int count) const;
    void loadWorkouts(std::ifstream &file, UserProfile &up, int count) const;
    void loadRoutines(std::ifstream &file, UserProfile &up, int count) const;

public:
    explicit FileManager(const std::string &directory = "data");

    void save(const UserProfile &up) const;
    UserProfile load(const std::string &name) const;
    bool exists(const std::string &name) const;
    std::vector<std::string> listProfiles() const;

    friend std::ostream &operator<<(std::ostream &os, const FileManager &fm);
};

#endif