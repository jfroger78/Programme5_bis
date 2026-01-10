-- Create enum if not exists
DO $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'EDiscipline') THEN
        CREATE TYPE EDiscipline AS ENUM ('T', 'TM', 'T3', 'P', 'PH', 'OBHA', 'OBS', 'OBHHA', 'OBHS');
    END IF;
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'EColor') THEN
        CREATE TYPE EColor AS ENUM ('Green', 'Blue', 'Orange', 'Yellow', 'Blank', 'None');
    END IF;
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'rowdata') THEN
        CREATE TYPE rowData AS (
            datas INTEGER[16],
            color EColor[16]
        );
    END IF;
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'EDataType') THEN
        CREATE TYPE EDataType AS ENUM ('En2', 'En3', 'En2En3');
    END IF;
END$$;

-- Table "en2En3Table"
CREATE TABLE IF NOT EXISTS en2En3Table (
    id SERIAL PRIMARY KEY,
    type eDataType NOT NULL,
    row1 rowData NOT NULL,
    row2 rowData NOT NULL,
    row3 rowData NOT NULL,
    row4 rowData NOT NULL,
    row5 rowData NOT NULL,
    row6 rowData NOT NULL,
    row7 rowData NOT NULL,
    row8 rowData NOT NULL,
    rowTotal_datas INTEGER[16] NOT NULL
);

-- Table "race"
CREATE TABLE IF NOT EXISTS race (
    id SERIAL PRIMARY KEY,
    horses INTEGER[8] NOT NULL,
    winner INTEGER NOT NULL,
    place1 INTEGER NOT NULL,
    place2 INTEGER NOT NULL,
    nbrLeaver INTEGER NOT NULL,
    distance INTEGER NOT NULL,
    discipline CHAR(256) NOT NULL,
    CHECK (array_length(horses, 1) = 8)
);

-- Table "combination"
CREATE TABLE IF NOT EXISTS combination(
    id SERIAL PRIMARY KEY,
    combination CHAR[9] NOT NULL
);

-- Table de jointure
CREATE TABLE IF NOT EXISTS race_en2En3 (
    id SERIAL PRIMARY KEY,
    race_id INTEGER REFERENCES race(id) ON DELETE CASCADE,
    en2En3Table_id INTEGER REFERENCES en2En3Table(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS combination_data (
    combination_id INTEGER REFERENCES combination(id) ON DELETE CASCADE,
    data_id INTEGER REFERENCES race_en2En3(id) ON DELETE CASCADE,
    PRIMARY KEY (combination_id, data_id)
)
