#pragma once

inline unsigned int uniqueNumber = 0;
inline unsigned int GetUniqueNumber(){
    uniqueNumber++;
    return uniqueNumber;
}