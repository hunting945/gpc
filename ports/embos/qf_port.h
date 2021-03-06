/**
* @file
* @brief QF/C port to embOS
* @ingroup ports
* @cond
******************************************************************************
* Last Updated for Version: 5.4.0
* Date of the Last Update:  2015-04-08
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. state-machine.com.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qf_port_h
#define qf_port_h

/* embOS message mailbox and thread types */
#define QF_EQUEUE_TYPE      OS_MAILBOX
#define QF_THREAD_TYPE      OS_TASK
#define QF_OS_OBJECT_TYPE   uint32_t

/* The maximum number of active objects in the application, see NOTE1 */
#define QF_MAX_ACTIVE       32

/* QF interrupt disable/enable */
#define QF_INT_DISABLE()    OS_IncDI()
#define QF_INT_ENABLE()     OS_DecRI()

/* QF critical section for embOS, see NOTE2 */
/*#define QF_CRIT_STAT_TYPE not defined */
#define QF_CRIT_ENTRY(dummy)  QF_INT_DISABLE()
#define QF_CRIT_EXIT(dummy)   QF_INT_ENABLE()

/* thred options... */
#define QF_TASK_USES_FPU    ((uint32_t)1)

#include "RTOS.h"      /* embOS API */
#include "qep_port.h"  /* QEP port */
#include "qequeue.h"   /* used for event deferral */
#include "qmpool.h"    /* this QP port uses the native QF memory pool */
#include "qf.h"        /* QF platform-independent public interface */

/* set attributes (e.g., QF_TASK_USES_FPU) before calling QACTIVE_START() */
void QF_setEmbOsTaskAttr(QActive *act, uint32_t attr);

/*****************************************************************************
* interface used only inside QF, but not in applications
*/
#ifdef QP_IMPL

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_) ((e_) = (QEvt *)QMPool_get(&(p_), (m_)))
    #define QF_EPOOL_PUT_(p_, e_)     (QMPool_put(&(p_), (e_)))

#endif /* ifdef QP_IMPL */

/*****************************************************************************
* NOTE1:
* The maximum number of active objects in QP can be increased to 63,
* inclusive, but it can be reduced to save some memory. Also, the number of
* active objects cannot exceed the number of embOS thread priorities,
* because each QP active object requires a unique priority level.
*
* NOTE2:
* The embOS critical section does not use the status variable, because the
* implementation uses a global up-down counter, which allows the embOS
* critical section to nest. Nesting of critical sections is needed in this
* QP-embOS port.
*/

#endif /* qf_port_h */
