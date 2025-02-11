#pragma once
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/**
 * $Id$
 *
 * @file lib/server/cf_parse.h
 * @brief API to parse internal format configuration items into native C types.
 *
 * @copyright 2015 The FreeRADIUS server project
 */
RCSIDH(cf_parse_h, "$Id$")

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <freeradius-devel/util/rb.h>
#include <freeradius-devel/util/table.h>
#include <freeradius-devel/util/value.h>
#include <freeradius-devel/server/cf_util.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_BUILTIN_CHOOSE_EXPR
typedef void _mismatch_abinary_m;	//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_abinary;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_bool_m;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_bool;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_char_m;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_char;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_double_m;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_double;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_ethernet_m;	//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_ethernet;	//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_float_m;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_float;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_fripaddr_m;	//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_fripaddr;	//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_ifid_m;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_ifid;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_int32_m;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_int32;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_size_m;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_size;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_time_delta_m;	//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_time_delta;     	//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_time_m;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_time;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_uint16_m;	//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_uint16;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_uint32_m;	//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_uint32;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_uint64_m;	//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_uint64;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_uint8_m_m;	//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_uint8_m;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_uint8;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_void_m;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_void;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_tmpl_m;	//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void _mismatch_vp_tmpl;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.


typedef void _mismatch_default;		//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void conf_type_mismatch;	//!< Dummy type used to indicate FR_TYPE_*/C type mismatch.
typedef void conf_type_invalid;		//!< Dummy type used to indicate invalid FR_TYPE_*.

/** Check the #fr_type_t matches the destination data type
 *
 * Validation macro to check the type of the pointer or offset _p passed in
 * matches the #fr_type_t of the configuration item.
 *
 * Uses various magic builtin precompilation functions, so will likely only
 * work with recent versions of clang and gcc.
 *
 * @note The warnings/errors emitted are usually awful.
 *
 * @param _t a #fr_type_t value with optional ``FR_TYPE_*`` flags.
 * @param _ct data type of global or struct field, obtained with ``__typeof__``.
 * @param _p Pointer or offset.
 */
#  define FR_CONF_TYPE_CHECK(_t, _ct, _p) \
__builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_SUBSECTION), _p, \
__builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_VOID), _p, \
__builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_SIZE) && !((_t) & FR_TYPE_MULTI), \
	__builtin_choose_expr(IS_COMPATIBLE((_ct), size_t *), _p, (_mismatch_size) 0), \
__builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_SIZE) && ((_t) & FR_TYPE_MULTI), \
	__builtin_choose_expr(IS_COMPATIBLE((_ct), size_t **), _p, (_mismatch_size_m) 0), \
__builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_DATE) && !((_t) & FR_TYPE_MULTI), \
	__builtin_choose_expr(IS_COMPATIBLE((_ct), time_t *), _p, (_mismatch_time) 0), \
__builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_DATE) && ((_t) & FR_TYPE_MULTI), \
	__builtin_choose_expr(IS_COMPATIBLE((_ct), time_t **), _p, (_mismatch_time_m) 0), \
__builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_TIME_DELTA) && !((_t) & FR_TYPE_MULTI), \
	__builtin_choose_expr(IS_COMPATIBLE((_ct), fr_time_delta_t *), _p, (_mismatch_time_delta) 0), \
__builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_TIME_DELTA) && ((_t) & FR_TYPE_MULTI), \
	__builtin_choose_expr(IS_COMPATIBLE((_ct), fr_time_delta_t **), _p, (_mismatch_time_delta_m) 0), \
__builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_ETHERNET) && !((_t) & FR_TYPE_MULTI), \
	__builtin_choose_expr(IS_COMPATIBLE((_ct), uint8_t(*)[6]), _p, (_mismatch_ethernet) 0), \
__builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_ETHERNET) && ((_t) & FR_TYPE_MULTI), \
	__builtin_choose_expr(IS_COMPATIBLE((_ct), uint8_t ***), _p, (_mismatch_ethernet_m) 0), \
__builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_IFID) && !((_t) & FR_TYPE_MULTI), \
	__builtin_choose_expr(IS_COMPATIBLE((_ct), uint8_t(*)[8]), _p, (_mismatch_ifid) 0), \
__builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_IFID) && ((_t) & FR_TYPE_MULTI), \
	__builtin_choose_expr(IS_COMPATIBLE((_ct), uint8_t ***), _p, (_mismatch_ifid_m) 0), \
_Generic((_ct), \
	tmpl_t **	: __builtin_choose_expr(((_t) & FR_TYPE_TMPL) && !((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_vp_tmpl) 0), \
	tmpl_t ***	: __builtin_choose_expr(((_t) & FR_TYPE_TMPL) && ((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_tmpl_m) 0), \
	char const **	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_STRING) && !((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_char) 0), \
	char const ***	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_STRING) && ((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_char_m) 0), \
	bool *		: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_BOOL) && !((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_bool) 0), \
	bool **		: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_BOOL) && ((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_bool_m) 0), \
	uint32_t * 	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_UINT32) && !((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_uint32) 0), \
	uint32_t **	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_UINT32) && ((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_uint32_m) 0), \
	fr_ipaddr_t *	: __builtin_choose_expr(((FR_BASE_TYPE(_t) == FR_TYPE_IPV4_ADDR) || \
						(FR_BASE_TYPE(_t) == FR_TYPE_IPV4_PREFIX) || \
						(FR_BASE_TYPE(_t) == FR_TYPE_IPV6_ADDR) || \
						(FR_BASE_TYPE(_t) == FR_TYPE_IPV6_PREFIX) || \
						(FR_BASE_TYPE(_t) == FR_TYPE_COMBO_IP_PREFIX) || \
						(FR_BASE_TYPE(_t) == FR_TYPE_COMBO_IP_ADDR)) || \
						!((_t) & FR_TYPE_MULTI), _p, (_mismatch_fripaddr) 0), \
	fr_ipaddr_t **	: __builtin_choose_expr(((FR_BASE_TYPE(_t) == FR_TYPE_IPV4_ADDR) || \
						(FR_BASE_TYPE(_t) == FR_TYPE_IPV4_PREFIX) || \
						(FR_BASE_TYPE(_t) == FR_TYPE_IPV6_ADDR) || \
						(FR_BASE_TYPE(_t) == FR_TYPE_IPV6_PREFIX) || \
						(FR_BASE_TYPE(_t) == FR_TYPE_COMBO_IP_PREFIX) || \
						(FR_BASE_TYPE(_t) == FR_TYPE_COMBO_IP_ADDR)) && \
						((_t) & FR_TYPE_MULTI), _p, (_mismatch_fripaddr_m) 0), \
	uint8_t const **	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_OCTETS) && !((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_uint8) 0), \
	uint8_t const ***: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_OCTETS) && ((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_uint8_m) 0), \
	uint8_t *	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_UINT8) && !((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_uint8) 0), \
	uint8_t **	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_UINT8) && ((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_uint8_m) 0), \
	uint16_t *	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_UINT16) && !((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_uint16) 0), \
	uint16_t **	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_UINT16) && ((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_uint16_m) 0), \
	int32_t	*	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_INT32) && !((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_int32) 0), \
	int32_t **	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_INT32) && ((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_int32_m) 0), \
	uint64_t *	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_UINT64) && !((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_uint64) 0), \
	uint64_t **	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_UINT64) && ((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_uint64_m) 0), \
	float *		: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_FLOAT32) && !((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_float) 0), \
	float **	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_FLOAT32) && ((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_float_m) 0), \
	double *	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_FLOAT64) && !((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_double) 0), \
	double **	: __builtin_choose_expr((FR_BASE_TYPE(_t) == FR_TYPE_FLOAT64) && ((_t) & FR_TYPE_MULTI), \
			_p, (_mismatch_double_m) 0), \
	default: (conf_type_mismatch)0)))))))))))))
#else
#  define FR_CONF_TYPE_CHECK(_type, _c_type, _ptr_or_offset) _ptr_or_offset
#endif

/** CONF_PARSER which parses a single CONF_PAIR, writing the result to a field in a struct
 *
 * @param[in] _name		of the CONF_PAIR to search for.
 * @param[in] _type		to parse the CONF_PAIR as.
 * @param[in] _struct		contaning the field to write the result to.
 * @param[in] _field		to write the result to.
 */
#  define FR_CONF_OFFSET(_name, _type, _struct, _field) \
	.name = _name, \
	.type = _type, \
	.offset = FR_CONF_TYPE_CHECK((_type), &(((_struct *)NULL)->_field), offsetof(_struct, _field))

/** CONF_PARSER which parses a single CONF_PAIR, writing the result to a field in a struct, recording if a default was used in `<_field>`_is_set
 *
 * @param[in] _name		of the CONF_PAIR to search for.
 * @param[in] _type		to parse the CONF_PAIR as.
 * @param[in] _struct		contaning the field to write the result to.
 * @param[in] _field		to write the result to.
 */
#  define FR_CONF_OFFSET_IS_SET(_name, _type, _struct, _field) \
	.name = _name, \
	.type = (_type) | FR_TYPE_IS_SET, \
	.offset = FR_CONF_TYPE_CHECK((_type), &(((_struct *)NULL)->_field), offsetof(_struct, _field)), \
	.is_set_offset = offsetof(_struct, _field ## _is_set)

/** CONF_PARSER which populates a sub-struct using a CONF_SECTION
 *
 * @param[in] _name		of the CONF_SECTION to search for.
 * @param[in] _flags		any additional flags to set.
 * @param[in] _struct		containing the sub-struct to populate.
 * @param[in] _field		containing the sub-struct to populate.
 * @param[in] _subcs		CONF_SECTION to parse.
 */
#  define FR_CONF_OFFSET_SUBSECTION(_name, _flags, _struct, _field, _subcs) \
	.name = _name, \
	.type = (FR_TYPE_SUBSECTION | _flags), \
	.offset = offsetof(_struct, _field), \
	.subcs = _subcs

/** CONF_PARSER which parses a single CONF_PAIR producing a single global result
 *
 * @param[in] _name		of the CONF_PAIR to search for.
 * @param[in] _type		to parse the CONF_PAIR as.
 * @param[out] _res_p		pointer to a global var, where the result will be written.
 */
#  define FR_CONF_POINTER(_name, _type, _res_p) \
	.name = _name, \
	.type = _type, \
	.data = FR_CONF_TYPE_CHECK((_type), (_res_p), _res_p)

/** CONF_PARSER which parses a single CONF_PAIR producing a single global result, recording if a default was used in `<_res_p>`_is_set
 *
 * @note is set state is recorded in variable `<_res_p>`_is_set.
 *
 * @param[in] _name		of the CONF_PAIR to search for.
 * @param[in] _type		to parse the CONF_PAIR as.
 * @param[out] _res_p		pointer to a global var, where the result will be written.
 */
#  define FR_CONF_POINTER_IS_SET(_name, _type, _res_p) \
	.name = _name, \
	.type = (_type) | FR_TYPE_IS_SET, \
	.data = FR_CONF_TYPE_CHECK((_type), (_res_p), _res_p), \
	.is_set_ptr = _res_p ## _is_set
#  define FR_ITEM_POINTER(_type, _res_p) _type, FR_CONF_TYPE_CHECK((_type), (_res_p), _res_p)

/** A CONF_PARSER multi-subsection
 *
 * Parse multiple instance of a subsection, allocating an array of structs
 * to hold the result.
 *
 * @param _name		name of subsection to search for.
 * @param _type		Must be FR_TYPE_SUBSECTION | FR_TYPE_MULTI and any optional flags.
 * @param _struct	instance data struct.
 * @param _field	field in instance data struct.
 * @param _subcs	CONF_PARSER array to use to parse subsection data.
 */
#  define FR_CONF_SUBSECTION_ALLOC(_name, _type, _struct, _field, _subcs) \
	.name = _name, \
	.type = (_type), \
	.offset = FR_CONF_TYPE_CHECK((_type), &(((_struct *)NULL)->_field), offsetof(_struct, _field)), \
	.subcs = _subcs, \
	.subcs_size = sizeof(**(((_struct *)0)->_field))

/** CONF_PARSER entry which doesn't fill in a pointer or offset, but relies on functions to record values
 *
 * @param[in] _name		name of pair to search for.
 * @param[in] _type		base type to parse pair as.
 * @param[in] _func		to use to record value.
 * @param[in] _dflt_func	to use to get defaults from a 3rd party library.
 */
#  define FR_CONF_FUNC(_name, _type, _func, _dflt_func) \
	.name = _name, \
	.type = _type, \
	.func = _func, \
	.dflt_func = _dflt_func

/** CONF_PARSER entry which runs CONF_PARSER entries for a subsection without any output
 *
 * @param[in] _name		of pair to search for.
 * @param[in] _flags		any extra flags to add.
 * @param[in] _subcs		to use to get defaults from a 3rd party library.
 */
#  define FR_CONF_SUBSECTION_GLOBAL(_name, _flags, _subcs) \
	.name = _name, \
	.type = FR_TYPE_SUBSECTION, \
	.subcs = _subcs

/** CONF_PARSER entry which raises an error if a matching CONF_PAIR is found
 *
 * @param[in] _name		of pair to search for.
 * @param[in] _type		type, mostly unused.
 * @param[in] _struct		where the result was previously written.
 * @param[in] _field		in the struct where the result was previously written.
 */
#define FR_CONF_DEPRECATED(_name, _type, _struct, _field) \
	.name = _name, \
	.type = (_type) | FR_TYPE_DEPRECATED

/*
 *  Instead of putting the information into a configuration structure,
 *  the configuration file routines MAY just parse it directly into
 *  user-supplied variables.
 */
#define FR_TYPE_SUBSECTION	102

/*
 *	It's a developer option and should be used carefully.
 */
#define FR_TYPE_HIDDEN     	0

/** @name #CONF_PARSER type flags
 *
 * These flags should be or'd with another FR_TYPE_* value to create validation
 * rules for the #cf_pair_parse function.
 *
 * @note File FR_TYPE_FILE_* types have a base type of string, so they're validated
 *	 correctly by the config parser.
 * @{
 */
#define FR_TYPE_DEPRECATED		(1 << 10) 			//!< If a matching #CONF_PAIR is found,
									//!< error out with a deprecated message.
#define FR_TYPE_REQUIRED		(1 << 11) 			//!< Error out if no matching #CONF_PAIR
									//!< is found, and no dflt value is set.
#define FR_TYPE_ATTRIBUTE		(1 << 12) 			//!< Value must resolve to attribute in dict
									//!< (deprecated, use #FR_TYPE_TMPL).
#define FR_TYPE_SECRET			(1 << 13)			 //!< Only print value if debug level >= 3.

#define FR_TYPE_FILE_INPUT		((1 << 14) | FR_TYPE_STRING)	//!< File matching value must exist,
								     	//!< and must be readable.
#define FR_TYPE_FILE_OUTPUT		((1 << 15) | FR_TYPE_STRING)	//!< File matching value must exist,
									//!< and must be writable.

#define FR_TYPE_XLAT			(1 << 16) 			//!< string will be dynamically expanded.
#define FR_TYPE_TMPL			(1 << 17) 			//!< CONF_PAIR should be parsed as a template.

#define FR_TYPE_MULTI			(1 << 18) 			//!< CONF_PAIR can have multiple copies.
#define FR_TYPE_NOT_EMPTY		(1 << 19)			//!< CONF_PAIR is required to have a non zero
									//!< length value.
#define FR_TYPE_FILE_EXISTS		((1 << 20) | FR_TYPE_STRING)	//!< File matching value must exist

#define FR_TYPE_IS_SET			(1 << 21)			//!< Write whether this config item was
									//!< left as the default to is_set_offset
									//!< or is_set_ptr.
#define FR_TYPE_OK_MISSING     		(1 << 22) 			//!< OK if it's missing

#define FR_BASE_TYPE(_t)		(0xff & (_t))
/** @} */

/** @name #CONF_PARSER flags checks
 *
 * @{
 */
#define fr_rule_deprecated(_rule)	((_rule)->type & FR_TYPE_DEPRECATED)

#define fr_rule_required(_rule)		((_rule)->type & FR_TYPE_REQUIRED)

#define fr_rule_secret(_rule)		((_rule)->type & FR_TYPE_SECRET)

#define fr_rule_file_input(_rule)	(((_rule)->type & FR_TYPE_FILE_INPUT) == FR_TYPE_FILE_INPUT)

#define fr_rule_file_output(_rule)	(((_rule)->type & FR_TYPE_FILE_OUTPUT) == FR_TYPE_FILE_OUTPUT)


#define fr_rule_multi(_rule)		((_rule)->type & FR_TYPE_MULTI)

#define fr_rule_not_empty(_rule)	((_rule)->type & FR_TYPE_NOT_EMPTY)

#define fr_rule_is_set(_rule)		((_rule)->type & FR_TYPE_IS_SET)

#define fr_rule_ok_missing(_rule)	((_rule)->type & FR_TYPE_OK_MISSING)

#define fr_rule_file_exists(_rule)	(((_rule)->type & FR_TYPE_FILE_EXISTS) == FR_TYPE_FILE_EXISTS)

#define fr_rule_dflt(_rule)		((_rule)->dflt || (_rule)->dflt_func)

#define fr_rule_is_attribute(_rule)	((_rule)->type & FR_TYPE_ATTRIBUTE)

#define fr_rule_is_xlat(_rule)		((_rule)->type & FR_TYPE_XLAT)

#define fr_rule_is_tmpl(_rule)		((_rule)->type & FR_TYPE_TMPL)
/** @} */

#define FR_SIZE_COND_CHECK(_name, _var, _cond, _new)\
do {\
	if (!(_cond)) {\
		WARN("Ignoring \"" _name " = %zu\", forcing to \"" _name " = %zu\"", _var, _new);\
		_var = _new;\
	}\
} while (0)

#define FR_SIZE_BOUND_CHECK(_name, _var, _op, _bound) FR_SIZE_COND_CHECK(_name, _var, (_var _op _bound), _bound)

#define FR_INTEGER_COND_CHECK(_name, _var, _cond, _new)\
do {\
	if (!(_cond)) {\
		WARN("Ignoring \"" _name " = %i\", forcing to \"" _name " = %i\"", _var, _new);\
		_var = _new;\
	}\
} while (0)

#define FR_INTEGER_BOUND_CHECK(_name, _var, _op, _bound) FR_INTEGER_COND_CHECK(_name, _var, (_var _op _bound), _bound)

#define FR_TIME_DELTA_COND_CHECK(_name, _var, _cond, _new)\
do {\
	if (!(_cond)) {\
		WARN("Ignoring \"" _name " = %pV\", forcing to \"" _name " = %pV\"", \
		     fr_box_time_delta(_var), fr_box_time_delta(_new));\
		_var = _new;\
	}\
} while (0)

#define FR_TIME_DELTA_BOUND_CHECK(_name, _var, _op, _bound)\
do {\
	if (!fr_time_delta_cond(_var, _op, _bound)) { \
		WARN("Ignoring \"" _name " = %pV\", forcing to \"" _name " = %pV\"",\
		     fr_box_time_delta(_var),\
		     fr_box_time_delta(_bound));\
		_var = _bound;\
	}\
} while (0)

extern bool check_config;

typedef struct CONF_PARSER CONF_PARSER;

/** Callback for performing custom parsing of a #CONF_SECTION or CONF_PAIR
 *
 * @param[in] ctx	to allocate any data in.
 * @param[out] out	Where to write the result of parsing.
 * @param[in] parent	The base address of the structure.
 * @param[in] ci	The #CONF_SECTION or #CONF_PAIR to parse.
 * @param[in] rule	Parse rules - How the #CONF_PAIR or #CONF_SECTION should be converted.
 * @return
 *	- 0 on success.
 *	- -1 on failure.
 */
typedef int (*cf_parse_t)(TALLOC_CTX *ctx, void *out, void *parent, CONF_ITEM *ci, CONF_PARSER const *rule);

/** Callback for producing dynamic defaults from 3rd party libraries
 *
 * @param[out] out	Where to write default conf pair.
 * @param[in] parent	being populated.
 * @param[in] cs	to allocate pair in.
 * @param[in] quote	to use when allocing the pair.  Provided as a convenience.
 * @param[in] rule	to produce default for.
 * @return
 *	- 0 on success.
 *	- -1 on failure.
 */
typedef int (*cf_dflt_t)(CONF_PAIR **out, void *parent, CONF_SECTION *cs, fr_token_t quote, CONF_PARSER const *rule);

/** Defines a #CONF_PAIR to C data type mapping
 *
 * Is typically used to define mappings between module sections, and module instance structs.
 * May also be used to set global configuration options.
 *
 * Offset/data values should be set using #FR_CONF_OFFSET or #FR_CONF_POINTER.
 *
 * Example with #FR_CONF_OFFSET :
 @code{.c}
   static CONF_PARSER module_config[] = {
   	{ FR_CONF_OFFSET("example", FR_TYPE_STRING | FR_TYPE_NOT_EMPTY, example_instance_t, example), .dflt = "default_value" },
   	CONF_PARSER_TERMINATOR
   }
 @endcode
 *
 * Example with #FR_CONF_POINTER :
 @code{.c}
   static CONF_PARSER global_config[] = {
   	{ FR_CONF_POINTER("example", FR_TYPE_STRING | FR_TYPE_NOT_EMPTY, &my_global), .dflt = "default_value" },
   	CONF_PARSER_TERMINATOR
   }
 @endcode
 *
 * @see FR_CONF_OFFSET
 * @see FR_CONF_POINTER
 * @see cf_section_parse
 * @see cf_pair_parse
 */
struct CONF_PARSER {
	char const	*name;			//!< Name of the #CONF_ITEM to parse.
	char const	*ident2;		//!< Second identifier for #CONF_SECTION.

	uint32_t	type;			//!< A #fr_type_t value, may be or'd with one or more FR_TYPE_* flags.
						//!< @see cf_pair_parse.

	size_t		offset;			//!< Relative offset of field or structure to write the parsed value to.
						//!< When #type is set to #FR_TYPE_SUBSECTION, may be used to specify
						//!< a base offset to add to all offsets contained within the
						//!< subsection.
						//!< @note Must be used exclusively to #data.

	void		*data;			//!< Pointer to a static variable to write the parsed value to.
						//!< @note Must be used exclusively to #offset.

	cf_parse_t	func;			//!< Override default parsing behaviour for the specified type with
						//!< a custom parsing function.

	cf_parse_t	on_read;		//!< Function to call as the item is being read, just after
						//!< it has been allocated and initialized.

	void const	*uctx;			//!< User data accessible by the #cf_parse_t func.  Useful for
						///< building reusable functions.

	/** Where to write status if FR_TYPE_IS_DEFAULT is set
	 *
	 * @note Which field is used, is determined by whether
	 *	data ptr is set.
	 */
	union {
		size_t		is_set_offset;	//!< If type contains FR_TYPE_IS_DEFAULT write status to bool.
						//!< at this address.
		void		*is_set_ptr;	//!< If type contains FR_TYPE_IS_DEFAULT write status to ptr
						//!< at this address.
	};

	union {
		struct {
			char const	*dflt;		//!< Default as it would appear in radiusd.conf.

			cf_dflt_t	dflt_func;	//!< Function to produce dynamic defaults.
		};

		struct {
			struct CONF_PARSER const *subcs;//!< When type is set to #FR_TYPE_SUBSECTION, should
							//!< be a pointer to the start of another array of
							//!< #CONF_PARSER structs, forming the subsection.
			size_t		subcs_size;	//!< If non-zero, allocate structs of this size to hold
							//!< the parsed data.
			char const	*subcs_type;	//!< Set a specific talloc type for subcs structures.
		};
	};

	fr_token_t	quote;			//!< Quoting around the default value.  Only used for templates.
};

typedef struct {
	fr_table_num_sorted_t const	*table;
	size_t			*len;
} cf_table_parse_ctx_t;

#define CONF_PARSER_TERMINATOR	{ .name = NULL, .type = ~(UINT32_MAX - 1), \
				  .offset = 0, .data = NULL, .dflt = NULL, .quote = T_INVALID }

#define CONF_PARSER_PARTIAL_TERMINATOR	{ .name = NULL, .type = ~(UINT32_MAX - 1), \
					  .offset = 1, .data = NULL, .dflt = NULL, .quote = T_INVALID }

#define CF_FILE_NONE   (0)
#define CF_FILE_ERROR  (1)
#define CF_FILE_CONFIG (1 << 2)
#define CF_FILE_MODULE (1 << 3)

void		cf_pair_debug(CONF_SECTION const *cs, CONF_PAIR *cp, CONF_PARSER const *rule);

/*
 *	Type validation and conversion
 */
int		cf_pair_to_value_box(TALLOC_CTX *ctx, fr_value_box_t *out, CONF_PAIR *cp, CONF_PARSER const *rule)
		CC_HINT(nonnull(2, 3, 4));

int		cf_pair_parse_value(TALLOC_CTX *ctx, void *out, void *base, CONF_ITEM *ci, CONF_PARSER const *rule)
		CC_HINT(nonnull(2, 4, 5));

int		cf_pair_parse(TALLOC_CTX *ctx, CONF_SECTION *cs, char const *name,
			      unsigned int type, void *data, char const *dflt, fr_token_t dflt_quote) CC_HINT(nonnull(2,3));
int		cf_section_parse(TALLOC_CTX *ctx, void *base, CONF_SECTION *cs);
int		cf_section_parse_pass2(void *base, CONF_SECTION *cs);

/*
 *	Runtime parse rules
 */
#define		cf_section_rule_push(_cs, _rule) _cf_section_rule_push(_cs, _rule, __FILE__, __LINE__)
int		_cf_section_rule_push(CONF_SECTION *cs, CONF_PARSER const *rule, char const *filename, int lineno);
#define		cf_section_rules_push(_cs, _rule) _cf_section_rules_push(_cs, _rule, __FILE__, __LINE__)
int		_cf_section_rules_push(CONF_SECTION *cs, CONF_PARSER const *rules, char const *filename, int lineno);

/*
 *	Generic parsing callback functions
 */
int		cf_table_parse_int(UNUSED TALLOC_CTX *ctx, void *out, UNUSED void *parent,
			 	   CONF_ITEM *ci, CONF_PARSER const *rule);

int		cf_table_parse_uint32(UNUSED TALLOC_CTX *ctx, void *out, UNUSED void *parent,
				      CONF_ITEM *ci, CONF_PARSER const *rule);

int		cf_table_parse_int32(UNUSED TALLOC_CTX *ctx, void *out, UNUSED void *parent,
				     CONF_ITEM *ci, CONF_PARSER const *rule);

int		cf_parse_uid(TALLOC_CTX *ctx, void *out, UNUSED void *parent,
			     CONF_ITEM *ci, CONF_PARSER const *rule);

int		cf_parse_gid(TALLOC_CTX *ctx, void *out, UNUSED void *parent,
			     CONF_ITEM *ci, CONF_PARSER const *rule);

#ifdef __cplusplus
}
#endif
