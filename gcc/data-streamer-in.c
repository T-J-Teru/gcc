/* Routines for restoring various data types from a file stream.  This deals
   with various data types like strings, integers, enums, etc.

   Copyright 2011 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@google.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "diagnostic.h"
#include "data-streamer.h"

/* Read a string from the string table in DATA_IN using input block
   IB.  Write the length to RLEN.  */

const char *
string_for_index (struct data_in *data_in, unsigned int loc, unsigned int *rlen)
{
  struct lto_input_block str_tab;
  unsigned int len;
  const char *result;

  if (!loc)
    {
      *rlen = 0;
      return NULL;
    }

  /* Get the string stored at location LOC in DATA_IN->STRINGS.  */
  LTO_INIT_INPUT_BLOCK (str_tab, data_in->strings, loc - 1,
			data_in->strings_len);
  len = lto_input_uleb128 (&str_tab);
  *rlen = len;

  if (str_tab.p + len > data_in->strings_len)
    internal_error ("bytecode stream: string too long for the string table");

  result = (const char *)(data_in->strings + str_tab.p);

  return result;
}


/* Read a string from the string table in DATA_IN using input block
   IB.  Write the length to RLEN.  */

const char *
input_string_internal (struct data_in *data_in, struct lto_input_block *ib,
		       unsigned int *rlen)
{
  return string_for_index (data_in, lto_input_uleb128 (ib), rlen);
}


/* Read a NULL terminated string from the string table in DATA_IN.  */

const char *
lto_input_string (struct data_in *data_in, struct lto_input_block *ib)
{
  unsigned int len;
  const char *ptr;

  ptr = input_string_internal (data_in, ib, &len);
  if (!ptr)
    return NULL;
  if (ptr[len - 1] != '\0')
    internal_error ("bytecode stream: found non-null terminated string");

  return ptr;
}


/* Read an ULEB128 Number of IB.  */

unsigned HOST_WIDE_INT
lto_input_uleb128 (struct lto_input_block *ib)
{
  unsigned HOST_WIDE_INT result = 0;
  int shift = 0;
  unsigned HOST_WIDE_INT byte;

  while (true)
    {
      byte = lto_input_1_unsigned (ib);
      result |= (byte & 0x7f) << shift;
      shift += 7;
      if ((byte & 0x80) == 0)
	return result;
    }
}


/* HOST_WIDEST_INT version of lto_input_uleb128.  IB is as in
   lto_input_uleb128.  */

unsigned HOST_WIDEST_INT
lto_input_widest_uint_uleb128 (struct lto_input_block *ib)
{
  unsigned HOST_WIDEST_INT result = 0;
  int shift = 0;
  unsigned HOST_WIDEST_INT byte;

  while (true)
    {
      byte = lto_input_1_unsigned (ib);
      result |= (byte & 0x7f) << shift;
      shift += 7;
      if ((byte & 0x80) == 0)
	return result;
    }
}


/* Read an SLEB128 Number of IB.  */

HOST_WIDE_INT
lto_input_sleb128 (struct lto_input_block *ib)
{
  HOST_WIDE_INT result = 0;
  int shift = 0;
  unsigned HOST_WIDE_INT byte;

  while (true)
    {
      byte = lto_input_1_unsigned (ib);
      result |= (byte & 0x7f) << shift;
      shift += 7;
      if ((byte & 0x80) == 0)
	{
	  if ((shift < HOST_BITS_PER_WIDE_INT) && (byte & 0x40))
	    result |= - ((HOST_WIDE_INT)1 << shift);

	  return result;
	}
    }
}
