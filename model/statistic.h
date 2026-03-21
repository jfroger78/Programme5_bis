#ifndef STATISTIC_H
#define STATISTIC_H

// Standard c++ includes
#include <array>

struct StatisticsData
{
    // Value
    std::map<int, std::array<int, 24>> datas;
    std::array<std::map<int, int>, 24> datasByColumn;
    
    // Totals
    int total = -1;
    std::array<int, 24> totalByColumn = {-1};
    std::map<int, std::array<int, 24>> totalByColumnByValue = {};

};

#endif // STATISTIC_H