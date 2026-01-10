#ifndef STATISTIC_H
#define STATISTIC_H

// Standard c++ includes
#include <array>

struct StatisticsData
{
    std::array<int, 24> arrayCircle = {};
    std::array<int, 24> arrayGreen = {};
    std::array<int, 24> arrayBlue = {};
    std::array<int, 24> arrayYellow = {};
    std::array<int, 24> arrayOrange = {};
    std::array<int, 24> arrayBlank = {};
    int totalDatas = 0; // Total of datas.

    // Data to compute V gagnant
    std::array<int, 24> arrayGreenWinner = {};
    std::array<int, 24> arrayGreenWinnerTotal = {};

    // Data to compute V m gagnant
    std::array<int, 24> arrayMGreenWinner = {};
    std::array<int, 24> arrayMGreenWinnerTotal = {};
};

#endif // STATISTIC_H