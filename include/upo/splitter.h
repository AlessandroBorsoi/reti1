#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/** \brief Declares the Splitter type. */
typedef struct upo_protocol_splitter_s *upo_protocol_splitter_t;

/**
 * \brief Creates a new protocol splitter. NULL if the file is not found. Not valid if the file has an invalid format.
 * 
 * \param file_path The file location with the input.
 *
 * \return A splitter.
 */
upo_protocol_splitter_t upo_protocol_splitter_create(char *file_path);

/**
 * \brief Destroys the given splitter.
 *
 * \param splitter The splitter to destroy.
 */
void upo_protocol_splitter_destroy(upo_protocol_splitter_t *splitter);

/**
 * \brief Tells if a splitter is valid to be splitted.
 * 
 * \param splitter The splitter to evaluate.
 *
 * \return True if is valid.
 */
bool upo_protocol_splitter_is_valid(upo_protocol_splitter_t splitter);

/**
 * \brief Returns the protocol compliant string for the server if the splitter is valid based on is_valid.
 *
 * \param splitter The splitter representing the numbers to send.
 * \param output The string in which store the result of the splitting.
 * \param output_size The dimention of the output string.
 */
void upo_protocol_splitter_next(upo_protocol_splitter_t splitter, char *output, size_t output_size);
