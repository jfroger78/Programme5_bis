// Custom includes
#include "en2en3.h"
#include "tools.h"

// Qt includes
#include <QDebug>

//-----------------------------------------------
En2En3::En2En3(const std::array<int, 8> p_horses)
//-----------------------------------------------
{
    initHorses(p_horses);
    en2En3Computation();
    computeColumnsTotal();
}

//-----------------------------------------------
En2En3::~En2En3()
//-----------------------------------------------
{
    // Destructor implementation
}

//-----------------------------------------------
void En2En3::initHorses(const std::array<int, 8>& p_horsesNumbers)
//-----------------------------------------------
{
    for(int horseIndex = 0; horseIndex < p_horsesNumbers.size(); ++horseIndex)
    {
        Horse* horse = new Horse(p_horsesNumbers[horseIndex]);
        m_horses[horseIndex] = horse;
    }
    m_columnsTotalEn2.fill(0);
    m_columnsTotalEn3.fill(0);
    m_columnsTotalEn2En3.fill(0);
}

//-----------------------------------------------
void En2En3::en2En3Computation()
//-----------------------------------------------
{
    //0-1-2-3
    std::array<Horse*, 4> currentArray;
    currentArray[0] = m_horses[0];
    currentArray[1] = m_horses[1];
    currentArray[2] = m_horses[2];
    currentArray[3] = m_horses[3];
    computeValue(currentArray);
    //0-1-4-5
    currentArray[2] = m_horses[4];
    currentArray[3] = m_horses[5];
    computeValue(currentArray);
    //0-1-6-7
    currentArray[2] = m_horses[6];
    currentArray[3] = m_horses[7];
    computeValue(currentArray);
    //2-3-4-5
    currentArray[0] = m_horses[2];
    currentArray[1] = m_horses[3];
    currentArray[2] = m_horses[4];
    currentArray[3] = m_horses[5];
    computeValue(currentArray);
    //2-3-6-7
    currentArray[2] = m_horses[6];
    currentArray[3] = m_horses[7];
    computeValue(currentArray);
    //4-5-6-7
    currentArray[0] = m_horses[4];
    currentArray[1] = m_horses[5];
    computeValue(currentArray);
}

//-----------------------------------------------
void En2En3::computeValue(std::array<Horse*, 4>& p_horses)
//-----------------------------------------------
{
    //0-1-2-3
    computeValueEn2(p_horses[0],
                    p_horses[1]);
    computeValueEn3(p_horses[0],
                    p_horses[1],
                    p_horses[2],
                    p_horses[3]);
    //0-2-1-3
    computeValueEn2(p_horses[0],
                    p_horses[2]);
    computeValueEn3(p_horses[0],
                    p_horses[2],
                    p_horses[1],
                    p_horses[3]);
    //0-3-1-2
    computeValueEn2(p_horses[0],
                    p_horses[3]);
    computeValueEn3(p_horses[0],
                    p_horses[3],
                    p_horses[1],
                    p_horses[2]);
    //1-2-3-0
    computeValueEn2(p_horses[1],
                    p_horses[2]);
    computeValueEn3(p_horses[1],
                    p_horses[2],
                    p_horses[3],
                    p_horses[0]);
    //1-3-0-2
    computeValueEn2(p_horses[1],
                    p_horses[3]);
    computeValueEn3(p_horses[1],
                    p_horses[3],
                    p_horses[0],
                    p_horses[2]);
    //2-3-0-1
    computeValueEn2(p_horses[2],
                    p_horses[3]);
    computeValueEn3(p_horses[2],
                    p_horses[3],
                    p_horses[0],
                    p_horses[1]);
}

//-----------------------------------------------
void En2En3::computeValueEn2(Horse* p_horse1,
                             Horse* p_horse2)
//-----------------------------------------------
{
    if((nullptr == p_horse1) || (nullptr == p_horse2))
    {
        qWarning() << "ComputeValueEn2 one horse is nullptr";
        return;
    }
    int sum = p_horse1->number() + p_horse2->number();
    std::vector<EType> typeEn2 = Tools::type(sum);
    for(const EType& type: typeEn2)
    {
        p_horse1->incrementTypesValue(type, EValueType::En2);
        p_horse2->incrementTypesValue(type, EValueType::En2);
    }
}

//-----------------------------------------------
void En2En3::computeValueEn3(Horse* p_horse1,
                             Horse* p_horse2,
                             Horse* p_horse3,
                             Horse* p_horse4)
//-----------------------------------------------
{
    if((nullptr == p_horse1) || (nullptr == p_horse2) || (nullptr == p_horse3) || (nullptr == p_horse4))
    {
        qWarning() << "ComputeValueEn3, at least, one horse is nullptr";
        return;
    }

    int sum = p_horse1->number() + p_horse2->number();
    int sumEn3 = sum + p_horse3->number();
    std::vector<EType> typesEn3 = Tools::type(sumEn3);
    for(const EType& type: typesEn3)
    {
        p_horse3->incrementTypesValue(type, EValueType::En3);
    }

    sumEn3 = sum + p_horse4->number();
    typesEn3.clear();
    typesEn3 = Tools::type(sumEn3);
    for(const EType& type: typesEn3)
    {
        p_horse4->incrementTypesValue(type, EValueType::En3);
    }
}

//-----------------------------------------------
void En2En3::computeColumnsTotal()
//-----------------------------------------------
{
    EType type = EType::TypeNone;
    int en2Value = 0;
    int en3Value = 0;
    int en2En3Value = 0;
    for(Horse* horse: m_horses)
    {
        for(int typeIndex = EType::TypeNone + 1; typeIndex != EType::TypeCount; ++typeIndex)
        {
            type = static_cast<EType>(typeIndex);
            en2Value = 0;
            en3Value = 0;
            en2En3Value = 0;
            if(horse->typesValueEn2().end() != horse->typesValueEn2().find(type))
            {
                en2Value = horse->typesValueEn2().find(type)->second;
            }
            if(horse->typesValueEn3().end() != horse->typesValueEn3().find(type))
            {
                en3Value = horse->typesValueEn3().find(type)->second;
            }
            if(horse->typesValueEn2En3().end() != horse->typesValueEn2En3().find(type))
            {
                en2En3Value = horse->typesValueEn2En3().find(type)->second;
            }
            if((EType::isP == type) || (EType::is3Double == type) || (EType::is3 == type))
            {
                m_columnsTotalEn2[typeIndex - 1] += en2Value;
                m_columnsTotalEn3[typeIndex -1] +=  en3Value;
                m_columnsTotalEn2En3[typeIndex -1] += en2En3Value;
            }
            
            m_columnsTotalEn2[typeIndex + 4] += en2Value;
            m_columnsTotalEn3[typeIndex + 4] += en3Value;
            m_columnsTotalEn2En3[typeIndex + 4] += en2En3Value;
        }
        m_columnsTotalEn2[3] += horse->rowTotalsEn2()[0];
        m_columnsTotalEn2[4] += horse->rowTotalsEn2()[1];
        m_columnsTotalEn2[11] += horse->rowTotalsEn2()[2];
        m_columnsTotalEn2[12] += horse->rowTotalsEn2()[3];
        m_columnsTotalEn2[13] += horse->rowTotalsEn2()[4];
        m_columnsTotalEn2[14] += horse->rowTotalsEn2()[5];
        m_columnsTotalEn2[15] += horse->rowTotalsEn2()[6];

        m_columnsTotalEn3[3] += horse->rowTotalsEn3()[0];
        m_columnsTotalEn3[4] += horse->rowTotalsEn3()[1];
        m_columnsTotalEn3[11] += horse->rowTotalsEn3()[2];
        m_columnsTotalEn3[12] += horse->rowTotalsEn3()[3];
        m_columnsTotalEn3[13] += horse->rowTotalsEn3()[4];
        m_columnsTotalEn3[14] += horse->rowTotalsEn3()[5];
        m_columnsTotalEn3[15] += horse->rowTotalsEn3()[6];

        m_columnsTotalEn2En3[3] += horse->rowTotalsEn2En3()[0];
        m_columnsTotalEn2En3[4] += horse->rowTotalsEn2En3()[1];
        m_columnsTotalEn2En3[11] += horse->rowTotalsEn2En3()[2];
        m_columnsTotalEn2En3[12] += horse->rowTotalsEn2En3()[3];
        m_columnsTotalEn2En3[13] += horse->rowTotalsEn2En3()[4];
        m_columnsTotalEn2En3[14] += horse->rowTotalsEn2En3()[5];
        m_columnsTotalEn2En3[15] += horse->rowTotalsEn2En3()[6];
    }
}