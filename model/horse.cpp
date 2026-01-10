// Custom includes
#include "horse.h"

//-----------------------------------------------------------------------------
Horse::Horse()
    : Horse(0) // Calls the other constructor with a default number of 0
//-----------------------------------------------------------------------------
{
    // No need to call init() here, as the member variables are already initialized
}

//---------------------------------------------------------------------------
Horse::Horse(const int p_number)
    : m_number{p_number}
    , m_typesValueEn2{}
    , m_typesValueEn3{}
    , m_typesValueEn2En3{}
    , m_rowTotalsEn2{}
    , m_rowTotalsEn3{}
    , m_rowTotalsEn2En3{}
//---------------------------------------------------------------------------
{
    init();
}

//---------------------------------------------------------------------------
Horse::Horse(const Horse& p_horse)
    : m_number{p_horse.m_number}
    , m_typesValueEn2{p_horse.m_typesValueEn2}
    , m_typesValueEn3{p_horse.m_typesValueEn3}
    , m_typesValueEn2En3{p_horse.m_typesValueEn2En3}
    , m_rowTotalsEn2{p_horse.m_rowTotalsEn2}
    , m_rowTotalsEn3{p_horse.m_rowTotalsEn3}
    , m_rowTotalsEn2En3{p_horse.m_rowTotalsEn2En3}
//---------------------------------------------------------------------------
{
    // No need to call init() here, as the member variables are already initialized
}

//---------------------------------------------------------------------------
void Horse::init()
//---------------------------------------------------------------------------
{
    m_typesValueEn2.clear();
    m_typesValueEn3.clear();
    m_typesValueEn2En3.clear();
    m_rowTotalsEn2.fill(0);
    m_rowTotalsEn3.fill(0);
    m_rowTotalsEn2En3.fill(0);
}

//---------------------------------------------------------------------------
void Horse::incrementTypesValue(const EType& p_type, const EValueType& p_value)
//---------------------------------------------------------------------------
{
    if (EValueType::En2 == p_value)
    {
        m_typesValueEn2[p_type]++;
        m_typesValueEn2En3[p_type]++;
        addIntoTotal(p_type, m_rowTotalsEn2);
        addIntoTotal(p_type, m_rowTotalsEn2En3);
    }
    else if (EValueType::En3 == p_value)
    {
        m_typesValueEn3[p_type]++;
        m_typesValueEn2En3[p_type]++;
        addIntoTotal(p_type, m_rowTotalsEn3);
        addIntoTotal(p_type, m_rowTotalsEn2En3);
    }
    else
    {
        qWarning() << "IncrementTypesValue, EValueType not treat: " << QString::number(p_value);
    }
}

//---------------------------------------------------------------------------
void Horse::addIntoTotal(const EType& p_type, std::array<int, 7>& p_totalRow)
//---------------------------------------------------------------------------
{
    switch (p_type)
    {
        case EType::isP:
            p_totalRow[0]++;
            p_totalRow[1]++;
            p_totalRow[2]++;
            p_totalRow[3]++;
            p_totalRow[4]++;
            p_totalRow[5]++;
            p_totalRow[6]++;
            break;
        case EType::is3:
            p_totalRow[1]++;
            p_totalRow[3]++;
            p_totalRow[5]++;
            break;
        case EType::is3Double:
            p_totalRow[0]++;
            p_totalRow[2]++;
            p_totalRow[4]++;
            p_totalRow[6]++;
            break;
        case EType::is4:
            p_totalRow[4]++;
            p_totalRow[5]++;
            break;
        case EType::is4Double:
            p_totalRow[2]++;
            p_totalRow[3]++;
            p_totalRow[6]++;
            break;
        case EType::isN:
            p_totalRow[6]++;
            break;
        default:
            qWarning() << "Unknown EType value: " << static_cast<int>(p_type);
    }
}