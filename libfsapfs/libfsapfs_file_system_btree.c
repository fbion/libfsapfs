/*
 * The file system B-tree functions
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsapfs_btree_footer.h"
#include "libfsapfs_btree_header.h"
#include "libfsapfs_file_system_btree.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"

#include "fsapfs_btree.h"
#include "fsapfs_object.h"
#include "fsapfs_file_system.h"

/* Creates a file system B-tree
 * Make sure the value file_system_btree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_btree_initialize(
     libfsapfs_file_system_btree_t **file_system_btree,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_file_system_btree_initialize";

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( *file_system_btree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system B-tree value already set.",
		 function );

		return( -1 );
	}
	*file_system_btree = memory_allocate_structure(
	                      libfsapfs_file_system_btree_t );

	if( *file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system B-tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_system_btree,
	     0,
	     sizeof( libfsapfs_file_system_btree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file system B-tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file_system_btree != NULL )
	{
		memory_free(
		 *file_system_btree );

		*file_system_btree = NULL;
	}
	return( -1 );
}

/* Frees a object map_btree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_btree_free(
     libfsapfs_file_system_btree_t **file_system_btree,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_file_system_btree_free";

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( *file_system_btree != NULL )
	{
		memory_free(
		 *file_system_btree );

		*file_system_btree = NULL;
	}
	return( 1 );
}

/* Reads the object map_btree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_btree_read_file_io_handle(
     libfsapfs_file_system_btree_t *file_system_btree,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t block_size,
     libcerror_error_t **error )
{
	uint8_t *block_data   = NULL;
	static char *function = "libfsapfs_file_system_btree_read_file_io_handle";
	ssize_t read_count    = 0;

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
#if ( SIZEOF_SIZE_T <= 4 )
	if( block_size > (uint32_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid block size value out of bounds.",
		 function );

		return( -1 );
	}
#endif
	block_data = (uint8_t *) memory_allocate(
	                          sizeof( uint8_t ) * block_size );

	if( block_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system B-tree.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading file system B-tree at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file system B-tree offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              block_data,
	              (size_t) block_size,
	              error );

	if( read_count != (ssize_t) block_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file system B-tree data.",
		 function );

		goto on_error;
	}
	if( libfsapfs_file_system_btree_read_data(
	     file_system_btree,
	     block_data,
	     (size_t) block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file system B-tree data.",
		 function );

		goto on_error;
	}
	memory_free(
	 block_data );

	return( 1 );

on_error:
	if( block_data != NULL )
	{
		memory_free(
		 block_data );
	}
	return( -1 );
}

/* Reads the object map_btree
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_file_system_btree_read_data(
     libfsapfs_file_system_btree_t *file_system_btree,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	fsapfs_btree_variable_size_entry_t *btree_entry = NULL;
	libfsapfs_btree_footer_t *btree_footer          = NULL;
	libfsapfs_btree_header_t *btree_header          = NULL;
	static char *function                           = "libfsapfs_file_system_btree_read_data";
	size_t data_offset                              = 0;
	size_t remaining_data_size                      = 0;
	uint64_t map_entry_index                        = 0;
	uint32_t object_subtype                         = 0;
	uint32_t object_type                            = 0;
	uint16_t entries_data_offset                    = 0;
	uint16_t footer_offset                          = 0;
	uint16_t key_data_offset                        = 0;
	uint16_t key_data_size                          = 0;
	uint16_t value_data_offset                      = 0;
	uint16_t value_data_size                        = 0;
	int entry_index                                 = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                            = 0;
#endif

	if( file_system_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system B-tree.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( fsapfs_object_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file system B-tree data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file system B-tree object data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_object_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_t *) data )->type,
	 object_type );

	if( object_type != 0x00000002UL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object type: 0x%08" PRIx32 ".",
		 function,
		 object_type );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_object_t *) data )->subtype,
	 object_subtype );

	if( object_subtype != 0x0000000eUL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object subtype: 0x%08" PRIx32 ".",
		 function,
		 object_subtype );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_object_t *) data )->checksum,
		 value_64bit );
		libcnotify_printf(
		 "%s: object checksum\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_object_t *) data )->identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object identifier\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_object_t *) data )->version,
		 value_64bit );
		libcnotify_printf(
		 "%s: object version\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: object type\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_type );

		libcnotify_printf(
		 "%s: object subtype\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_subtype );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = sizeof( fsapfs_object_t );

	if( data_size < ( data_offset + sizeof( fsapfs_btree_header_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		goto on_error;
	}
	if( libfsapfs_btree_header_initialize(
	     &btree_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree header.",
		 function );

		goto on_error;
	}
	if( libfsapfs_btree_header_read_data(
	     btree_header,
	     &( data[ data_offset ] ),
	     sizeof( fsapfs_btree_header_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree header.",
		 function );

		goto on_error;
	}
	if( btree_header->flags != 0x0003 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%04" PRIx16 ".",
		 function,
		 btree_header->flags );

		goto on_error;
	}
	data_offset += sizeof( fsapfs_btree_header_t );

	if( data_size < ( data_offset + sizeof( fsapfs_btree_footer_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		goto on_error;
	}
	remaining_data_size = data_size - data_offset - sizeof( fsapfs_btree_footer_t );

	if( btree_header->entries_data_offset >= remaining_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entries offset size value out of bounds.",
		 function );

		goto on_error;
	}
	remaining_data_size -= btree_header->entries_data_offset;

	if( btree_header->entries_data_size > remaining_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entries data size value out of bounds.",
		 function );

		goto on_error;
	}
	remaining_data_size -= btree_header->entries_data_size;

	if( btree_header->unused_data_offset >= remaining_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid unused offset size value out of bounds.",
		 function );

		goto on_error;
	}
	remaining_data_size -= btree_header->unused_data_offset;

	if( btree_header->unused_data_size > remaining_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid unused data size value out of bounds.",
		 function );

		goto on_error;
	}
/* TODO sanity check other data_offset and data_size values */

	if( libfsapfs_btree_footer_initialize(
	     &btree_footer,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B-tree footer.",
		 function );

		goto on_error;
	}
	if( libfsapfs_btree_footer_read_data(
	     btree_footer,
	     &( data[ data_size - sizeof( fsapfs_btree_footer_t ) ] ),
	     sizeof( fsapfs_btree_footer_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B-tree footer.",
		 function );

		goto on_error;
	}
	if( btree_footer->key_size != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key size value out of bounds.",
		 function );

		goto on_error;
	}
	if( btree_footer->value_size != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value size value out of bounds.",
		 function );

		goto on_error;
	}
	if( btree_footer->number_of_entries > (uint64_t) ( btree_header->entries_data_size / sizeof( fsapfs_btree_variable_size_entry_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		goto on_error;
	}
	data_offset += btree_header->entries_data_offset;

	entries_data_offset = btree_header->entries_data_offset + (uint16_t) ( sizeof( fsapfs_object_t ) + sizeof( fsapfs_btree_header_t ) );
	footer_offset       = (uint16_t) ( data_size - sizeof( fsapfs_btree_footer_t ) );

	for( map_entry_index = 0;
	     map_entry_index < btree_footer->number_of_entries;
	     map_entry_index++ )
	{
		btree_entry = (fsapfs_btree_variable_size_entry_t *) &( data[ data_offset ] );

		byte_stream_copy_to_uint16_little_endian(
		 btree_entry->key_data_offset,
		 key_data_offset );

		byte_stream_copy_to_uint16_little_endian(
		 btree_entry->key_data_size,
		 key_data_size );

		byte_stream_copy_to_uint16_little_endian(
		 btree_entry->value_data_offset,
		 value_data_offset );

		byte_stream_copy_to_uint16_little_endian(
		 btree_entry->value_data_size,
		 value_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry: %02" PRIu64 " key data offset\t: 0x%04" PRIx16 " (block offset: 0x%04" PRIzx ")\n",
			 function,
			 map_entry_index,
			 key_data_offset,
			 (size_t) key_data_offset + (size_t) entries_data_offset + (size_t) btree_header->entries_data_size );

			libcnotify_printf(
			 "%s: entry: %02" PRIu64 " key data size\t\t: %" PRIu16 "\n",
			 function,
			 map_entry_index,
			 key_data_size );

			libcnotify_printf(
			 "%s: entry: %02" PRIu64 " value data offset\t: 0x%04" PRIx16 " (block offset: 0x%04" PRIzx ")\n",
			 function,
			 map_entry_index,
			 value_data_offset,
			 (size_t) footer_offset - (size_t) value_data_offset);

			libcnotify_printf(
			 "%s: entry: %02" PRIu64 " value data size\t: %" PRIu16 "\n",
			 function,
			 map_entry_index,
			 value_data_size );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += sizeof( fsapfs_btree_variable_size_entry_t );

		key_data_offset += entries_data_offset + btree_header->entries_data_size;

		if( ( (size_t) key_data_offset > data_size )
		 || ( (size_t) key_data_size > ( data_size - key_data_offset ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid key data offset value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry: %" PRIu64 " key data:\n",
			 function,
			 map_entry_index );
			libcnotify_print_data(
			 &( data[ key_data_offset ] ),
			 (size_t) key_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		value_data_offset = footer_offset - value_data_offset;

		if( ( (size_t) value_data_offset > data_size )
		 || ( (size_t) value_data_size > ( data_size - value_data_offset ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value data offset value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry: %" PRIu64 " value data:\n",
			 function,
			 map_entry_index );
			libcnotify_print_data(
			 &( data[ value_data_offset ] ),
			 (size_t) value_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
	}
	if( libfsapfs_btree_footer_free(
	     &btree_footer,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B-tree footer.",
		 function );

		goto on_error;
	}
	if( libfsapfs_btree_header_free(
	     &btree_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B-tree header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( btree_footer != NULL )
	{
		libfsapfs_btree_footer_free(
		 &btree_footer,
		 NULL );
	}
	if( btree_header != NULL )
	{
		libfsapfs_btree_header_free(
		 &btree_header,
		 NULL );
	}
	return( -1 );
}

