/*
 * Copyright 2003-2005 Sun Microsystems, Inc.  All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 *  
 */
// This file is a derivative work resulting from (and including) modifications
// made by Azul Systems, Inc.  The date of such changes is 2010.
// Copyright 2010 Azul Systems, Inc.  All Rights Reserved.
//
// Please contact Azul Systems, Inc., 1600 Plymouth Street, Mountain View, 
// CA 94043 USA, or visit www.azulsystems.com if you need additional information 
// or have any questions.


#include "adjoiningVirtualSpaces.hpp"
#include "java.hpp"
#include "psVirtualspace.hpp"

AdjoiningVirtualSpaces::AdjoiningVirtualSpaces(ReservedSpace rs,
					       size_t min_low_byte_size,
					       size_t min_high_byte_size,
					       size_t alignment) :
  _reserved_space(rs), _min_low_byte_size(min_low_byte_size),
  _min_high_byte_size(min_high_byte_size), _low(0), _high(0),
  _alignment(alignment) {}

// The maximum byte sizes are for the initial layout of the 
// virtual spaces and are not the limit on the maximum bytes sizes.
void AdjoiningVirtualSpaces::initialize(size_t max_low_byte_size,
					size_t init_low_byte_size,
					size_t init_high_byte_size) {

  // The reserved spaces for the two parts of the virtual space.
  ReservedSpace old_rs   = _reserved_space.first_part(max_low_byte_size);
  ReservedSpace young_rs = _reserved_space.last_part(max_low_byte_size);

  _low = new PSVirtualSpace(old_rs, alignment(), os::JAVA_COMMITTED_MEMORY_ACCOUNT);
  if (!_low->expand_by(init_low_byte_size)) {
    vm_exit_during_initialization("Could not reserve enough space for "
				  "object heap");
  }

  _high = new PSVirtualSpaceHighToLow(young_rs, alignment(),
                                      os::JAVA_COMMITTED_MEMORY_ACCOUNT);
  if (!_high->expand_by(init_high_byte_size)) {
    vm_exit_during_initialization("Could not reserve enough space for "
				  "object heap");
  }  
}

bool AdjoiningVirtualSpaces::adjust_boundary_up(size_t change_in_bytes) {
  assert(UseAdaptiveSizePolicy && UseAdaptiveGCBoundary, "runtime check");
  size_t actual_change = low()->expand_into(high(), change_in_bytes);
  return actual_change != 0;
}

bool AdjoiningVirtualSpaces::adjust_boundary_down(size_t change_in_bytes) {
  assert(UseAdaptiveSizePolicy && UseAdaptiveGCBoundary, "runtime check");
  size_t actual_change = high()->expand_into(low(), change_in_bytes);
  return actual_change != 0;
}
