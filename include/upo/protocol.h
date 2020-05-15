/**
 * \file upo/protocol.h
 *
 * \brief The network Protocol interface
 *
 * \author Alessandro Borsoi
 */
#pragma once

/** \brief Define the max size of the messages (input and output) allowed by the protocol */
#define UPO_PROTOCOL_MAX 512

/** \brief Declares the possible type of output of the protocol */
typedef enum
{
    OK_START,
    OK_DATA,
    OK_STATS,
    ERR_SYNTAX,
    ERR_DATA,
    ERR_STATS,
    INVALID
} upo_protocol_response_t;
