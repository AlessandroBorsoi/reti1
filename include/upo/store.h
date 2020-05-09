/**
 * \file upo/store.h
 *
 * \brief The Store abstract data type.
 *
 * \author Alessandro Borsoi
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

/** \brief Declares the Store type. */
typedef struct upo_store_s *upo_store_t;

/**
 * \brief Creates a new store.
 *
 * \return A store.
 */
upo_store_t upo_store_create();

/**
 * \brief Destroys the given store.
 *
 * \param store The store to destroy.
 */
void upo_store_destroy(upo_store_t store);

/**
 * \brief Add an element to the given store.
 *
 * \param store The store to which inserting a new element.
 * \param number The new number.
 */
void upo_store_insert(upo_store_t store, uint64_t number);

/**
 * \brief Returns the number of elements stored in the given store.
 *
 * \param store The store.
 * \return The number of elements stored in the store, or `0` otherwise.
 */
size_t upo_store_size(const upo_store_t store);

/**
 * \brief Returns the average of the elements stored in the given store.
 *
 * \param store The store.
 * \return The average of the (at least one) element(s) stored in the store, or `-1` otherwise.
 */
double upo_store_sample_mean(const upo_store_t store);

/**
 * \brief Returns the variance of the elements stored in the given store.
 *
 * \param store The store.
 * \return The variance of the (at least two) elements stored in the store, or `-1` otherwise.
 */
double upo_store_sample_variance(const upo_store_t store);
