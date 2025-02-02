/**
 * @file snifferData.h
 * @brief Container of helper functionalities
 *
 * This file contains the global helper functionalities
 * 
 *
 * @author Lorenzo Giuliani
 */

#ifndef __SNIFFER_DATA_H__
#define __SNIFFER_DATA_H__

#include <future>
#include "helper.h"

/**
 * @class SnifferData
 * @brief Global Container of helper funcionalities
*/
class SnifferData {
private:
    AsyncSerialSniffer asyncSniffer;
public:
    AsyncSerialSniffer& getAsyncSniffer();
};

extern SnifferData snifferData;


#endif