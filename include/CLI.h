#ifndef CLI_H
#define CLI_H

#include <string>
#include <vector>

// ANSI colors
namespace Color
{
    constexpr const char *RESET = "\033[0m";
    constexpr const char *BOLD = "\033[1m";
    constexpr const char *DIM = "\033[2m";

    constexpr const char *BLUE = "\033[34m";
    constexpr const char *CYAN = "\033[36m";
    constexpr const char *WHITE = "\033[37m";
    constexpr const char *GREEN = "\033[32m";
    constexpr const char *YELLOW = "\033[33m";
    constexpr const char *RED = "\033[31m";

    constexpr const char *BBLUE = "\033[94m";
    constexpr const char *BCYAN = "\033[96m";
    constexpr const char *BWHITE = "\033[97m";
    constexpr const char *BGREEN = "\033[92m";
    constexpr const char *BYELLOW = "\033[93m";
}

struct MenuItem
{
    int key;
    std::string label;
    std::string description;
};

class CLI
{
private:
    static constexpr int WIDTH = 60;

    static std::string repeat(char c, int n);
    static std::string center(const std::string &text, int width);
    static std::string padRight(const std::string &text, int width);

public:
    // structural
    static void clear();
    static void newline(int n = 1);

    // headers
    static void printBanner();
    static void printHeader(const std::string &title,
                            const std::string &subtitle = "");
    static void printProfileBar(const std::string &profileName, int age);

    // separators
    static void separatorThick();
    static void separatorThin();
    static void separatorDashed();

    // menus
    static void printMenu(const std::string &title,
                          const std::vector<MenuItem> &items);

    // content rows
    static void printRow(const std::string &label,
                         const std::string &value,
                         const char *valueColor = Color::BWHITE);
    static void printItem(const std::string &tag,
                          const std::string &text,
                          const char *tagColor = Color::BCYAN);
    static void printBullet(const std::string &text);

    // status messages
    static void success(const std::string &msg);
    static void error(const std::string &msg);
    static void warning(const std::string &msg);
    static void info(const std::string &msg);
    static void pr(const std::string &msg);

    // prompts
    static void prompt(const std::string &text);
    static void promptDefault(const std::string &text,
                              const std::string &defaultVal);

    // summary box
    static void printSummaryBox(const std::string &title,
                                const std::vector<std::pair<std::string,
                                                            std::string>> &rows);

    // progress bar
    static void printProgressBar(const std::string &label,
                                 float value, float max,
                                 int barWidth = 24);
};

#endif