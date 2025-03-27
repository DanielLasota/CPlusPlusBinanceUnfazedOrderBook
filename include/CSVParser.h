#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <vector>
#include <string>
#include "OrderBookEntry.h"

std::vector<OrderBookEntry> getOrderbookEntriesFromCSV(const std::string &csvPath);
std::vector<std::string> split(const std::string &line, char delimiter);

#endif
