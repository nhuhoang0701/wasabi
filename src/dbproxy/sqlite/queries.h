#pragma once


const char* createTableSQL =	\
"CREATE TABLE Table1 (ColumnName0 TEXT, ColumnName1 TEXT, ColumnName2 REAL, ColumnName3 REAL);"
"INSERT INTO Table1(ColumnName0, ColumnName1, ColumnName2, ColumnName3) VALUES ('FR', 'Paris',12.0, 2000000.0);"
"INSERT INTO Table1(ColumnName0, ColumnName1, ColumnName2, ColumnName3) VALUES ('FR', 'Bordeaux',7, 3000000.0);"
\
"CREATE TABLE Table2 (ColumnName0 TEXT, ColumnName1 TEXT);"
"INSERT INTO Table2(ColumnName0, ColumnName1) VALUES ('FR', 'Paris');"
"INSERT INTO Table2(ColumnName0, ColumnName1) VALUES ('FR', 'Bordeaux');"
\

"CREATE TABLE Table3 (ColumnName0 TEXT, ColumnName1 TEXT, ColumnName2 REAL);"
"INSERT INTO Table3(ColumnName0, ColumnName1, ColumnName2) VALUES ('FR', 'Paris', 1);"
"INSERT INTO Table3(ColumnName0, ColumnName1, ColumnName2) VALUES ('FR', 'Bordeaux', 2);"\
;
