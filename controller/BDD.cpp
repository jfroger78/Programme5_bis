// Custom includes
#include "BDD.h"
#include "tools.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

// Custom includes
#include "model/constants.h"

//-----------------------------------------------------------------------------
BDD::BDD()
    : m_isFilterByCombination { false }
//-----------------------------------------------------------------------------
{
    
}

//-----------------------------------------------------------------------------
std::array<char, 3>& BDD::computeCombinationProg5(const std::array<int, 16>& p_datas)
//-----------------------------------------------------------------------------
{
    // P > 3
    if(p_datas[0] > p_datas[2])
    {
        m_combination[0] = '+';
    }
    else if(p_datas[0] == p_datas[2])
    {
        m_combination[0] = '=';
    }
    else
    {
        m_combination[0] = '-';
    }
    
    // P > 3 double
    if(p_datas[0] > p_datas[2])
    {
        m_combination[1] = '-';
    }
    else if(p_datas[0] == p_datas[2])
    {
        m_combination[1] = '=';
    }
    else
    {
        m_combination[1] = '+';
    }

    // P > 3
    if(p_datas[0] > p_datas[1])
    {
        m_combination[2] = '-';
    }
    else if(p_datas[0] == p_datas[1])
    {
        m_combination[2] = '=';
    }
    else
    {
        m_combination[2] = '+';
    }

    return m_combination;
}

//-----------------------------------------------------------------------------
bool BDD::formatEn2En3Datas(const QAbstractItemModel* p_en2Array,
                            const QAbstractItemModel* p_en3Array,
                            const QAbstractItemModel* p_en2En3Array,
                            std::array<int, 3>& p_ids)
//-----------------------------------------------------------------------------
{
    formatArrayDatas(p_en2Array, m_en2Datas);
    if(false == writeEn2En3Datas(m_en2Datas, "En2", p_ids[0]))
    {
        qWarning() << "Error during set En2 array into database";
        return false;
    }
    formatArrayDatas(p_en3Array, m_en3Datas);
    if(false == writeEn2En3Datas(m_en3Datas, "En3", p_ids[1]))
    {
        qWarning() << "Error during set En3 array into database";
        return false;
    }
    formatArrayDatas(p_en2En3Array, m_en2En3Datas);
    if(false == writeEn2En3Datas(m_en2En3Datas, "En2En3", p_ids[2]))
    {
        qWarning() << "Error during set En2En3 array into database";
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool BDD::raceDatas(const std::array<int, 8>& p_horses,
                    const int p_winner,
                    const int p_place1,
                    const int p_place2,
                    const QString& p_discipline,
                    const int p_nbrLeaver,
                    const int p_distance,
                    int& p_id)
//-----------------------------------------------------------------------------
{
    if((0 == p_winner) || (0 == p_place1) || (0 == p_place2) || (p_discipline.isEmpty()) ||
      (0 == p_nbrLeaver) || (0 == p_distance))
    {
        qWarning() << "At least one parameter is invalid";
        return false;
    }
    QString horses = "{";
    for(int indexHorse = 0; indexHorse < p_horses.size(); ++indexHorse)
    {
        if(0 == p_horses[indexHorse])
        {
            qWarning() << "At least one horse is invalid";
            return false;
        }
        horses += QString::number(p_horses[indexHorse]);
        if(p_horses.size() - 1 != indexHorse)
        {
            horses += ",";
        }
    }
    horses += "}";
    QString request = "INSERT INTO race (horses, winner, place1, place2, nbrleaver, distance, discipline) VALUES (";
    request += "'" + horses + "',";
    request += QString::number(p_winner) + ",";
    request += QString::number(p_place1) + ",";
    request += QString::number(p_place2) + ",";
    request += QString::number(p_nbrLeaver) + ",";
    request += QString::number(p_distance) + ",";
    request += "'" + p_discipline + "') RETURNING id;";
    QSqlQuery query(m_db);
    query.prepare(request);
    if(query.exec() && query.next())
    {
        p_id = query.value(0).toInt();
    }
    else
    {
        qWarning() << "Failed to insert" << query.lastError().text();
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool BDD::writeCombination(const std::array<char, 3>& p_combinationEn2,
                           const std::array<char, 3>& p_combinationEn3,
                           const std::array<char, 3>& p_combinationEn2En3,
                           int& p_id)
//-----------------------------------------------------------------------------
{
    // Create combination
    QString combination = "{";
    for(char symbol : p_combinationEn2)
    {
        combination += symbol;
        combination += ",";
    }
    for(char symbol : p_combinationEn3)
    {
        combination += symbol;
        combination += ",";
    }
    for(int index = 0; index < p_combinationEn2En3.size(); ++index)
    {
        combination += p_combinationEn2En3[index];
        if(index != p_combinationEn2En3.size() - 1)
        {
            combination += ",";
        }
    }
    combination += "}";

    // Check if the combination already exists
    QSqlQuery selectQuery(m_db);
    selectQuery.prepare("SELECT id FROM combination WHERE combination = :combination");
    selectQuery.bindValue(":combination", combination);
    if((true == selectQuery.exec()) && selectQuery.next())
    {
        p_id = selectQuery.value(0).toInt();
        return true;
    }

    // Else insert the combination into the table
    if(combination.contains(" "))
    {
        qWarning() << "Error with combination: " << combination;
        return false;
    }
    QSqlQuery insertQuery(m_db);
    insertQuery.prepare("INSERT INTO combination (combination) VALUES(:combination) RETURNING id");
    insertQuery.bindValue(":combination", combination);
    if((true == insertQuery.exec()) && insertQuery.next())
    {
        p_id = insertQuery.value(0).toInt();
        return true;
    }
    else
    {
        qWarning() << "Failed to insert" << insertQuery.lastError().text();
        return false;
    }
}

//-----------------------------------------------------------------------------
bool BDD::connectToDatabase()
//-----------------------------------------------------------------------------
{
    const QString connectionName = "programmeDB";
    if(QSqlDatabase::contains(connectionName))
    {
        m_db = QSqlDatabase::database(connectionName);
        if(m_db.isOpen())
        {
            m_db.close();
        }
    }
    else
    {
        m_db = QSqlDatabase::addDatabase("QPSQL", connectionName);
    }
    m_db.setHostName("localhost");
    m_db.setDatabaseName("programmeDB");
    m_db.setUserName("postgres");
    m_db.setPassword("");

    if(false == m_db.open())
    {
        qWarning() << "Failed to connect to the database:" << m_db.lastError().text();
        return false;
    }

    m_db.transaction();
    return true;
}

//-----------------------------------------------------------------------------
bool BDD::joinDatas(const int& p_raceId,
                    const std::array<int, 3>& p_dataIds,
                    std::array<int, 3>& p_ids)
//-----------------------------------------------------------------------------
{
    QString baseRequest = "INSERT INTO race_en2En3 (race_id, en2En3Table_id) VALUES(" + QString::number(p_raceId) + ",";
    for(int index = 0; index < p_dataIds.size(); ++index)
    {
        QString request = baseRequest;
        request += QString::number(p_dataIds[index]) + ") RETURNING id;";
        QSqlQuery query(m_db);
        query.prepare(request);
        if((true == query.exec()) && query.next())
        {
            p_ids[index] = query.value(0).toInt();
        }
        else
        {
            qWarning() << "Failed to insert " << query.lastError().text();
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
bool BDD::joinCombination(const int& p_combinationId,
                          const std::array<int, 3>& p_dataIds)
//-----------------------------------------------------------------------------
{
    QString baseRequest = "INSERT INTO combination_data (combination_id, data_id) VALUES("+ QString::number(p_combinationId) + ",";
    for(int id: p_dataIds)
    {
        QString request = baseRequest;
        request += QString::number(id) + ");";
        QSqlQuery query(m_db);
        if(false == query.exec(request))
        {
            qWarning() << "Failed to insert " << query.lastError().text();
            return false;
        }
    }
    return true;
}


//-----------------------------------------------------------------------------
bool BDD::formatArrayDatas(const QAbstractItemModel* p_array,
                           std::array<BDDRowData<int>, 9>& p_arrayDatas)
//-----------------------------------------------------------------------------
{
    if(nullptr == p_array)
    {
        qWarning() << "p_array is nullptr";
        return false;
    }
    int rowCount = p_array->rowCount();
    if(p_arrayDatas.size() < rowCount)
    {
        qWarning() << "There is too data in the standard item model. It should contain 9 rows.";
        return false;
    }
    int columnCount = p_array->columnCount();
    for(int row = 0; row < rowCount; ++row)
    {
        p_arrayDatas[row].rowId = p_array->headerData(row, Qt::Orientation::Vertical).toString().toStdString();
        for(int column = 0; column < columnCount; ++column)
        {
            QModelIndex index = p_array->index(row, column);
            QVariant value = p_array->data(index);
            p_arrayDatas[row].value[column] = value.toInt();

            QVariant background = p_array->data(index, Qt::BackgroundRole);
            if(background.canConvert<QBrush>())
            {
                QBrush brush = background.value<QBrush>();
                p_arrayDatas[row].color[column] = convertToColor(brush.color());
                if(EColor::None != p_arrayDatas[row].color[column])
                {
                    p_arrayDatas[row].colorStr[column] = brush.color().name().toStdString();
                }
            }
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
EColor BDD::convertToColor(const QColor& p_color)
//-----------------------------------------------------------------------------
{
    if(GREEN_COLOR == p_color.name().toStdString())
    {
        return EColor::GreenColor;
    }
    else if(BLUE_COLOR == p_color.name().toStdString())
    {
        return EColor::BlueColor;
    }
    else if(YELLOW_COLOR == p_color.name().toStdString())
    {
        return EColor::YellowColor;
    }
    else if(ORANGE_COLOR == p_color.name().toStdString())
    {
        return EColor::OrangeColor;
    }
    else if(BLANK_COLOR == p_color.name().toStdString())
    {
        return EColor::BlankColor;
    }

    return EColor::None;
}

//-----------------------------------------------------------------------------
bool BDD::writeEn2En3Datas(const std::array<BDDRowData<int>, 9>& p_datas,
                           const std::string p_type,
                           int& p_id)
//-----------------------------------------------------------------------------
{
    QString request = "INSERT INTO en2En3Table (type, row1, row2, row3, row4, row5, row6, row7, row8, rowTotal_datas) ";
    request += "VALUES ('" + QString::fromStdString(p_type) + "',";
    for(int index = 0; index < p_datas.size() - 1; ++index)
    {
        BDDRowData currentData = p_datas[index];
        QString rowData = "ROW(ARRAY[";
        QString rowColor = " ARRAY[";
        for(int indexData = 0; indexData < currentData.value.size(); ++indexData)
        {
            rowData += QString::number(currentData.value[indexData]);
            std::string color = Tools::convertColor(currentData.colorStr[indexData]);
            rowColor += "'" + QString::fromStdString(color) + "'";
            if(indexData != currentData.value.size() - 1)
            {
                rowData += ",";
                rowColor += ",";
            }
        }
        rowData += "], ";
        rowColor += "])::rowData,";
        request += rowData + rowColor;
    }
    BDDRowData total = p_datas[p_datas.size() - 1];
    QString data = "ARRAY[";
    for(int index = 0; index < total.value.size(); ++index)
    {
        data += QString::number(total.value[index]);
        if(index != total.value.size() - 1)
        {
            data += ",";
        }
    }
    data += "]) RETURNING id;";
    request += data;
    QSqlQuery query(m_db);
    query.prepare(request);
    if((true == query.exec()) && query.next())
    {
        p_id = query.value(0).toInt();
    }
    else
    {
        qWarning() << "Failed to insert " << query.lastError().text();
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
void BDD::insertValidation(const bool p_isValid)
//-----------------------------------------------------------------------------
{
    if(p_isValid)
    {
        m_db.commit();
    }
    else
    {
        m_db.rollback();
        qWarning() << "Cancel insertion";
    }

    closeConnection();
}

//-----------------------------------------------------------------------------
std::vector<CombinationToDisplay> BDD::combinations()
//-----------------------------------------------------------------------------
{
    QSqlQuery selectQuery(m_db);
    selectQuery.prepare("SELECT * FROM combination;");
    if(selectQuery.exec())
    {
        std::vector<CombinationToDisplay> combinations;
        while(selectQuery.next())
        {
            QString combination = selectQuery.value("combination").toString();
            int id = selectQuery.value("id").toInt();
            CombinationToDisplay comb {id, combination.toStdString()};
            combinations.push_back(comb);
        }
        return combinations;
    }
    else
    {
        qWarning() << "Failed to retrieve combinations:" << selectQuery.lastError().text();
    }
    return {};
}

//-----------------------------------------------------------------------------
const std::vector<RaceData> BDD:: raceDataByCombinationId(const int p_combinationId)
//-----------------------------------------------------------------------------
{
    connectToDatabase();
    QSqlQuery selectQuery(m_db);
    selectQuery.prepare("SELECT data_id FROM combination_data WHERE combination_id = :combinationId");
    selectQuery.bindValue(":combinationId", p_combinationId);
    std::vector<int> raceIdTreat;
    if(selectQuery.exec())
    {
        std::vector<RaceData> raceDatas;
        while(selectQuery.next())
        {
            int dataId = selectQuery.value("data_id").toInt();
            std::vector<RaceData> currentRaceDatas = combinationData(dataId, raceIdTreat);
            if(!currentRaceDatas.empty())
            {
                for(RaceData data: currentRaceDatas)
                {
                    raceDatas.push_back(data);
                }
            }
        }
        closeConnection();
        return raceDatas;
    }

    closeConnection();
    return {};
}

//-----------------------------------------------------------------------------
const std::vector<RaceData> BDD::raceDataByCombination(const std::array<std::string, 9>& p_combination)
//-----------------------------------------------------------------------------
{
    connectToDatabase();
    QSqlQuery selectQuery(m_db);
    if(m_isFilterByCombination)
    {
        selectQuery.prepare("SELECT id FROM combination WHERE combination = :combination");
    }
    else
    {
        selectQuery.prepare("SELECT id FROM combination");
    }
    QString combination = "{";
    for(const std::string& symbol: p_combination)
    {
        combination += QString::fromStdString(symbol) + ",";
    }
    combination.chop(1); // Remove the last comma
    combination += "}";
    selectQuery.bindValue(":combination", combination);
    if(!selectQuery.exec())
    {
        qWarning() << "Failed to exec query: " << selectQuery.lastError().text();
    }
    
    std::vector<RaceData> fullDatas;
    while(selectQuery.next())
    {
        int combinationId = selectQuery.value("id").toInt();
        std::vector<RaceData> tmp = raceDataByCombinationId(combinationId);
        fullDatas.insert(
            fullDatas.end(),
            std::make_move_iterator(tmp.begin()),
            std::make_move_iterator(tmp.end())
        );
    }
    return fullDatas;
}

//-----------------------------------------------------------------------------
std::vector<RaceData> BDD::combinationData(const int& p_dataId, std::vector<int> &p_raceIdTreat)
//-----------------------------------------------------------------------------
{
    QSqlQuery selectQuery(m_db);
    selectQuery.prepare("SELECT race_id FROM race_en2en3 WHERE id = :dataId");
    selectQuery.bindValue(":dataId", p_dataId);
    if(selectQuery.exec())
    {
        std::vector<RaceData> dataRaces;
        while(selectQuery.next())
        {
            int raceId = selectQuery.value("race_id").toInt();
            if(!isRaceIdAlreadyTreat(p_raceIdTreat, raceId))
            {
                p_raceIdTreat.push_back(raceId);
                RaceData raceData = raceDatas(raceId);

                if(0 != raceData.id)
                {
                    combinationEn2En3(raceId, raceData);
                    dataRaces.push_back(raceData);
                }
            }
        }

        return dataRaces;
    }
    return {};
}

//-----------------------------------------------------------------------------
void BDD::combinationEn2En3(const int p_raceId, RaceData& p_raceData)
//-----------------------------------------------------------------------------
{
    QSqlQuery findId(m_db);
    findId.prepare("SELECT en2en3table_id FROM race_en2en3 WHERE race_id = :raceId");
    findId.bindValue(":raceId", p_raceId);
    if(findId.exec())
    {
        std::array<int, 3> en2En3Id;
        int index = 0;
        while(findId.next())
        {
            int en2En3_id = findId.value("en2en3table_id").toInt();
            en2En3Id[index] = en2En3_id;
            index++;
        }
        if(!en2En3Id.empty())
        {
            p_raceData.en2 = extractEn2En3Data(en2En3Id, "En2");
            p_raceData.en3 = extractEn2En3Data(en2En3Id, "En3");
            p_raceData.en2En3 = extractEn2En3Data(en2En3Id, "En2En3");
        }
    }
}

//-----------------------------------------------------------------------------
std::array<std::array<en2En3Struct, 16>, 9> BDD::extractEn2En3Data(const std::array<int, 3>& p_ids, const std::string p_type)
//-----------------------------------------------------------------------------
{
    std::array<std::array<en2En3Struct, 16>, 9> retDatas;
    QSqlQuery en2En3Datas(m_db);
    en2En3Datas.prepare("SELECT * FROM en2en3table WHERE (id = :id AND type = :type);");
    en2En3Datas.bindValue(":type", QString::fromStdString(p_type));
    for(int id: p_ids)
    {
        en2En3Datas.bindValue(":id", id);

        if(en2En3Datas.exec())
        {
            // Get data from table and set them into the return value
            while(en2En3Datas.next())
            {
                retDatas[0] = extractRowData(en2En3Datas.value("row1").toString());
                retDatas[1] = extractRowData(en2En3Datas.value("row2").toString());
                retDatas[2] = extractRowData(en2En3Datas.value("row3").toString());
                retDatas[3] = extractRowData(en2En3Datas.value("row4").toString());
                retDatas[4] = extractRowData(en2En3Datas.value("row5").toString());
                retDatas[5] = extractRowData(en2En3Datas.value("row6").toString());
                retDatas[6] = extractRowData(en2En3Datas.value("row7").toString());
                retDatas[7] = extractRowData(en2En3Datas.value("row8").toString());
                retDatas[8] = extractRowData(en2En3Datas.value("rowtotal_datas").toString(), true);
            }
        }
    }

    return retDatas;
}

//-----------------------------------------------------------------------------
std::array<en2En3Struct, 16> BDD::extractRowData(QString p_datas, const bool p_isTotal)
//-----------------------------------------------------------------------------
{
    std::array<en2En3Struct, 16> retDatas;
    QStringList value;
    QStringList color;
    if(!p_isTotal)
    {
        p_datas.remove("(\"{");
        p_datas.remove("}\")");
        QStringList valueColor = p_datas.split("}\",\"{");
        value = valueColor[0].split(",");
        color = valueColor[1].split(",");
        if(value.size() != color.size())
        {
            qWarning() << "Value and color are not in same number. Value size: " << QString::number(value.size())
            << ", color size: " << QString::number(color.size());
            return std::array<en2En3Struct, 16>{};
        }
    }
    else
    {
        p_datas.remove("{");
        p_datas.remove("}");
        value = p_datas.split(",");
    }

    en2En3Struct data;
    for(int index = 0; index < value.size(); ++index)
    {
        data.value = value[index].toInt();
        data.color = (false == p_isTotal)? color[index] : "Blank";
        retDatas[index] = data;
    }
    return retDatas;
}

//-----------------------------------------------------------------------------
RaceData BDD::raceDatas(const int& p_raceId)
//-----------------------------------------------------------------------------
{
    QSqlQuery selectQuery(m_db);
    selectQuery.prepare("SELECT * FROM race WHERE id = :id");
    selectQuery.bindValue(":id", p_raceId);
    if(selectQuery.exec())
    {
        RaceData raceData;
        while(selectQuery.next())
        {
            QString numbers = selectQuery.value("horses").toString();
            numbers.remove("{");
            numbers.remove("}");
            QStringList horses = numbers.split(",");
            for(int index = 0; index < horses.size(); ++ index)
            {
                raceData.numbers[index] = horses[index].toInt();
            }
            raceData.winner = selectQuery.value("winner").toInt();
            raceData.place1 = selectQuery.value("place1").toInt();
            raceData.place2 = selectQuery.value("place2").toInt();
            raceData.nbrLeaver = selectQuery.value("nbrleaver").toInt();
            raceData.distance = selectQuery.value("distance").toInt();
            raceData.discipline = selectQuery.value("discipline").toString().remove(" ");
            raceData.id = selectQuery.value("id").toInt();
        }
        return raceData;
    }
    return RaceData{};
}

//-----------------------------------------------------------------------------
bool BDD::isRaceIdAlreadyTreat(const std::vector<int>& p_raceIds, const int& p_raceId)
//-----------------------------------------------------------------------------
{
    for(int raceId: p_raceIds)
    {
        if(p_raceId == raceId)
        {
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
void BDD::closeConnection()
//-----------------------------------------------------------------------------
{
    m_db.close();
}

//-----------------------------------------------------------------------------
bool BDD::addDiscplineValue(const std::string& p_value)
//-----------------------------------------------------------------------------
{
    bool querySuccess = false;

    const QString connectionName = "programmeDB_alter";
    if(QSqlDatabase::contains(connectionName))
    {
        QSqlDatabase::removeDatabase(connectionName);
    }

    {
        QSqlDatabase tempDb = QSqlDatabase::addDatabase("QPSQL", connectionName);
        tempDb.setHostName("localhost");
        tempDb.setDatabaseName("programmeDB");
        tempDb.setUserName("postgres");
        tempDb.setPassword("");

        if(!tempDb.open())
        {
            qWarning() << "Failed to connect to the database:" << m_db.lastError().text();
            querySuccess = false;
        }
        else
        {
            // Check if the value already exists
            QString checkRequest = "SELECT 1 FROM pg_enum "
                                "JOIN pg_type ON pg_enum.enumtypid = pg_type.oid "
                                "WHERE pg_type.typname = 'ediscipline' AND pg_enum.enumlabel = :value";
            QSqlQuery checkQuery(tempDb);
            checkQuery.prepare(checkRequest);
            checkQuery.bindValue(":value", QString::fromStdString(p_value));
            if(checkQuery.exec() && checkQuery.next())
            {
                // Value already exists, no need to add it
                querySuccess = true;
            }
            else
            {
                QString request = "ALTER TYPE EDiscipline ADD VALUE '" + QString::fromStdString(p_value) + "'";
                QSqlQuery query(tempDb);
                querySuccess = query.exec(request);
                if(!querySuccess)
                {
                    qWarning() << "Failed to add discipline value:" << query.lastError().text();
                }
            }
        }

        tempDb.close();
    }
    QSqlDatabase::removeDatabase(connectionName);
    
    return querySuccess;
}

//-----------------------------------------------------------------------------
const int BDD::combinationId(const std::string& p_combination)
//-----------------------------------------------------------------------------
{
    connectToDatabase();
    QSqlQuery selectQuery(m_db);
    selectQuery.prepare("SELECT id FROM combination WHERE combination = :combination");
    selectQuery.bindValue(":combination", QString::fromStdString(p_combination));
    if(selectQuery.exec())
    {
        selectQuery.next();
        closeConnection();
        return selectQuery.value("id").toInt();
    }

    closeConnection();
    return -1;
}