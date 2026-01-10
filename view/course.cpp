// Custom includes
#include "course.h"
#include "ui_course.h"

#include "controller/tools.h"

// Standard c++ includes
#include <unordered_map>

// Qt includes
#include <QDebug>

namespace view {
    //---------------------------------------------
    Course::Course(const std::string &p_courseName,
                QWidget *parent)
        : QWidget(parent)
        , m_ui(new Ui::Course)
        , m_courseName(p_courseName)
        , m_combination {}
    //---------------------------------------------
    {
        m_ui->setupUi(this);
        setupUi();
    }

    //---------------------------------------------
    Course::~Course()
    //---------------------------------------------
    {
        delete m_ui;
    }

    //---------------------------------------------
    const std::array<int, 8> Course::numbers() const
    //---------------------------------------------
    {
        std::array<int, 8> numbers;
        numbers.at(0) = m_ui->m_nbre1->text().toUInt();
        numbers.at(1) = m_ui->m_nbre2->text().toUInt();
        numbers.at(2) = m_ui->m_nbre3->text().toUInt();
        numbers.at(3) = m_ui->m_nbre4->text().toUInt();
        numbers.at(4) = m_ui->m_nbre5->text().toUInt();
        numbers.at(5) = m_ui->m_nbre6->text().toUInt();
        numbers.at(6) = m_ui->m_nbre7->text().toUInt();
        numbers.at(7) = m_ui->m_nbre8->text().toUInt();

        return numbers;
    }

    //---------------------------------------------
    std::string Course::name()
    //---------------------------------------------
    {
        return m_courseName;
    }

    //---------------------------------------------
    const int Course::winner() const
    //---------------------------------------------
    {
        return m_ui->m_winner->text().toInt();
    }

    //---------------------------------------------
    const int Course::place1() const
    //---------------------------------------------
    {
        return m_ui->m_place1->text().toInt();
    }

    //---------------------------------------------
    const int Course::place2() const
    //---------------------------------------------
    {
        return m_ui->m_place2->text().toInt();
    }

    //---------------------------------------------
    const QString Course::discipline() const
    //---------------------------------------------
    {
        return m_ui->m_discipline->text();
    }

    //---------------------------------------------
    const int Course::numberLeavers() const
    //---------------------------------------------
    {
        return m_ui->m_nbrePartant->text().toInt();
    }

    //---------------------------------------------
    const int Course::distance() const
    //---------------------------------------------
    {
        return m_ui->m_distance->text().toInt();
    }

    //---------------------------------------------
    void Course::connectIHM()
    //---------------------------------------------
    {
        connect(m_ui->m_nbre1, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_nbre2, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_nbre3, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_nbre4, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_nbre5, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_nbre6, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_nbre7, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_nbre8, &QLineEdit::textChanged, this, &Course::onCheckValue);

        connect(m_ui->m_winner, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_place1, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_place2, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_nbrePartant, &QLineEdit::textChanged, this, &Course::onCheckValue);

        connect(m_ui->m_start, &QPushButton::clicked, this, &Course::startClicked);
        connect(m_ui->m_clear, &QPushButton::clicked, this, &Course::clearClicked);

        // Filters
        connect(m_ui->m_nbrLeaverMin, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_nbrLeaverStrict, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_yellowMin, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_yellowStrict, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_filterNbr1, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_filterNbr2, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_filterNbr3, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_filterNbr4, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_filterNbr5, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_filterNbr6, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_filterNbr7, &QLineEdit::textChanged, this, &Course::onCheckValue);
        connect(m_ui->m_filterNbr8, &QLineEdit::textChanged, this, &Course::onCheckValue);
    }

    //---------------------------------------------
    void Course::setupUi()
    //---------------------------------------------
    {
        connectIHM();
        m_ui->m_distance->setValidator(new QIntValidator(1, 9999, this));
        m_ui->m_distanceFilter->setValidator(new QIntValidator(1, 9999, this));
    }

    //---------------------------------------------
    void Course::onCheckValue(const QString &p_value)
    //---------------------------------------------
    {
        QLineEdit* sender = qobject_cast<QLineEdit*>(QObject::sender());
        if(!sender)
        {
            return;
        }

        if (p_value.toInt() < MIN_HORSE_NUMBER || p_value.toInt() > MAX_HORSE_NUMBER)
        {
            sender->blockSignals(true);
            sender->setText(p_value.left(p_value.length() - 1));
            sender->blockSignals(false);
        }
    }

    //------------------------------------------------------------------------------------------------------------------------
    void Course::fillEn2En3(const std::array<Horse*, 8>& p_horses,
                            const std::array<int, 16>& p_totalsEn2,
                            const std::array<int, 16>& p_totalsEn3,
                            const std::array<int, 16>& p_totalsEn2En3)
    //------------------------------------------------------------------------------------------------------------------------
    {
        QStandardItemModel* modelEn2 = new QStandardItemModel(m_ui->m_en2);
        QStandardItemModel* modelEn3 = new QStandardItemModel(m_ui->m_en3);
        QStandardItemModel* modelEn2En3 = new QStandardItemModel(m_ui->m_en2En3);
        QStringList rowHeader;

        for(int horseIndex = 0; horseIndex < p_horses.size(); ++horseIndex)
        {
            
            Horse* currentHorse = p_horses[horseIndex];
            rowHeader.push_back(QString::number(currentHorse->number()));
            for(int typeIndex = EType::TypeNone + 1;
                typeIndex < EType::TypeCount;
                ++typeIndex)
            {
                EType type = static_cast<EType>(typeIndex);
                if((EType::isP == type) || (EType::is3 == type) || (EType::is3Double == type))
                {
                    QStandardItem* itemEn2 = new QStandardItem(QString::number(currentHorse->typesValueEn2().find(type)->second));
                    modelEn2->setItem(horseIndex, typeIndex - 1, itemEn2);
                    QStandardItem* itemEn3 = new QStandardItem(QString::number(currentHorse->typesValueEn3().find(type)->second));
                    modelEn3->setItem(horseIndex, typeIndex - 1, itemEn3);
                    QStandardItem* itemEn2En3 = new QStandardItem(QString::number(currentHorse->typesValueEn2En3().find(type)->second));
                    modelEn2En3->setItem(horseIndex, typeIndex - 1, itemEn2En3);
                }
                QStandardItem* itemEn2_2 = new QStandardItem(QString::number(currentHorse->typesValueEn2().find(type)->second));
                modelEn2->setItem(horseIndex, typeIndex + 4, itemEn2_2);
                QStandardItem* itemEn3_3 = new QStandardItem(QString::number(currentHorse->typesValueEn3().find(type)->second));
                modelEn3->setItem(horseIndex, typeIndex + 4, itemEn3_3);
                QStandardItem* itemEn2En3_2 = new QStandardItem(QString::number(currentHorse->typesValueEn2En3().find(type)->second));
                modelEn2En3->setItem(horseIndex, typeIndex + 4, itemEn2En3_2);
            }
            
            int offset = 3;
            for(int index = 0; index < 7; ++index)
            {
                if(2 == index)
                {
                    offset = 9;
                }
                QStandardItem* totalLineEn2 = new QStandardItem(QString::number(currentHorse->rowTotalsEn2()[index]));
                modelEn2->setItem(horseIndex, index + offset, totalLineEn2);
                QStandardItem* totalLineEn3 = new QStandardItem(QString::number(currentHorse->rowTotalsEn3()[index]));
                modelEn3->setItem(horseIndex, index + offset, totalLineEn3);
                QStandardItem* totalLineEn2En3 = new QStandardItem(QString::number(currentHorse->rowTotalsEn2En3()[index]));
                modelEn2En3->setItem(horseIndex, index + offset, totalLineEn2En3);
            }
        }
        
        rowHeader.push_back("T");
        writeColumnTotal(p_totalsEn2, p_horses.size(), modelEn2);
        writeColumnTotal(p_totalsEn3, p_horses.size(), modelEn3);
        writeColumnTotal(p_totalsEn2En3, p_horses.size(), modelEn2En3);

        modelEn2->setVerticalHeaderLabels(rowHeader);
        modelEn3->setVerticalHeaderLabels(rowHeader);
        modelEn2En3->setVerticalHeaderLabels(rowHeader);

        createTypeHeader(modelEn2);
        createTypeHeader(modelEn3);
        createTypeHeader(modelEn2En3);

        colorizeArray(modelEn2);
        colorizeArray(modelEn3);
        colorizeArray(modelEn2En3);

        m_ui->m_en2->setModel(modelEn2);
        m_ui->m_en2->resizeColumnsToContents();
        m_ui->m_en3->setModel(modelEn3);
        m_ui->m_en3->resizeColumnsToContents();
        m_ui->m_en2En3->setModel(modelEn2En3);
        m_ui->m_en2En3->resizeColumnsToContents();
    }
    
    //------------------------------------------------------------------------------------------------------------------------
    void Course::fillCombination(const Combination& p_combination,
                                const Totals& p_totals)
    //------------------------------------------------------------------------------------------------------------------------
    {
        QStringList header;
        m_combination = p_combination;
        for(int index = 0; index < 24; ++index)
        {
            header.push_back("");
        }
        for(int index = EType::TypeNone + 1; index < EType::TypeCount; ++index)
        {
            std::string typeStr = Tools::convertTypeToString(static_cast<EType>(index));
            header.replace(index - 1, QString::fromStdString(typeStr));
            header.replace(index + 7, QString::fromStdString(typeStr));
            header.replace(index + 15, QString::fromStdString(typeStr));
        }
        header.replace(6, "T");
        header.replace(7, "T");
        header.replace(14, "T");
        header.replace(15, "T");
        header.replace(22, "T");
        header.replace(23, "T");

        QStandardItemModel* combination = new QStandardItemModel();
        combination->setHorizontalHeaderLabels(header);

        displayCombinationBasedValues(p_totals, combination);
        for(size_t index = 0; index < p_totals.en2.size(); ++index)
        {
            m_totalRow[index] = p_totals.en2[index];
            m_totalRow[index + 16] = p_totals.en3[index];
            m_totalRow[index + 32] = p_totals.en2En3[index];
        }
        displayCombination(m_combination, combination);
        

        m_ui->m_combination->setModel(combination);
        m_ui->m_combination->resizeColumnsToContents();
    }

    //------------------------------------------------------------------------------------------------------------------------
    void Course::fillCourses(const std::array<int, 8>& p_numbers,
                            const int& p_winner,
                            const int& p_place1,
                            const int& p_place2,
                            const QString& p_discipline,
                            const int& p_numberLeavers,
                            const int& p_distance)
    //------------------------------------------------------------------------------------------------------------------------
    {
        m_ui->m_nbre1->setText(QString::number(p_numbers[0]));
        m_ui->m_nbre2->setText(QString::number(p_numbers[1]));
        m_ui->m_nbre3->setText(QString::number(p_numbers[2]));
        m_ui->m_nbre4->setText(QString::number(p_numbers[3]));
        m_ui->m_nbre5->setText(QString::number(p_numbers[4]));
        m_ui->m_nbre6->setText(QString::number(p_numbers[5]));
        m_ui->m_nbre7->setText(QString::number(p_numbers[6]));
        m_ui->m_nbre8->setText(QString::number(p_numbers[7]));

        m_ui->m_winner->setText(QString::number(p_winner));
        m_ui->m_place1->setText(QString::number(p_place1));
        m_ui->m_place2->setText(QString::number(p_place2));

        m_ui->m_discipline->setText(p_discipline);
        m_ui->m_nbrePartant->setText(QString::number(p_numberLeavers));
        m_ui->m_distance->setText(QString::number(p_distance));
    }

    //------------------------------------------------------------------------------------------------------------------------
    const QString Course::letterStrict() const
    //------------------------------------------------------------------------------------------------------------------------
    {
        return m_ui->m_letterStrict->text();
    }

    //------------------------------------------------------------------------------------------------------------------------
    const QString Course::letterMin() const
    //------------------------------------------------------------------------------------------------------------------------
    {
        return m_ui->m_letterMin->text();
    }

    //------------------------------------------------------------------------------------------------------------------------
    const int Course::nbrLeaverStrict() const
    //------------------------------------------------------------------------------------------------------------------------
    {
        return m_ui->m_nbrLeaverStrict->text().toInt();
    }

    //------------------------------------------------------------------------------------------------------------------------
    const int Course::nbrLeaverMin() const
    //------------------------------------------------------------------------------------------------------------------------
    {
        return m_ui->m_nbrLeaverMin->text().toInt();
    }

    //------------------------------------------------------------------------------------------------------------------------
    const int Course::yellowStrict() const
    //------------------------------------------------------------------------------------------------------------------------
    {
        return m_ui->m_yellowStrict->text().toInt();
    }

    //------------------------------------------------------------------------------------------------------------------------
    const int Course::yellowMin() const
    //------------------------------------------------------------------------------------------------------------------------
    {
        return m_ui->m_yellowMin->text().toInt();
    }

    //------------------------------------------------------------------------------------------------------------------------
    const int Course::distanceFilter() const
    //------------------------------------------------------------------------------------------------------------------------
    {
        return m_ui->m_distanceFilter->text().toInt();
    }

    //------------------------------------------------------------------------------------------------------------------------
    const std::array<int, 8> Course::horsesFilter() const
    //------------------------------------------------------------------------------------------------------------------------
    {
        return {
            m_ui->m_filterNbr1->text().toInt(),
            m_ui->m_filterNbr2->text().toInt(),
            m_ui->m_filterNbr3->text().toInt(),
            m_ui->m_filterNbr4->text().toInt(),
            m_ui->m_filterNbr5->text().toInt(),
            m_ui->m_filterNbr6->text().toInt(),
            m_ui->m_filterNbr7->text().toInt(),
            m_ui->m_filterNbr8->text().toInt()
        };
    }

    //------------------------------------------------------------------------------------------------------------------------
    void Course::writeColumnTotal(const std::array<int, 16>& p_totals,
                                const int p_row,
                                QStandardItemModel* p_model)
    //------------------------------------------------------------------------------------------------------------------------
    {
        for(int index = 0; index < p_totals.size(); ++index)
        {
            QStandardItem* item = new QStandardItem(QString::number(p_totals[index]));
            p_model->setItem(p_row, index, item);
        }
    }

    //------------------------------------------------------------------------------------------------------------------------
    void Course::createTypeHeader(QStandardItemModel* p_model)
    //------------------------------------------------------------------------------------------------------------------------
    {
        QStringList typeHeader;
        typeHeader.insert(0, "P");
        typeHeader.insert(1, "3");
        typeHeader.insert(2, "");
        typeHeader.insert(3, "T");
        typeHeader.insert(4, "T");
        typeHeader.insert(5, "P");
        typeHeader.insert(6, "3");
        typeHeader.insert(7, "");
        typeHeader.insert(8, "4");
        typeHeader.insert(9, "");
        typeHeader.insert(10, "N");
        typeHeader.insert(11, "T");
        typeHeader.insert(12, "T");
        typeHeader.insert(13, "T");
        typeHeader.insert(14, "T");
        typeHeader.insert(15, "T");
        p_model->setHorizontalHeaderLabels(typeHeader);
    }

    //------------------------------------------------------------------------------------------------------------------------
    void Course::colorizeArray(QStandardItemModel* p_model)
    //------------------------------------------------------------------------------------------------------------------------
    {
        bool valueZeroTreat = false;
        for(int column = 0; column < p_model->columnCount(); ++column)
        {
            valueZeroTreat = false;
            std::array<uint16_t, 8> columnData = {0, 0, 0, 0, 0, 0, 0};
            for(int row = 0; row < p_model->rowCount(); ++row)
            {
                QStandardItem* item = p_model->item(row, column);
                if(nullptr != item)
                {
                    columnData[row] = item->text().toUInt();
                }
            }
            columnData = Tools::removeDuplicatesAndSortDesc(columnData);
            for(int dataIndex = 0; dataIndex < columnData.size(); ++dataIndex)
            {
                if((0 == columnData[dataIndex]) && (false == valueZeroTreat))
                {
                    valueZeroTreat = true;
                }
                else if(0 == columnData[dataIndex] && valueZeroTreat)
                {
                    continue;
                }
                QColor color;
                switch(dataIndex)
                {
                    case 0:
                        color = QColor(QString::fromStdString(GREEN_COLOR));
                        break;
                    case 1:
                        color = QColor(QString::fromStdString(BLUE_COLOR));
                        break;
                    case 2:
                        color = QColor(QString::fromStdString(YELLOW_COLOR));
                        break;
                    case 3:
                        color = QColor(QString::fromStdString(ORANGE_COLOR));
                        break;
                    default:
                        continue; // Skip if dataIndex is greater than 3
                }
                for(int row = 0; row < p_model->rowCount(); ++row)
                {
                    QStandardItem* item = p_model->item(row, column);
                    if(nullptr != item && (columnData[dataIndex] == item->text().toUInt()))
                    {
                        item->setBackground(QBrush(color));
                    }
                }
            }
        }
    }

    //------------------------------------------------------------------------------------------------------------------------
    void Course::clear()
    //------------------------------------------------------------------------------------------------------------------------
    {
        m_ui->m_en2->setModel(new QStandardItemModel());
        m_ui->m_en3->setModel(new QStandardItemModel());
        m_ui->m_en2En3->setModel(new QStandardItemModel());

        m_ui->m_combination->setModel(new QStandardItemModel());

        m_ui->m_nbre1->clear();
        m_ui->m_nbre2->clear();
        m_ui->m_nbre3->clear();
        m_ui->m_nbre4->clear();
        m_ui->m_nbre5->clear();
        m_ui->m_nbre6->clear();
        m_ui->m_nbre7->clear();
        m_ui->m_nbre8->clear();

        m_ui->m_nbrePartant->clear();
        m_ui->m_distance->clear();
        m_ui->m_discipline->clear(); // Reset to default discipline
        
        m_ui->m_winner->clear();
        m_ui->m_place1->clear();
        m_ui->m_place2->clear();
    }

    //------------------------------------------------------------------------------------------------------------------------
    bool Course::isValid()
    //------------------------------------------------------------------------------------------------------------------------
    {
        for(int number: numbers())
        {
            if(0 == number)
            {
                return false;
            }
        }
        return true;
    }

    //------------------------------------------------------------------------------------------------------------------------
    void Course::displayCombinationBasedValues(const Totals& p_totals,
                                            QStandardItemModel* p_model)
    //------------------------------------------------------------------------------------------------------------------------
    {
        displayOneTypeValue(0, 5, p_totals, p_model); // P
        displayOneTypeValue(1, 7, p_totals, p_model); // 3
        displayOneTypeValue(2, 6, p_totals, p_model); // 3 double
        displayOneTypeValue(3, 8, p_totals, p_model); // 4 double
        displayOneTypeValue(4, 9, p_totals, p_model); // 4
        displayOneTypeValue(5, 10, p_totals, p_model); // N
        displayOneTypeValue(6, 4, p_totals, p_model); // Total
        displayOneTypeValue(7, 14, p_totals, p_model); // Total
    }

    //------------------------------------------------------------------------------------------------------------------------
    void Course::displayOneTypeValue(const int p_columnIndex,
                                    const int p_valueIndex,
                                    const Totals& p_totals,
                                    QStandardItemModel* p_model)
    //------------------------------------------------------------------------------------------------------------------------
    {
        QStandardItem* en2Item = new QStandardItem(QString::number(p_totals.en2[p_valueIndex]));
        p_model->setItem(0, p_columnIndex, en2Item);
        QStandardItem* en3Item = new QStandardItem(QString::number(p_totals.en3[p_valueIndex]));
        p_model->setItem(0, p_columnIndex + 8, en3Item);
        QStandardItem* en2En3Item = new QStandardItem(QString::number(p_totals.en2En3[p_valueIndex]));
        p_model->setItem(0, p_columnIndex + 16, en2En3Item);
    }

    //------------------------------------------------------------------------------------------------------------------------
    void Course::displayCombination(const Combination& p_combination,
                                    QStandardItemModel* p_model)
    //------------------------------------------------------------------------------------------------------------------------
    {
        for(int index = 0; index < 3; ++index)
        {
            QStandardItem* en2Item = new QStandardItem(QString::fromStdString(std::string(1, p_combination.en2[index])));
            p_model->setItem(1, index, en2Item);
            QStandardItem* en3Item = new QStandardItem(QString::fromStdString(std::string(1, p_combination.en3[index])));
            p_model->setItem(1, index + 8, en3Item);
            QStandardItem* en2En3Item = new QStandardItem(QString::fromStdString(std::string(1, p_combination.en2En3[index])));
            p_model->setItem(1, index + 16, en2En3Item);
        }
    }

    //------------------------------------------------------------------------------------------------------------------------
    QAbstractItemModel* Course::modelEn2() const
    //------------------------------------------------------------------------------------------------------------------------
    {
        return m_ui->m_en2->model();
    }

    //------------------------------------------------------------------------------------------------------------------------
    QAbstractItemModel* Course::modelEn3() const
    //------------------------------------------------------------------------------------------------------------------------
    {
        return m_ui->m_en3->model();
    }

    //------------------------------------------------------------------------------------------------------------------------
    QAbstractItemModel* Course::modelEn2En3() const
    //------------------------------------------------------------------------------------------------------------------------
    {
        return m_ui->m_en2En3->model();
    }

    //------------------------------------------------------------------------------------------------------------------------
    const std::array<std::string, 9> Course::combinationString() const
    //------------------------------------------------------------------------------------------------------------------------
    {
        return {
            std::string(1, m_combination.en2[0]),
            std::string(1, m_combination.en2[1]),
            std::string(1, m_combination.en2[2]),
            std::string(1, m_combination.en3[0]),
            std::string(1, m_combination.en3[1]),
            std::string(1, m_combination.en3[2]),
            std::string(1, m_combination.en2En3[0]),
            std::string(1, m_combination.en2En3[1]),
            std::string(1, m_combination.en2En3[2])
        };
    }

    //------------------------------------------------------------------------------------------------------------------------
    const bool Course::isFilterByCombination() const
    //------------------------------------------------------------------------------------------------------------------------
    {
        return m_ui->m_combFilter->isChecked();
    }

    //------------------------------------------------------------------------------------------------------------------------
    void Course::addResultArray(QWidget* p_resultArray,
                                const int p_row,
                                const int p_column,
                                const int p_rowSpan,
                                const int p_colSpan)
    //------------------------------------------------------------------------------------------------------------------------
    {
        static_cast<QGridLayout*>(m_ui->m_resultats->layout())->addWidget(p_resultArray, p_row, p_column, p_rowSpan, p_colSpan);
    }

}