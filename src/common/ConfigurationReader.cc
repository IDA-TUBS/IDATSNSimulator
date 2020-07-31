//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "../common/ConfigurationReader.h"

// Singletion design pattern: initialize instance, is set to 0 until the first function call
ConfigurationReader* ConfigurationReader::instance = 0;

ConfigurationReader::~ConfigurationReader()
{
    // TODO Auto-generated destructor stub
}

ConfigurationReader* ConfigurationReader::getInstance()
{
    if(instance == 0)
    {
        instance = new ConfigurationReader();
    }
    return instance;
}

configVector ConfigurationReader::getMACTableEntriesFromInputFile(std::string inputConfigurationFile)
{
    if(this->macTableVector.empty() == true)
    {
        openConfigurationFile(inputConfigurationFile);
        this->macTableVector = readOutInputFile();
        this->inputFile.close();
        return this->macTableVector;
    }
    else
    {
        return this->macTableVector;
    }
}

configVector ConfigurationReader::getAVBTableEntriesFromInputFile(std::string inputConfigurationFile)
{
    if(this->avbTableVector.empty() == true)
    {
        openConfigurationFile(inputConfigurationFile);
        this->avbTableVector = readOutInputFile();
        this->inputFile.close();
        return this->avbTableVector;
    }
    else
    {
        return this->avbTableVector;
    }
}

void ConfigurationReader::openConfigurationFile(std::string inputConfigurationFile)
{
    this->inputFile.open(inputConfigurationFile, std::ios::in);

    if(!this->inputFile.is_open())
    {
        throw cRuntimeError("Cannot open file. Check if the file exists.");
    }
}

configVector ConfigurationReader::readOutInputFile()
{
    configVector configurationVec;

    while(std::getline(this->inputFile, this->fileLine))
    {
        // check if the reading line corresponds to a comment line
        if(!isCommentLine(this->fileLine) || this ->fileLine.empty())
        {
            // it is a comment line -> ignore (skip) it and get the next line
            continue;
        }
        std::vector<int> intVector = splitFileLineAndGetEntry();
        configurationVec.push_back(intVector);
    }
    return configurationVec;
}


bool ConfigurationReader::isCommentLine(std::string line)
{
    // comments are marked by '#' character and are always at the first position = 0
    bool isComment = line.find_first_of(COMMENT_SYMBOL, COMMENT_POSITION);
    return isComment;
}



std::vector<int> ConfigurationReader::splitFileLineAndGetEntry()
{
    // preparation for breaking words in line
    std::stringstream splitLine(this->fileLine);

    // only one word of the fileLine
    std::string entry;

    // words are stored in vector -> strings has to be converted to int
    std::vector<int> MACTableColumnEntries;

    // every entry is separated by ',' -> get and store it
    while(std::getline(splitLine, entry, ','))
    {
        // convert string entry to integer
        int integerValue = std::stoi(entry);

        // store it in vector
        MACTableColumnEntries.push_back(integerValue);
     }
    return MACTableColumnEntries;
}

ConfigurationReader::ConfigurationReader()
{
    // Singleton design pattern: private constructor to prevent initialization
}
