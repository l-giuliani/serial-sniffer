/**
 * @file snifferData.h
 * @brief Container of helper functionalities
 *
 * This file contains the global helper functionalities
 * 
 *
 * @author Lorenzo Giuliani
 */

#include "snifferData.h"

SnifferData snifferData;

/**
 * @brief Global Async sniffer getter
 * @return The global asyncSniffer
*/
AsyncSniffer& SnifferData::getAsyncSniffer() {
    return asyncSniffer;
}