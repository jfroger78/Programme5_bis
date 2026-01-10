#!/bin/bash

# Variables de connexion
DB_NAME="programmeDB"
DB_USER="postgres"
SQL_FILE="init_db.sql"

echo "🔍 Vérification de l'existence de la base '$DB_NAME'..."

# Supprime la base si elle existe
if psql -U "$DB_USER" -tAc "SELECT 1 FROM pg_database WHERE datname = '$DB_NAME'" | grep -q 1; then
    echo "⚠️  La base '$DB_NAME' existe déjà. Suppression en cours..."
    dropdb -U "$DB_USER" "$DB_NAME"
    echo "🗑️  Base supprimée."
fi

# Crée la base
echo "📦 Création de la base '$DB_NAME'..."
createdb -U "$DB_USER" "$DB_NAME"
echo "✅ Base créée."

# Exécute le script SQL
echo "⚙️  Exécution du script SQL..."
psql -U "$DB_USER" -d "$DB_NAME" -f "$SQL_FILE"
echo "✅ Script exécuté avec succès."