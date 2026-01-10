// Custom includes
#include "course.h"

using namespace model;

//---------------------------------------------------
Course::Course(const std::array<int, 8>& p_horses,
               const int p_winner,
               const int p_place1,
               const int p_place2,
               const int p_nbrLeaver,
               const int p_distance,
               const QString p_discipline)
//---------------------------------------------------
    : m_horses(p_horses),
      m_winner(p_winner),
      m_place1(p_place1),
      m_place2(p_place2),
      m_nbrLeaver(p_nbrLeaver),
      m_distance(p_distance),
      m_discipline(p_discipline)
{
}

//---------------------------------------------------
Course::~Course() = default;
//---------------------------------------------------

