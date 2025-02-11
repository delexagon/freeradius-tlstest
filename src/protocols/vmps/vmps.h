#pragma once
/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/**
 * $Id$
 *
 * @file src/protocols/vmps/vmps.h
 * @brief Structures and prototypes for Cisco's VLAN Query Protocol
 *
 * @copyright 2007 The FreeRADIUS server project
 * @copyright 2007 Alan DeKok (aland@deployingradius.com)
 */

RCSIDH(vmps_h, "$Id$")

#ifdef __cplusplus
extern "C" {
#endif

#define FR_VQP_HDR_LEN (8)
#define FR_VQP_VERSION (1)

bool fr_vmps_ok(uint8_t const *packet, size_t *packet_len);

int	fr_vmps_decode(TALLOC_CTX *ctx, fr_pair_list_t *out, uint8_t const *data, size_t data_len, unsigned int *code);

ssize_t fr_vmps_packet_size(uint8_t const *data, size_t data_len);

void fr_vmps_print_hex(FILE *fp, uint8_t const *packet, size_t packet_len);

ssize_t fr_vmps_encode(fr_dbuff_t *dbuff, uint8_t const *original,
		       int code, uint32_t id, fr_dcursor_t *cursor) CC_HINT(nonnull(1));

typedef enum {
	FR_VMPS_INVALID = 0,
	FR_VMPS_JOIN_REQUEST = 1,
	FR_VMPS_JOIN_RESPONSE = 2,
	FR_VMPS_RECONFIRM_REQUEST = 3,
	FR_VMPS_RECONFIRM_RESPONSE = 4,
	FR_VMPS_CODE_MAX = 5,
	FR_VMPS_DO_NOT_RESPOND = 256,
} fr_vmps_packet_code_t;

#define FR_VMPS_PACKET_CODE_VALID(_code) (((_code) > 0) && ((_code) < FR_VMPS_CODE_MAX))

extern char const	*fr_vmps_packet_names[FR_VMPS_CODE_MAX];

int fr_vmps_init(void);

void fr_vmps_free(void);

#ifdef __cplusplus
}
#endif
