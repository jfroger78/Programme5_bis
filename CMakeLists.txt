cmake_minimum_required(VERSION 3.10)

project(Programme_cinco)

# Recherche Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Sql REQUIRED)

# Fichiers source
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/*.h")

# Exclusion du dossier "build"
list(FILTER SOURCES EXCLUDE REGEX ".*/build/.*")
list(FILTER HEADERS EXCLUDE REGEX ".*/build/.*")

# Fichiers ui
set(UI_FILES "*.ui")

# Convertir les fichiers .ui en fichiers .h
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Création de l'exécutable
add_executable(Programme_cinco ${SOURCES} ${HEADERS} ${UI_HEADERS})

# Ajout des dossiers d'include (racine + sous-dossiers)
target_include_directories(Programme_cinco PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/model
    ${CMAKE_SOURCE_DIR}/controller
    ${CMAKE_SOURCE_DIR}/view
)

# Lier Qt5 à ton projet
target_link_libraries(Programme_cinco Qt5::Widgets Qt5::Core Qt5::Sql)
