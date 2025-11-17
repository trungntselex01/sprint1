/*
 * ms_json_maker.h
 *
 *  Created on: Nov 14, 2025
 *      Author: MYPC
 */

#ifndef SM_JSON_MAKER_H_
#define SM_JSON_MAKER_H_



#ifdef  __cplusplus
extern "C" {
#endif

/** @defgroup makejoson Make JSON.
  * @{ */

/** Open a JSON object in a JSON string.
  * @param dest Pointer to the end of JSON under construction.
  * @param name Pointer to null-terminated string or null for unnamed.
  * @return Pointer to the new end of JSON under construction. */
char* json_objOpen( char* dest, char const* name );

/** Close a JSON object in a JSON string.
  * @param dest Pointer to the end of JSON under construction.
  * @return Pointer to the new end of JSON under construction. */
char* json_objClose( char* dest );

/** Used to finish the root JSON object. After call json_objClose().
  * @param dest Pointer to the end of JSON under construction.
  * @return Pointer to the new end of JSON under construction. */
char* json_end( char* dest );

/** Open an array in a JSON string.
  * @param dest Pointer to the end of JSON under construction.
  * @param name Pointer to null-terminated string or null for unnamed.
  * @return Pointer to the new end of JSON under construction. */
char* json_arrOpen( char* dest, char const* name );

/** Close an array in a JSON string.
  * @param dest Pointer to the end of JSON under construction.
  * @return Pointer to the new end of JSON under construction. */
char* json_arrClose( char* dest );

/** Add a text property in a JSON string.
  * @param dest Pointer to the end of JSON under construction.
  * @param name Pointer to null-terminated string or null for unnamed.
  * @param value A valid null-terminated string with the value.
  *              Backslash escapes will be added for special characters.
  * @return Pointer to the new end of JSON under construction. */
char* json_str( char* dest, char const* name, char const* value );

/** Add a boolean property in a JSON string.
  * @param dest Pointer to the end of JSON under construction.
  * @param name Pointer to null-terminated string or null for unnamed.
  * @param value Zero for false. Non zero for true.
  * @return Pointer to the new end of JSON under construction. */
char* json_bool( char* dest, char const* name, int value );

/** Add a null property in a JSON string.
  * @param dest Pointer to the end of JSON under construction.
  * @param name Pointer to null-terminated string or null for unnamed.
  * @return Pointer to the new end of JSON under construction. */
char* json_null( char* dest, char const* name );

/** Add an integer property in a JSON string.
  * @param dest Pointer to the end of JSON under construction.
  * @param name Pointer to null-terminated string or null for unnamed.
  * @param value Value of the property.
  * @return Pointer to the new end of JSON under construction. */
char* json_int( char* dest, char const* name, int value );

/** Add an unsigned integer property in a JSON string.
  * @param dest Pointer to the end of JSON under construction.
  * @param name Pointer to null-terminated string or null for unnamed.
  * @param value Value of the property.
  * @return Pointer to the new end of JSON under construction. */
char* json_uint( char* dest, char const* name, unsigned int value );

/** Add a long integer property in a JSON string.
  * @param dest Pointer to the end of JSON under construction.
  * @param name Pointer to null-terminated string or null for unnamed.
  * @param value Value of the property.
  * @return Pointer to the new end of JSON under construction. */
char* json_long( char* dest, char const* name, long int value );

/** Add an unsigned long integer property in a JSON string.
  * @param dest Pointer to the end of JSON under construction.
  * @param name Pointer to null-terminated string or null for unnamed.
  * @param value Value of the property.
  * @return Pointer to the new end of JSON under construction. */
char* json_ulong( char* dest, char const* name, unsigned long int value );

/** Add a long long integer property in a JSON string.
  * @param dest Pointer to the end of JSON under construction.
  * @param name Pointer to null-terminated string or null for unnamed.
  * @param value Value of the property.
  * @return Pointer to the new end of JSON under construction. */
char* json_verylong( char* dest, char const* name, long long int value );

/** Add a double precision number property in a JSON string.
  * @param dest Pointer to the end of JSON under construction.
  * @param name Pointer to null-terminated string or null for unnamed.
  * @param value Value of the property.
  * @return Pointer to the new end of JSON under construction. */
char* json_double( char* dest, char const* name, double value );

/** @ } */

#ifdef  __cplusplus
}
#endif

#endif /* SM_JSON_MAKER_H_ */
