#
CC = g++
CFLAGS = -g -std=c++11  -Wall

yafeamake: YAFEAP.cpp BulkDataReader.cpp TableStorage.cpp
	$(CC) $(CFLAGS) -o yafeap YAFEAP.cpp BulkDataReader.cpp TableStorage.cpp 
