/**
 * @file ldatabase.h
 * @brief Header file for the user database management functions.
 *
 * This file contains declarations of structures and functions to interact with
 * a SQLite database for managing user data.
 */

#ifndef L_DATA
#define L_DATA

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <dlfcn.h>
#include <string.h>
#include <wchar.h>

/**
 * @brief Structure to hold user message data.
 */
struct UserMessage {
    char *TYPEWAY;   /**< Type of message */
    char *NAME;      /**< User's name */
    char *IDNUMBER;  /**< User's identification number */
    char NFCID1[50]; /**< NFC ID of the user */
};

/**
 * @brief Enum to define different query methods.
 */
enum Query_Way {
    IDNUMBER = 0,  /**< Query by ID number */
    NAME = 1,       /**< Query by name */
    NFCID1 = 2,     /**< Query by NFC ID */
    LIMIT,          /**< Query with a limit on the number of results */
    ALL             /**< Query for all records */
};

/**
 * @brief Creates a new table in the SQLite database.
 *
 * @param DatabaseName The name of the database.
 * @param TableName The name of the table to create.
 * @return 1 on success, -1 if DatabaseName or TableName is NULL or TableName is too long, -2 on open error, -3 on create error.
 */
int Create_Table(const char *DatabaseName, const char *TableName);

/**
 * @brief Adds a new user to the database.
 *
 * @param DatabaseName The name of the database.
 * @param NFC_Data The data to insert into the database.
 * @return 1 on success, -1 if DatabaseName is NULL, -2 on open error, -3 on insert error.
 */
int User_Add(const char *DatabaseName, const char *NFC_Data);

/**
 * @brief Confirms a user's identity by checking the database.
 *
 * @param DatabaseName The name of the database.
 * @param msg Pointer to a UserMessage structure containing the user's data.
 * @param TableName The name of the table to query.
 * @return 1 if the user is confirmed, -1 if DatabaseName or TableName is invalid, -2 on open error, -3 on query error.
 */
int Identity_Confirm(const char *DatabaseName, struct UserMessage *msg, const char *TableName);

/**
 * @brief Retrieves user identity data from the database.
 *
 * @param DatabaseName The name of the database.
 * @param DataMsg Pointer to a UserMessage structure to store the retrieved data.
 * @param TableName The name of the table to query.
 * @return 1 on success, -1 on open error, -2 if TableName is too long.
 */
int GetUserIdentityData(const char *DatabaseName, struct UserMessage *DataMsg, const char *TableName);

/**
 * @brief Removes a user from the database.
 *
 * @param DatabaseName The name of the database.
 * @param NFCID1 The NFC ID of the user to remove.
 * @param IDNUMBER The ID number of the user to remove.
 * @param TableName The name of the table from which to remove the user.
 * @return 1 on success, -1 if any parameter is invalid, -2 on open error, -3 on delete error.
 */
int User_Remove(const char *DatabaseName, const char *NFCID1, const char *IDNUMBER, const char *TableName);

/**
 * @brief Gets user data from the database based on the query type.
 *
 * @param DatabaseName The name of the database.
 * @param TableName The name of the table to query.
 * @param data The data to use for the query.
 * @param QueryWay The type of query to perform.
 * @return The result of the query operation.
 */
int GetUserData(const char *DatabaseName, const char *TableName, const char *data, enum Query_Way QueryWay);

/**
 * @brief Updates data in the database.
 *
 * @param DatabaseName The name of the database.
 * @param sql The SQL update statement.
 * @param TableName The name of the table to update.
 * @return 1 on success, -1 if any parameter is invalid, -2 on open error, -3 on update error.
 */
int UpdateData(const char *DatabaseName, const char *sql, const char *TableName);

#endif // L_DATA