#pragma once

#include <stddef.h>
#include <stdint.h>

/** \brief Declares the Splitter type. */
typedef struct upo_protocol_splitter_s *upo_protocol_splitter_t;

/**
 * \brief Creates a new protocol splitter.
 * 
 * \param file_path The file location with the input.
 *
 * \return A splitter.
 */
upo_protocol_splitter_t upo_protocol_splitter_create(char *file_path);

/**
 * \brief Destroys the given splitter.
 *
 * \param store The splitter to destroy.
 */
void upo_protocol_splitter_destroy(upo_protocol_splitter_t *splitter);

/**
 * \brief Returns the protocol compliant string for the server.
 *
 * \param splitter The splitter representing the numbers to send.
 * \param output The string in which store the result of the splitting.
 * \param output_size The dimention of the output string.
 */
void upo_protocol_splitter_next(upo_protocol_splitter_t splitter, char *output, size_t output_size);
