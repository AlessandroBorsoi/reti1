/**
 * \file upo/protocol.h
 *
 * \brief The network Protocol interface
 *
 * \author Alessandro Borsoi
 */
#pragma once

#include <upo/store.h>

/** \brief Declares the possible type of output of the protocol */
typedef enum
{
    OK_DATA,
    OK_STATS,
    ERR_SYNTAX,
    ERR_DATA,
    ERR_STATS
} * upo_protocol_response_t;

/**
 * \brief Given a store and an input string, returns the correct protocol output.
 *
 * \param store The store used for the number calculations.
 * \param input The input string.
 * \param output The output.
 */
void upo_protocol(upo_store_t store, const char *input, char *output, upo_protocol_response_t response);
