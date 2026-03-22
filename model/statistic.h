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

struct Composition {
    int number = -1;
    QString column = "";
    
    std::map<QString, int> keepComposition = {{"P", 0}, {"3", 0}, {"4", 0}, {"N", 0}};
    std::map<QString, int> removedComposition = {{"P", 0}, {"3", 0}, {"4", 0}, {"N", 0}};

    void incrementKeepType(const QString& p_type) {
        incrementType(p_type, keepComposition);
    }

    void incrementRemovedType(const QString& p_type) {
        incrementType(p_type, removedComposition);
    }

    void incrementType(const QString& p_type, std::map<QString, int>& p_mapToIncrement) {
        std::map<QString, int>::iterator it = p_mapToIncrement.find(p_type);
        if(p_mapToIncrement.end() == it) {
            p_mapToIncrement.emplace(std::pair<QString, int>(p_type, 1));
        } else {
            it->second++;
        }
    }
};

#endif // STATISTIC_H