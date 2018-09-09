/*
 * Volume functions
 *
 * Copyright (C) 2018, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBFSAPFS_INTERNAL_VOLUME_H )
#define _LIBFSAPFS_INTERNAL_VOLUME_H

#include <common.h>
#include <types.h>

#include "libfsapfs_extern.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsapfs_internal_volume libfsapfs_internal_volume_t;

struct libfsapfs_internal_volume
{
	/* The IO handle
	 */
	libfsapfs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;
};

LIBFSAPFS_EXTERN \
int libfsapfs_volume_initialize(
     libfsapfs_volume_t **volume,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_free(
     libfsapfs_volume_t **volume,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_signal_abort(
     libfsapfs_volume_t *volume,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_open(
     libfsapfs_volume_t *volume,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBFSAPFS_EXTERN \
int libfsapfs_volume_open_wide(
     libfsapfs_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBFSAPFS_EXTERN \
int libfsapfs_volume_open_file_io_handle(
     libfsapfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBFSAPFS_EXTERN \
int libfsapfs_volume_close(
     libfsapfs_volume_t *volume,
     libcerror_error_t **error );

int libfsapfs_volume_open_read(
     libfsapfs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSAPFS_INTERNAL_VOLUME_H ) */
