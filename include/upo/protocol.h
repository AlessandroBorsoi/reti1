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

/**
 * \brief Given a protocol response, returns the string rapresentation.
 *
 * \param response The upo_protocol_response_t.
 * 
 * \return A string rapresentation of the upo_protocol_response_t.
 */
// const char *upo_protocol_to_string(upo_protocol_response_t response)
// {
//     switch (response)
//     {
//     case OK_START:
//         return "OK START";
//     case OK_DATA:
//         return "OK DATA";
//     case OK_STATS:
//         return "OK STATS";
//     case ERR_SYNTAX:
//         return "ERR SYNTAX";
//     case ERR_DATA:
//         return "ERR DATA";
//     case ERR_STATS:
//         return "ERR STATS";
//     default:
//         return "INVALID";
//     }
// }
