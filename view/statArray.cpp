// Custom includes
#include "statArray.h"
#include "ui_statArray.h"

// Qt includes
#include <QStandardItemModel>
#include <QDebug>

namespace view
{
    //-------------------------------------------
    StatArray::StatArray(const QString p_filter1,
                         const QString p_filter2,
                         QWidget* p_parent)
        : QWidget { p_parent }
        , m_ui { new Ui::StatArray }
        , m_filter1 { p_filter1 }
        , m_filter2 { p_filter2 }
    //-------------------------------------------
    {
        m_ui->setupUi(this);
        m_ui->m_statGroupBox->setTitle(p_filter1);
        m_ui->m_changeStatBtn->setVisible(!(p_filter1.isEmpty() || p_filter2.isEmpty()));
        m_ui->m_changeStatBtn->setText(p_filter2);
        m_ui->m_statDatas->setMinimumHeight(275);

        connection();
    }

    //-------------------------------------------
    void StatArray::connection()
    //-------------------------------------------
    {
        connect(m_ui->m_start, &QPushButton::clicked, this, [this](){
                QItemSelectionModel* selectionModel = m_ui->m_statDatas->selectionModel();
                if(nullptr != selectionModel)
                {
                    emit startSubFilter(selectionModel->selectedIndexes());
                }
                else
                {
                    qWarning() << "Selection model is null";
                }
            });
        connect(m_ui->m_reset, &QPushButton::clicked, this, &StatArray::resetSubFilter);
        connect(m_ui->m_changeStatBtn, &QPushButton::clicked, this, &StatArray::changeStatFilter);
    }

    //-------------------------------------------
    StatArray::~StatArray()
    //-------------------------------------------
    {

    }

    //-------------------------------------------
    void StatArray::displayDatas(const StatisticsData& p_datasToDisplay)
    //-------------------------------------------
    {
        QItemSelectionModel* oldItemSelection = m_ui->m_statDatas->selectionModel();
        QList<QPair<int, int>> selectedCells;
        if(nullptr != oldItemSelection)
        {
            for(const QModelIndex& index: oldItemSelection->selectedIndexes())
            {
                selectedCells.append(QPair<int, int>(index.row(), index.column()));
            }
        }
        m_ui->m_total->setText(QString::number(p_datasToDisplay.total));

        const QStringList columnHeaders = {"P", "3", " ", "4", " ", "N", "T", "T",
                                     "P", "3", " ", "4", " ", "N", "T", "T",
                                     "P", "3", " ", "4", " ", "N", "T", "T"};

        QStandardItemModel* statModel = new QStandardItemModel(m_ui->m_statDatas);
        statModel->setHorizontalHeaderLabels(columnHeaders);

        QStringList rowHeaders;
        // Sets percents
        int rowIndex = 0;

        // O
        rowHeaders.push_back("O");
        setPercent(rowIndex,
                   p_datasToDisplay.arrayCircle,
                   p_datasToDisplay.total,
                   statModel);
        rowIndex++;

        // V%O
        rowHeaders.push_back("V\%O");
        setPercent(rowIndex,
                   p_datasToDisplay.arrayGreen,
                   p_datasToDisplay.arrayCircle,
                   statModel);
        rowIndex++;

        // Vert
        rowHeaders.push_back("Vert");
        setPercent(rowIndex,
                   p_datasToDisplay.arrayGreen,
                   p_datasToDisplay.totalGreen,
                   statModel);
        rowIndex++;

        // V gagnant
        rowHeaders.push_back("V gagnant");
        setPercent(rowIndex,
                   p_datasToDisplay.arrayGreenWinner,
                   p_datasToDisplay.arrayGreenWinnerTotal,
                   statModel);
        rowIndex++;

        // Bleu
        rowHeaders.push_back("Bleu");
        setPercent(rowIndex,
                   p_datasToDisplay.arrayBlue,
                   p_datasToDisplay.totalBlue,
                   statModel);
        rowIndex++;

        // Jaune
        rowHeaders.push_back("Jaune");
        setPercent(rowIndex,
                   p_datasToDisplay.arrayYellow,
                   p_datasToDisplay.totalYellow,
                   statModel);
        rowIndex++;

        // Orange
        rowHeaders.push_back("Orange");
        setPercent(rowIndex,
                   p_datasToDisplay.arrayOrange,
                   p_datasToDisplay.totalOrange,
                   statModel);
        rowIndex++;

        // Blanc
        rowHeaders.push_back("Blanc");
        setPercent(rowIndex,
                   p_datasToDisplay.arrayBlank,
                   p_datasToDisplay.totalBlank,
                   statModel);
        
        statModel->setVerticalHeaderLabels(rowHeaders);
        m_ui->m_statDatas->setModel(statModel);
        m_ui->m_statDatas->resizeColumnsToContents();

        QItemSelectionModel* newItemSelection = m_ui->m_statDatas->selectionModel();
        for(const QPair<int, int>& cell: selectedCells)
        {
            const int row = cell.first;
            const int column = cell.second;
            if((row < statModel->rowCount()) && (column < statModel->columnCount()))
            {
                QModelIndex index = statModel->index(row, column);
                newItemSelection->select(index, QItemSelectionModel::Select);
            }
        }
    }

    //-------------------------------------------
    void StatArray::changeFilterText()
    //-------------------------------------------
    {
        if(0 == m_filter1.compare(m_ui->m_changeStatBtn->text(), Qt::CaseInsensitive))
        {
            m_ui->m_changeStatBtn->setText(m_filter2);
            m_ui->m_statGroupBox->setTitle(m_filter1);
        }
        else
        {
            m_ui->m_changeStatBtn->setText(m_filter1);
            m_ui->m_statGroupBox->setTitle(m_filter2);
        }
    }

    //-------------------------------------------
    void StatArray::resetText()
    //-------------------------------------------
    {
        m_ui->m_statGroupBox->setTitle(m_filter1);
        m_ui->m_changeStatBtn->setText(m_filter2);
    }

    //-------------------------------------------
    void StatArray::setPercent(const int p_rowIndex,
                               const std::array<int, 24>& p_percents,
                               const int p_total,
                               QStandardItemModel* p_model)
    //-------------------------------------------
    {
        if(0 == p_total)
        {
            for(size_t colIndex = 0; colIndex < p_percents.size(); ++colIndex)
            {
                QStandardItem* percentItem = new QStandardItem("0");
                p_model->setItem(p_rowIndex, colIndex, percentItem);
            }
            return;
        }

        for(size_t colIndex = 0; colIndex < p_percents.size(); ++colIndex)
        {
            const float percent = p_percents[colIndex];
            QStandardItem* percentItem = new QStandardItem(QString::number((percent / p_total) * 100, 'f', 2));
            p_model->setItem(p_rowIndex, colIndex, percentItem);
        }
    }

    //-------------------------------------------
    void StatArray::setPercent(const int p_rowIndex,
                               const std::array<int, 24>& p_percents,
                               const std::array<int, 24>& p_totals,
                               QStandardItemModel* p_model)
    //-------------------------------------------
    {
        for(size_t colIndex = 0; colIndex < p_percents.size(); ++colIndex)
        {
            const float percent = p_percents[colIndex];
            const float total = p_totals[colIndex];
            QStandardItem* percentItem = nullptr;
            if(0 == total)
            {
                percentItem = new QStandardItem("0");
            }
            else
            {
                percentItem = new QStandardItem(QString::number((percent / total) * 100, 'f', 2));
            }
            p_model->setItem(p_rowIndex, colIndex, percentItem);
        }
    }
    
}