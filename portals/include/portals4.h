/*
 * Copyright (C) Bull S.A.S - 2024
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * BXI Low Level Team
 *
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <poll.h>

#ifndef __cplusplus
#include <stdatomic.h>
#else
#include <atomic>
typedef std::atomic<bool> atomic_bool;
extern "C" {
#endif

#define PTL_LE_OP_PUT PTL_ME_OP_PUT
#define PTL_LE_OP_GET PTL_ME_OP_GET
#define PTL_LE_USE_ONCE PTL_ME_USE_ONCE
#define PTL_LE_ACK_DISABLE PTL_ME_ACK_DISABLE
#define PTL_LE_UNEXPECTED_HDR_DISABLE PTL_ME_UNEXPECTED_HDR_DISABLE
#define PTL_LE_IS_ACCESSIBLE PTL_ME_IS_ACCESSIBLE
#define PTL_LE_EVENT_COMM_DISABLE PTL_ME_EVENT_COMM_DISABLE
#define PTL_LE_EVENT_FLOWCTRL_DISABLE PTL_ME_EVENT_FLOWCTRL_DISABLE
#define PTL_LE_EVENT_SUCCESS_DISABLE PTL_ME_EVENT_SUCCESS_DISABLE
#define PTL_LE_EVENT_OVER_DISABLE PTL_ME_EVENT_OVER_DISABLE
#define PTL_LE_EVENT_UNLINK_DISABLE PTL_ME_EVENT_UNLINK_DISABLE
#define PTL_LE_EVENT_LINK_DISABLE PTL_ME_EVENT_LINK_DISABLE
#define PTL_LE_EVENT_CT_COMM PTL_ME_EVENT_CT_COMM
#define PTL_LE_EVENT_CT_OVERFLOW PTL_ME_EVENT_CT_OVERFLOW
#define PTL_LE_EVENT_CT_BYTES PTL_ME_EVENT_CT_BYTES

#define PTL_CT_NONE ((ptl_handle_ct_t){ NULL })
#define PTL_EQ_NONE ((ptl_handle_eq_t){ NULL })
#define PTL_INVALID_HANDLE ((ptl_handle_any_t){ NULL })

#define PTL_TARGET_BIND_INACCESSIBLE 0x1
#define PTL_TOTAL_DATA_ORDERING 0x2

#define PTL_TIME_FOREVER (-1)

#define PTL_IFACE_DEFAULT 0

/*
 * max ptl_pid_t
 */
#define PTL_PID_MAX ((1 << 12) - 1)

/*
 * options for PtlNIInit(),
 */
#define PTL_NI_PHYSICAL (0x1 << 0)
#define PTL_NI_MATCHING (0x1 << 1)
#define PTL_NI_LOGICAL (0x1 << 2)
#define PTL_NI_NO_MATCHING (0x1 << 3)

/*
 * special identifiers
 */
#define PTL_PT_ANY (~0)
#define PTL_PID_ANY 0x0
#define PTL_UID_ANY ((ptl_uid_t)-1)
#define PTL_NID_ANY 0x0
#define PTL_RANK_ANY 0x7fffff

/*
 * PT options
 */
#define PTL_PT_ONLY_USE_ONCE 1
#define PTL_PT_ONLY_TRUNCATE 2
#define PTL_PT_FLOWCTRL 4
#define PTL_PT_ALLOC_DISABLED 8

/*
 * event types
 */
typedef enum {
	PTL_EVENT_GET,
	PTL_EVENT_GET_OVERFLOW,
	PTL_EVENT_PUT,
	PTL_EVENT_PUT_OVERFLOW,
	PTL_EVENT_ATOMIC,
	PTL_EVENT_ATOMIC_OVERFLOW,
	PTL_EVENT_FETCH_ATOMIC,
	PTL_EVENT_FETCH_ATOMIC_OVERFLOW,
	PTL_EVENT_REPLY,
	PTL_EVENT_SEND,
	PTL_EVENT_ACK,
	PTL_EVENT_PT_DISABLED,
	PTL_EVENT_AUTO_UNLINK,
	PTL_EVENT_AUTO_FREE,
	PTL_EVENT_SEARCH,
	PTL_EVENT_LINK,
} ptl_event_kind_t;

/*
 * ACK request modes
 */
#define PTL_NO_ACK_REQ 0x0
#define PTL_CT_ACK_REQ 0x1
#define PTL_OC_ACK_REQ 0x2
#define PTL_ACK_REQ 0x3

/*
 * NI fail types
 */
#define PTL_FAIL 0x6
#define PTL_ARG_INVALID 0x7
#define PTL_IN_USE 0x8

/*
 * Error codes.
 * We use a range above the 0..31 range to avoid dealing with namespace
 * conflicts
 */
#define PTL_OK PTL_NI_OK
#define PTL_CT_NONE_REACHED 32
#define PTL_EQ_DROPPED 33
#define PTL_EQ_EMPTY 34
#define PTL_IGNORED 36
#define PTL_INTERRUPTED 37
#define PTL_LIST_TOO_LONG 38
#define PTL_NO_INIT 39
#define PTL_NO_SPACE 40
#define PTL_PID_IN_USE 41
#define PTL_PT_FULL 42
#define PTL_PT_EQ_NEEDED 43
#define PTL_PT_IN_USE 44
#define PTL_ABORTED 47

typedef enum {
	PTL_NI_UNDELIVERABLE = 0x6,
	PTL_NI_PT_DISABLED = 0x7,
	PTL_NI_DROPPED = 0x8,
	PTL_NI_PERM_VIOLATION = 0x9,
	PTL_NI_OP_VIOLATION = 0xA,
	PTL_NI_SEGV = 0xB,
	PTL_NI_NO_MATCH = 0xC,
	PTL_NI_OK = 0x0,

	/* BXI Extensions */
	PTL_NI_ARG_INVALID = PTL_ARG_INVALID,
} ptl_ni_fail_t;

/*
 * PTL_IOVEC option common to MEs and MDs
 */
#define PTL_IOVEC 0x80

/*
 * ME options
 */
#define PTL_ME_EVENT_COMM_DISABLE 0x1
#define PTL_ME_EVENT_SUCCESS_DISABLE 0x2
#define PTL_ME_EVENT_OVER_DISABLE 0x4
#define PTL_ME_EVENT_LINK_DISABLE 0x8
#define PTL_ME_EVENT_CT_BYTES 0x10
#define PTL_ME_EVENT_CT_OVERFLOW 0x20
#define PTL_ME_EVENT_CT_COMM 0x40
#define PTL_ME_OP_PUT 0x100
#define PTL_ME_OP_GET 0x200
#define PTL_ME_USE_ONCE 0x400
#define PTL_ME_ACK_DISABLE 0x800
#define PTL_ME_MAY_ALIGN 0x1000
#define PTL_ME_EVENT_UNLINK_DISABLE 0x2000
#define PTL_ME_MANAGE_LOCAL 0x4000
#define PTL_ME_NO_TRUNCATE 0x8000
#define PTL_ME_UNEXPECTED_HDR_DISABLE 0x10000
#define PTL_ME_EVENT_FLOWCTRL_DISABLE 0x20000
#define PTL_ME_IS_ACCESSIBLE 0x40000
#define PTL_ME_LOCAL_INC_UH_RLENGTH 0x100000

/*
 * MD options
 */
#define PTL_MD_EVENT_SEND_DISABLE 0x40
#define PTL_MD_EVENT_SUCCESS_DISABLE 0x1
#define PTL_MD_EVENT_CT_SEND 0x2
#define PTL_MD_EVENT_CT_REPLY 0x4
#define PTL_MD_EVENT_CT_ACK 0x8
#define PTL_MD_EVENT_CT_BYTES 0x10
#define PTL_MD_UNORDERED 0x20
#define PTL_MD_VOLATILE 0x100
#define PTL_MD_UNRELIABLE 0x200

/*
 * Ptl{ME,LE}Search() op parameter
 */
#define PTL_SEARCH_ONLY 0x0
#define PTL_SEARCH_DELETE 0x1

/*
 * list types
 */
#define PTL_PRIORITY_LIST 0x0
#define PTL_OVERFLOW_LIST 0x1

/*
 * atomic operations
 */
typedef enum {
	PTL_MIN = 0x0,
	PTL_MAX = 0x1,
	PTL_SUM = 0x2,
	PTL_PROD = 0x3,
	PTL_LOR = 0x4,
	PTL_LAND = 0x5,
	PTL_LXOR = 0x6,
	PTL_BOR = 0x8,
	PTL_BAND = 0x9,
	PTL_BXOR = 0xa,
	PTL_SWAP = 0xc,
	PTL_CSWAP_GT = 0x10,
	PTL_CSWAP_LT = 0x11,
	PTL_CSWAP_GE = 0x12,
	PTL_CSWAP_LE = 0x13,
	PTL_CSWAP = 0x14,
	PTL_CSWAP_NE = 0x15,
	PTL_MSWAP = 0x18,
} ptl_op_t;

/*
 * atomic types
 */
typedef enum {
	PTL_INT8_T = 0x0,
	PTL_UINT8_T = 0x1,
	PTL_INT16_T = 0x2,
	PTL_UINT16_T = 0x3,
	PTL_INT32_T = 0x4,
	PTL_UINT32_T = 0x5,
	PTL_INT64_T = 0x6,
	PTL_UINT64_T = 0x7,
	PTL_FLOAT = 0xa,
	PTL_FLOAT_COMPLEX = 0xb,
	PTL_DOUBLE = 0xc,
	PTL_DOUBLE_COMPLEX = 0xd,
	PTL_LONG_DOUBLE = 0x14,
	PTL_LONG_DOUBLE_COMPLEX = 0x15,
} ptl_datatype_t;

/*
 * scalar types
 */
typedef unsigned int ptl_pid_t;
typedef unsigned int ptl_nid_t;
typedef int ptl_ack_req_t;
typedef int ptl_interface_t;
typedef unsigned int ptl_rank_t;
typedef int ptl_sr_value_t;
typedef int ptl_uid_t;
typedef int ptl_list_t;
typedef unsigned int ptl_index_t;
typedef uint64_t ptl_size_t;
typedef uint64_t ptl_match_bits_t;
typedef uint64_t ptl_hdr_data_t;
typedef long ptl_time_t;

typedef struct {
	void *handle;
} ptl_handle_any_t;
typedef ptl_handle_any_t ptl_handle_ni_t;
typedef ptl_handle_any_t ptl_handle_md_t;
typedef ptl_handle_any_t ptl_handle_eq_t;
typedef ptl_handle_any_t ptl_handle_ct_t;
typedef ptl_handle_any_t ptl_handle_me_t;
typedef ptl_handle_me_t ptl_handle_le_t;

typedef unsigned int ptl_search_op_t;
typedef void *ptl_addr_t;

/*
 * status registers
 */

typedef enum {
	PTL_SR_DROP_COUNT,
	PTL_SR_PERMISSION_VIOLATIONS,
	PTL_SR_OPERATION_VIOLATIONS,
} ptl_sr_index_t;

typedef unsigned int ptl_pt_index_t;

/*
 * nic limits
 */
typedef struct {
	int max_entries;
	int max_unexpected_headers;
	int max_mds;
	int max_cts;
	int max_eqs;
	int max_pt_index;
	int max_iovecs;
	int max_list_size;
	int max_triggered_ops;
	ptl_size_t max_msg_size;
	ptl_size_t max_atomic_size;
	ptl_size_t max_fetch_atomic_size;
	ptl_size_t max_waw_ordered_size;
	ptl_size_t max_war_ordered_size;
	ptl_size_t max_volatile_size;
	unsigned int features;
} ptl_ni_limits_t;

/*
 * logical interface to physical (nid, pid) mapping entry
 */
typedef union {
	struct {
		ptl_nid_t nid;
		ptl_pid_t pid;
	} phys;
	ptl_rank_t rank;
} ptl_process_t;

/*
 * event
 */
typedef struct {
	ptl_addr_t start;
	void *user_ptr;
	ptl_hdr_data_t hdr_data;
	ptl_match_bits_t match_bits;
	ptl_size_t rlength, mlength, remote_offset;
	ptl_uid_t uid;
	ptl_process_t initiator;
	ptl_event_kind_t type;
	ptl_list_t ptl_list;
	ptl_pt_index_t pt_index;
	ptl_ni_fail_t ni_fail_type;
	ptl_op_t atomic_operation;
	ptl_datatype_t atomic_type;
} ptl_event_t;

/*
 * I/O Vector
 */
typedef struct {
	ptl_addr_t iov_base;
	ptl_size_t iov_len;
} ptl_iovec_t;

/*
 * CT event
 */
typedef struct {
	ptl_size_t success;
	ptl_size_t failure;
} ptl_ct_event_t;

typedef struct {
	ptl_addr_t start;
	ptl_size_t length;
	ptl_handle_ct_t ct_handle;
	ptl_uid_t uid;
	unsigned int options;
	ptl_process_t match_id;
	ptl_match_bits_t match_bits;
	ptl_match_bits_t ignore_bits;
	ptl_size_t min_free;
} ptl_me_t, ptl_le_t;

typedef struct {
	ptl_addr_t start;
	ptl_size_t length;
	unsigned int options;
	ptl_handle_eq_t eq_handle;
	ptl_handle_ct_t ct_handle;
} ptl_md_t;

int PtlInit(void);

int PtlNIInit(ptl_interface_t iface, unsigned int options, ptl_pid_t pid,
	      const ptl_ni_limits_t *desired, ptl_ni_limits_t *actual, ptl_handle_ni_t *ni_handle);

int PtlEQAlloc(ptl_handle_ni_t ni_handle, ptl_size_t c, ptl_handle_eq_t *eq_handle);

int PtlPTAlloc(ptl_handle_ni_t ni_handle, unsigned int options, ptl_handle_eq_t eq_handle,
	       ptl_pt_index_t pt_index_req, ptl_pt_index_t *pt_index);

int PtlLEAppend(ptl_handle_ni_t ni_handle, ptl_pt_index_t pt_index, const ptl_le_t *le,
		ptl_list_t ptl_list, void *user_ptr, ptl_handle_le_t *le_handle);

int PtlGetId(ptl_handle_ni_t ni_handle, ptl_process_t *id);

int PtlEQWait(ptl_handle_eq_t eq_handle, ptl_event_t *event);

int PtlMDBind(ptl_handle_ni_t ni_handle, const ptl_md_t *md, ptl_handle_md_t *md_handle);

int PtlPut(ptl_handle_md_t md_handle, ptl_size_t local_offset, ptl_size_t length,
	   ptl_ack_req_t ack_req, ptl_process_t target_id, ptl_pt_index_t pt_index,
	   ptl_match_bits_t match_bits, ptl_size_t remote_offset, void *user_ptr,
	   ptl_hdr_data_t hdr_data);

int PtlMEAppend(ptl_handle_ni_t ni_handle, ptl_pt_index_t pt_index, const ptl_le_t *me,
		ptl_list_t ptl_list, void *user_ptr, ptl_handle_le_t *me_handle);

void PtlFini(void);

void PtlAbort(void);

int PtlNIFini(ptl_handle_ni_t ni_handle);

int PtlNIHandle(ptl_handle_any_t handle, ptl_handle_ni_t *ni_handle);

int PtlNIStatus(ptl_handle_ni_t ni_handle, ptl_sr_index_t status_register, ptl_sr_value_t *status);

int PtlSetMap(ptl_handle_ni_t ni_handle, ptl_size_t map_size, const ptl_process_t *mapping);

int PtlGetMap(ptl_handle_ni_t ni_handle, ptl_size_t map_size, ptl_process_t *mapping,
	      ptl_size_t *actual_map_size);

int PtlPTFree(ptl_handle_ni_t ni_handle, ptl_pt_index_t pt_index);

int PtlPTEnable(ptl_handle_ni_t ni_handle, ptl_pt_index_t pt_index);

int PtlGetUid(ptl_handle_ni_t ni_handle, ptl_uid_t *uid);

int PtlGetPhysId(ptl_handle_ni_t ni_handle, ptl_process_t *id);

int PtlMDRelease(ptl_handle_md_t md_handle);

int PtlLEUnlink(ptl_handle_le_t le_handle);

int PtlMEUnlink(ptl_handle_le_t me_handle);

int PtlLESearch(ptl_handle_ni_t ni_handle, ptl_pt_index_t pt_index, const ptl_le_t *le,
		ptl_search_op_t ptl_search_op, void *user_ptr);

int PtlMESearch(ptl_handle_ni_t ni_handle, ptl_pt_index_t pt_index, const ptl_me_t *me,
		ptl_search_op_t ptl_search_op, void *user_ptr);

int PtlEQFree(ptl_handle_eq_t eq_handle);

int PtlEQGet(ptl_handle_eq_t eq_handle, ptl_event_t *event);

int PtlCTAlloc(ptl_handle_ni_t ni_handle, ptl_handle_ct_t *ct_handle);

int PtlCTFree(ptl_handle_ct_t ct_handle);

int PtlCTCancelTriggered(ptl_handle_ct_t ct_handle);

int PtlCTGet(ptl_handle_ct_t ct_handle, ptl_ct_event_t *event);

int PtlCTPoll(const ptl_handle_ct_t *ct_handles, const ptl_size_t *tests, unsigned int size,
	      ptl_time_t timeout, ptl_ct_event_t *event, unsigned int *which);

int PtlGet(ptl_handle_md_t md_handle, ptl_size_t local_offset, ptl_size_t length,
	   ptl_process_t target_id, ptl_pt_index_t pt_index, ptl_match_bits_t match_bits,
	   ptl_size_t remote_offset, void *user_ptr);

int PtlAtomic(ptl_handle_md_t md_handle, ptl_size_t local_offset, ptl_size_t length,
	      ptl_ack_req_t ack_req, ptl_process_t target_id, ptl_pt_index_t pt_index,
	      ptl_match_bits_t match_bits, ptl_size_t remote_offset, void *user_ptr,
	      ptl_hdr_data_t hdr_data, ptl_op_t operation, ptl_datatype_t datatype);

int PtlFetchAtomic(ptl_handle_md_t get_md_handle, ptl_size_t local_get_offset,
		   ptl_handle_md_t put_md_handle, ptl_size_t local_put_offset, ptl_size_t length,
		   ptl_process_t target_id, ptl_pt_index_t pt_index, ptl_match_bits_t match_bits,
		   ptl_size_t remote_offset, void *user_ptr, ptl_hdr_data_t hdr_data,
		   ptl_op_t operation, ptl_datatype_t datatype);

int PtlSwap(ptl_handle_md_t get_md_handle, ptl_size_t local_get_offset,
	    ptl_handle_md_t put_md_handle, ptl_size_t local_put_offset, ptl_size_t length,
	    ptl_process_t target_id, ptl_pt_index_t pt_index, ptl_match_bits_t match_bits,
	    ptl_size_t remote_offset, void *user_ptr, ptl_hdr_data_t hdr_data, const void *operand,
	    ptl_op_t operation, ptl_datatype_t datatype);

int PtlTriggeredPut(ptl_handle_md_t md_handle, ptl_size_t local_offset, ptl_size_t length,
		    ptl_ack_req_t ack_req, ptl_process_t target_id, ptl_pt_index_t pt_index,
		    ptl_match_bits_t match_bits, ptl_size_t remote_offset, void *user_ptr,
		    ptl_hdr_data_t hdr_data, ptl_handle_ct_t trig_ct_handle, ptl_size_t threshold);

int PtlTriggeredGet(ptl_handle_md_t md_handle, ptl_size_t local_offset, ptl_size_t length,
		    ptl_process_t target_id, ptl_pt_index_t pt_index, ptl_match_bits_t match_bits,
		    ptl_size_t remote_offset, void *user_ptr, ptl_handle_ct_t trig_ct_handle,
		    ptl_size_t threshold);

int PtlTriggeredAtomic(ptl_handle_md_t md_handle, ptl_size_t local_offset, ptl_size_t length,
		       ptl_ack_req_t ack_req, ptl_process_t target_id, ptl_pt_index_t pt_index,
		       ptl_match_bits_t match_bits, ptl_size_t remote_offset, void *user_ptr,
		       ptl_hdr_data_t hdr_data, ptl_op_t operation, ptl_datatype_t datatype,
		       ptl_handle_ct_t trig_ct_handle, ptl_size_t threshold);

int PtlTriggeredFetchAtomic(ptl_handle_md_t get_md_handle, ptl_size_t local_get_offset,
			    ptl_handle_md_t put_md_handle, ptl_size_t local_put_offset,
			    ptl_size_t length, ptl_process_t target_id, ptl_pt_index_t pt_index,
			    ptl_match_bits_t match_bits, ptl_size_t remote_offset, void *user_ptr,
			    ptl_hdr_data_t hdr_data, ptl_op_t operation, ptl_datatype_t datatype,
			    ptl_handle_ct_t trig_ct_handle, ptl_size_t threshold);

int PtlTriggeredSwap(ptl_handle_md_t get_md_handle, ptl_size_t local_get_offset,
		     ptl_handle_md_t put_md_handle, ptl_size_t local_put_offset, ptl_size_t length,
		     ptl_process_t target_id, ptl_pt_index_t pt_index, ptl_match_bits_t match_bits,
		     ptl_size_t remote_offset, void *user_ptr, ptl_hdr_data_t hdr_data,
		     const void *operand, ptl_op_t operation, ptl_datatype_t datatype,
		     ptl_handle_ct_t trig_ct_handle, ptl_size_t threshold);

int PtlTriggeredCTInc(ptl_handle_ct_t ct_handle, ptl_ct_event_t increment,
		      ptl_handle_ct_t trig_ct_handle, ptl_size_t threshold);

int PtlTriggeredCTSet(ptl_handle_ct_t ct_handle, ptl_ct_event_t increment,
		      ptl_handle_ct_t trig_ct_handle, ptl_size_t threshold);

int PtlEQPoll(const ptl_handle_eq_t *eq_handles, unsigned int size, ptl_time_t timeout,
	      ptl_event_t *event, unsigned int *which);

int PtlStartBundle(ptl_handle_ni_t ni_handle);

int PtlEndBundle(ptl_handle_ni_t ni_handle);

int PtlCTWait(ptl_handle_ct_t ct_handle, ptl_size_t test, ptl_ct_event_t *event);

int PtlCTInc(ptl_handle_ct_t ct_handle, ptl_ct_event_t increment);

int PtlCTSet(ptl_handle_ct_t ct_handle, ptl_ct_event_t increment);

int PtlAtomicSync();

int PtlHandleIsEqual(ptl_handle_any_t handle1, ptl_handle_any_t handle2);

#ifdef __cplusplus
}
#endif
