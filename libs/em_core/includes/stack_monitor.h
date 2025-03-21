////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/em_core/includes/stack_monitor.h
///
/// @project    T9305
///
/// @brief      Functions to report the maximum size the stack grew to.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2020, EM Microelectronic
/// @cond
///
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
/// 1. Redistributions of source code must retain the above copyright notice,
/// this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice,
/// this list of conditions and the following disclaimer in the documentation
/// and/or other materials provided with the distribution.
///
////////////////////////////////////////////////////////////////////////////////
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
/// @endcond
////////////////////////////////////////////////////////////////////////////////


#ifndef _STACK_MONITOR_H
#define _STACK_MONITOR_H


#include <types.h>


/**
 * @defgroup stack_monitor Stack Monitor
 * @ingroup drivers
 * @brief Stack Monitor
 */


/**
 * @addtogroup stack_monitor
 * @{
 */


/******************************************************************************\
 *  API FUNCTION PROTOTYPES
\******************************************************************************/


/**
 * @brief Start the stack monitor.
 * This will write a pattern into the stack. This pattern can be checked
 * later to determine the maximum stack usage.
 */
void StackMonitor_Start(void);


/**
 * @brief Check what the maximum number of bytes used in the stack was.
 * @returns Maximum number of bytes in the stack that were used.
 * This check the stack for the pattern and stops when it has been overwritten
 * by a stack write.
 */
uint32_t StackMonitor_CheckUsage(void);


/**
 * @}
 */

#endif /* _STACK_MONITOR_H */
