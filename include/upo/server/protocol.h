/**
 * \file upo/server/protocol.h
 *
 * \brief The server network Protocol interface
 *
 * \author Alessandro Borsoi
 */
#pragma once

#include <upo/protocol.h>
#include <upo/server/store.h>

/**
 * \brief Given a store and an input string, returns the correct protocol output and response.
 *
 * \param store The store used for the calculations.
 * \param input The input string.
 * \param output The output string.
 * 
 * \return A upo_protocol_response_t.
 */
upo_protocol_response_t upo_protocol(upo_store_t store, const char *input, char *output);
