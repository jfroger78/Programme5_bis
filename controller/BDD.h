#ifndef BDD_H
#define BDD_H

// Standard c++ includes
#include <array>
#include <optional>

// Qt includes
#include <QStandardItemModel>
#include <QtSql/QSqlDatabase>

// Custom includes
#include <constants.h>

template <typename T>
struct BDDRowData
{
    std::string rowId;
    std::array<T, 16> value;
    std::array<EColor, 16> color;
    std::array<std::string, 16> colorStr;
};

struct RaceDatas
{
    std::array<int, 8> horses;
    std::array<int, 3> finishers;
    QString discipline;
    int nbrLeaver;
    int distance;
};

class BDD
{
    public:
        /**
         * @brief Default constructor for the BDD class.
         */
        BDD();

        /**
         * @brief Default destructor for the BDD class.
         */
        ~BDD() = default;

        /**
         * @brief Computes the combination for one array.
         * @param p_datas: Datas which permits to compute the combination.
         * @return The compbination for the array.
         */
        std::array<char, 3>& computeCombinationProg5(const std::array<int, 16>& p_datas);

        /**
         * @brief Formats input data to be use to set them into the BDD.
         * @param p_en2Array: The en2 array datas.
         * @param p_en3Array: The en3 array datas.
         * @param p_en2En3Array: The en2 array datas.
         * @param p_ids: The list of en2, en3 and en2en3 array id.
         * @return True if all array are set in database, false otherwise.
         */
        bool formatEn2En3Datas(const QAbstractItemModel* p_en2Array,
                               const QAbstractItemModel* p_en3Array,
                               const QAbstractItemModel* p_en2En3Array,
                               std::array<int, 3>& p_ids);

        /**
         * @brief Formats the race datas.
         * @param p_horse: Horses numbers.
         * @param p_winner: The race winner.
         * @param p_place1: The first place.
         * @param p_place2: The second place.
         * @param p_discipline: The discipline of the race.
         * @param p_nbreLeaver: The number of leaver.
         * @param p_distance : The distance.
         * @param p_id: The id return by the query.
         * @return True if the datas are well inserted, false otherwise.
         */
        bool raceDatas(const std::array<int, 8>& p_horses,
                       const int p_winner,
                       const int p_place1,
                       const int p_place2,
                       const QString& p_discipline,
                       const int p_nbrLeaver,
                       const int p_distance,
                       int& p_id);

        /**
         * @brief Writes the combination into the database.
         * @param p_combinationEn2: The combination, for en2 array, to write in the database.
         * @param p_combinationEn3: The combination, for en3 array, to write in the database.
         * @param p_combinationEn2En3: The combination, for en2En3 array, to write in the database.
         * @param p_id: The id corresponding to the combination row.
         * @return True if the combination is write into the database, false otherwise.
         */
        bool writeCombination(const std::array<char, 3>& p_combinationEn2,
                              const std::array<char, 3>& p_combinationEn3,
                              const std::array<char, 3>& p_combinationEn2En3,
                              int& p_id);

        /**
         * @brief Connects to the database.
         * @return True if the connection is done, false otherwise.
         */
        bool connectToDatabase();

        /**
         * @brief Joins race with en2En3 arrays datas.
         * @param p_raceId: The race id to link with datas.
         * @param p_dataIds: The 3 id for the en2, en3 and en2En3 array datas.
         * @param p_ids: The ids corresponding to the insert in the table.
         * @return True if join is done, false otherwise.
         */
        bool joinDatas(const int& p_raceId,
                       const std::array<int, 3>& p_dataIds,
                       std::array<int, 3>& p_ids);

        /**
        * @brief Joins combination with datas.
        * @param p_combinationId: The id of the current combination.
        * @param p_dataIds: The ids concerning the join table race_en2En3Table.
        * @return True if join is done, false otherwise.
        */
        bool joinCombination(const int& p_combinationId,
                             const std::array<int, 3>& p_dataIds);

        /**
         * @brief Commit or rollback all insert.
         * @param p_isValid: True to commit, false to rollback.
         */
        void insertValidation(const bool p_isValid);

        /**
         * @brief Return the combinations contain in the BDD.
         * @return The list of the combination
         */
        std::vector<CombinationToDisplay> combinations();

        /**
         * @brief Returns race datas associated to the input combination id.
         * @param p_combinationId: The combination id.
         * @return The data race corresponding to the input combination id.
         */
        const std::vector<RaceData> raceDataByCombinationId(const int p_combinationId);

        /**
         * @brief Returns the races datas from the combination.
         * @param p_combination: The combination to search.
         * @return A vector of RaceData corresponding to the combination.
         */
        const std::vector<RaceData> raceDataByCombination(const std::array<std::string, 9>& p_combination);

        /**
         * @brief Closes the database connection.
         */
        void closeConnection();

        /**
         * @brief Returns the combination id by using it's value.
         * @param p_combination: The combination value.
         * @return The combination id.
         */
        const int combinationId(const std::string& p_combination);

        /**
         * @brief Sets if the computation should be done with filter on combination or not.
         * @param p_isFilterByCombination: The new value.
         */
        void setIsFilterByCombination(const bool p_isFilterByCombination)
        {
            m_isFilterByCombination = p_isFilterByCombination;
        }

    private:
        /**
         * @brief Formats array datas from input item mode.
         * @param p_array The data to format.
         * @param p_arrayDatas The array with format data.
         * @return True if data is well format, false otherwise.
         */
        bool formatArrayDatas(const QAbstractItemModel* p_array,
                              std::array<BDDRowData<int>, 9>& p_arrayDatas);

        /**
         * @brief Convert the input QColor into EColor.
         * @param p_color The color to convert.
         * @return The color enum value.
         */
        EColor convertToColor(const QColor& p_color);

        /**
         * @brief Writes the en2 en3 datas into the database.
         * @param p_datas The en2 en3 datas to write.*
         * @param p_type The type of the datas (En2, En3, En2En3).
         * @return True if the datas are well written, false otherwise.
         */
        bool writeEn2En3Datas(const std::array<BDDRowData<int>, 9>& p_datas,
                              const std::string p_type,
                              int& p_id);

        /**
         * @brief Adds a discipline value to the database.
         * @param p_value The value to add.
         * @return True if the value is added, false otherwise.
         */
        bool addDiscplineValue(const std::string& p_value);

        /**
         * @brief Returns combination data from the dataId
         * @param p_dataId: The data id to extract data.
         * @return A list of combination data.
         */
        std::vector<RaceData> combinationData(const int& p_dataId, std::vector<int> &p_raceIdTreat);

        /**
         * @brief Fills en2En3 data into RaceData.
         * @param p_raceId: The race id for which search en2En3.
         * @param p_raceData: The raceData fill.
         */
        void combinationEn2En3(const int p_raceId, RaceData& p_raceData);

        /**
         * @brief Extracts data from the en2en3table.
         * @param p_Ids The en2En3 ids associated to the data id.
         * @param p_type: The type of datas (en2, en3, en2En3).
         * @return Data from en2En3Table.
         */
        std::array<std::array<en2En3Struct, 16>, 9> extractEn2En3Data(const std::array<int, 3>& p_Ids, const std::string p_type);

        /**
         * @brief Extracts data from the sql query value.
         * @param p_datas: The sql query value.
         * @param p_isTotal: True if it's the total row to treat, false otherwise.
         * @return The value containing in the sql query value.
         */
        std::array<en2En3Struct, 16> extractRowData(QString p_datas, const bool p_isTotal = false);

        /**
         * @brief Returns the race data from the raceId.
         * @param p_raceId: The race id to extract data.
         * @return A race data.
         */
        RaceData raceDatas(const int& p_raceId);

        /**
         * @brief Searches if raceId is already treat or not.
         * @param p_raceIds: The list of race id already treat.
         * @param p_raceId: The race id to search.
         * @return True if the raceId is already treat, false otherwise.
         */
        bool isRaceIdAlreadyTreat(const std::vector<int>& p_raceIds, const int& p_raceId);

    private:
        std::array<char, 3> m_combination;
        std::array<BDDRowData<int>, 9> m_en2Datas; //!< The en2 datas.
        std::array<BDDRowData<int>, 9> m_en3Datas; //!< The en3 datas.
        std::array<BDDRowData<int>, 9> m_en2En3Datas; //!< The en2En3 datas.

        QSqlDatabase m_db; //!< The database connection.
        bool m_isFilterByCombination; //!< True if the BDD result should be filter by combination, false otherwise.
};
#endif // BDD_H