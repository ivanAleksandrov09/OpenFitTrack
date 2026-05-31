#include "CLI.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

// ── Utilities ──────────────────────────────────────────────────────────────

std::string CLI::repeat(char c, int n)
{
    return n > 0 ? std::string(n, c) : "";
}

std::string CLI::center(const std::string &text, int width)
{
    int padding = width - (int)text.size();
    if (padding <= 0)
        return text;
    int left = padding / 2;
    int right = padding - left;
    return repeat(' ', left) + text + repeat(' ', right);
}

std::string CLI::padRight(const std::string &text, int width)
{
    if ((int)text.size() >= width)
        return text;
    return text + repeat(' ', width - (int)text.size());
}

// ── Structural ─────────────────────────────────────────────────────────────

void CLI::clear()
{
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    system("cls");
#else
    system("clear");
#endif
}

void CLI::newline(int n)
{
    for (int i = 0; i < n; i++)
        std::cout << "\n";
}

// ── Separators ─────────────────────────────────────────────────────────────

void CLI::separatorThick()
{
    std::cout << Color::BLUE
              << repeat('=', WIDTH)
              << Color::RESET << "\n";
}

void CLI::separatorThin()
{
    std::cout << Color::DIM
              << repeat('-', WIDTH)
              << Color::RESET << "\n";
}

void CLI::separatorDashed()
{
    std::cout << Color::DIM;
    for (int i = 0; i < WIDTH / 2; i++)
        std::cout << "- ";
    std::cout << Color::RESET << "\n";
}

// ── Banner ─────────────────────────────────────────────────────────────────

void CLI::printBanner()
{
    clear();
    std::cout << Color::BBLUE;
    std::cout << "\n";
    std::cout << "   ::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n";
    std::cout << "   ::                                                    ::\n";
    std::cout << "   ::    ";
    std::cout << Color::BWHITE << Color::BOLD;
    std::cout << "F I T N E S S   T R A C K E R";
    std::cout << Color::BBLUE;
    std::cout << "                ::\n";
    std::cout << "   ::                                                    ::\n";
    std::cout << "   ::    ";
    std::cout << Color::DIM << Color::WHITE;
    std::cout << "track  lift  progress  repeat          ";
    std::cout << Color::BBLUE;
    std::cout << "        ::\n";
    std::cout << "   ::                                                    ::\n";
    std::cout << "   ::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n";
    std::cout << Color::RESET << "\n";
}

// ── Headers ────────────────────────────────────────────────────────────────

void CLI::printHeader(const std::string &title, const std::string &subtitle)
{
    separatorThick();
    std::cout << Color::BOLD << Color::BWHITE
              << "  " << title
              << Color::RESET << "\n";
    if (!subtitle.empty())
    {
        std::cout << Color::DIM << Color::WHITE
                  << "  " << subtitle
                  << Color::RESET << "\n";
    }
    separatorThin();
}

void CLI::printProfileBar(const std::string &profileName, int age)
{
    std::cout << Color::DIM
              << "  profile: "
              << Color::RESET
              << Color::BCYAN << Color::BOLD
              << profileName
              << Color::RESET
              << Color::DIM
              << "  |  age: "
              << Color::RESET
              << Color::BWHITE
              << age
              << Color::RESET
              << "\n";
    separatorThin();
}

// ── Menu ───────────────────────────────────────────────────────────────────

void CLI::printMenu(const std::string &title,
                    const std::vector<MenuItem> &items)
{
    printHeader(title);
    newline();
    for (const auto &item : items)
    {
        if (item.key == 0)
        {
            std::cout << Color::DIM
                      << "  [ 0 ]  " << item.label
                      << Color::RESET << "\n";
        }
        else
        {
            std::cout << "  "
                      << Color::BBLUE << Color::BOLD
                      << "[ " << item.key << " ]"
                      << Color::RESET
                      << "  "
                      << Color::BWHITE
                      << padRight(item.label, 22)
                      << Color::RESET;
            if (!item.description.empty())
            {
                std::cout << Color::DIM << Color::WHITE
                          << item.description
                          << Color::RESET;
            }
            std::cout << "\n";
        }
    }
    newline();
    separatorThin();
}

// ── Content rows ───────────────────────────────────────────────────────────

void CLI::printRow(const std::string &label,
                   const std::string &value,
                   const char *valueColor)
{
    std::cout << "  "
              << Color::DIM << Color::WHITE
              << padRight(label, 20)
              << Color::RESET
              << valueColor
              << value
              << Color::RESET << "\n";
}

void CLI::printItem(const std::string &tag,
                    const std::string &text,
                    const char *tagColor)
{
    std::cout << "  "
              << tagColor << Color::BOLD
              << "[" << tag << "]"
              << Color::RESET
              << "  "
              << Color::BWHITE
              << text
              << Color::RESET << "\n";
}

void CLI::printBullet(const std::string &text)
{
    std::cout << Color::BLUE << "  ->  "
              << Color::RESET
              << Color::WHITE << text
              << Color::RESET << "\n";
}

// ── Status messages ────────────────────────────────────────────────────────

void CLI::success(const std::string &msg)
{
    std::cout << Color::BGREEN << "  [ok]  "
              << Color::RESET << Color::WHITE
              << msg << Color::RESET << "\n";
}

void CLI::error(const std::string &msg)
{
    std::cout << Color::RED << "  [!!]  "
              << Color::RESET << Color::WHITE
              << msg << Color::RESET << "\n";
}

void CLI::warning(const std::string &msg)
{
    std::cout << Color::BYELLOW << "  [**]  "
              << Color::RESET << Color::WHITE
              << msg << Color::RESET << "\n";
}

void CLI::info(const std::string &msg)
{
    std::cout << Color::BCYAN << "  [--]  "
              << Color::RESET << Color::WHITE
              << msg << Color::RESET << "\n";
}

void CLI::pr(const std::string &msg)
{
    std::cout << Color::BYELLOW << Color::BOLD
              << "  [PR]  "
              << Color::RESET
              << Color::BWHITE << Color::BOLD
              << msg
              << Color::RESET << "\n";
}

// ── Prompts ────────────────────────────────────────────────────────────────

void CLI::prompt(const std::string &text)
{
    std::cout << Color::BCYAN << "  -> "
              << Color::RESET
              << Color::WHITE << text
              << Color::RESET;
}

void CLI::promptDefault(const std::string &text,
                        const std::string &defaultVal)
{
    std::cout << Color::BCYAN << "  -> "
              << Color::RESET
              << Color::WHITE << text
              << Color::RESET
              << Color::DIM << " [" << defaultVal << "]: "
              << Color::RESET;
}

// ── Summary box ────────────────────────────────────────────────────────────

void CLI::printSummaryBox(const std::string &title,
                          const std::vector<std::pair<std::string,
                                                      std::string>> &rows)
{
    separatorThick();
    std::cout << Color::BOLD << Color::BWHITE
              << "  " << title
              << Color::RESET << "\n";
    separatorThin();
    for (const auto &[label, value] : rows)
        printRow(label, value, Color::BGREEN);
    separatorThick();
}

// ── Progress bar ───────────────────────────────────────────────────────────

void CLI::printProgressBar(const std::string &label,
                           float value, float max, int barWidth)
{
    int filled = (max > 0)
                     ? static_cast<int>((value / max) * barWidth)
                     : 0;
    filled = std::min(filled, barWidth);

    std::cout << "  "
              << Color::DIM << Color::WHITE
              << padRight(label, 18)
              << Color::RESET
              << Color::BLUE << "["
              << Color::BGREEN
              << repeat('|', filled)
              << Color::DIM
              << repeat('.', barWidth - filled)
              << Color::BLUE << "]"
              << Color::RESET
              << "  "
              << Color::BWHITE
              << value
              << Color::RESET << "\n";
}