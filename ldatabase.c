#include "ldatabase.h"
struct UserMessage Msg = {
    .TYPEWAY = "Type-A",
    .NAME = "LLP",
    .IDNUMBER = "20291111",
    .NFCID1 = "01020304"
};
/*LLP ADD<*/
static int GetUserData_CallBack(void *data, int argc, char **argv, char **azColName){
    if (argc <= 0)
      return -1;
    for (int i = 0; i < argc; i++){
        if(strlen(azColName[i]) == 2)
            fprintf(data,"\n");
        fprintf(data,"|%s", argv[i] ? argv[i] : "NULL");
    }
    return 0;
}
static int GetUserData_By_Id(FILE *stream,sqlite3 *db,const char *data,const char *TableName){
    char sql[160];
    snprintf(sql, 159, "SELECT * FROM %s WHERE IDNUMBER = %s", TableName, data);
    return sqlite3_exec(db, sql, GetUserData_CallBack, stream, NULL);
}
static int GetUserData_By_Limit(FILE *stream,sqlite3 *db,const char *data,const char *TableName){
    char sql[160];
    snprintf(sql, 159, "SELECT * FROM %s ORDER BY ID LIMIT %d", TableName,atoi(data));
    return sqlite3_exec(db, sql, GetUserData_CallBack, stream, NULL);
}
static int GetUserData_By_All(FILE *stream,sqlite3 *db,const char *data,const char *TableName){
    char sql[160];
    snprintf(sql, 159, "SELECT * FROM %s ORDER BY ID", TableName);
    return sqlite3_exec(db, sql, GetUserData_CallBack, stream, NULL);
}
static int callback(void *data, int argc, char **argv, char **azColName)
{
    int *State = data;

    if (argc >= 1)
        *State = 1;

    for (int i = 0; i < argc; i++)
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");

    return 0;
}

static int Save_UserData(void *data, int argc, char **argv, char **azColName)
{
    if (data == NULL || argc < 3 || azColName == NULL)
        return -1;
    
    struct UserMessage *userMessage = data;
    strncpy(userMessage->IDNUMBER, argv[0], 48); // sizeof(serMessage->IDNUMBER) = 50;
    strncpy(userMessage->NFCID1, argv[1], 48);   //
    strncpy(userMessage->NAME, argv[2], 48);
    // strncpy(userMessage->IDNUMBER, argv[3], 48);
    return 1;
}

int Create_Table(const char *DatabaseName, const char *TableName)
{
    sqlite3 *db;
    int rc;

    // 打开数据库
    if (DatabaseName == NULL || TableName == NULL || strlen(TableName) >= 40)
        return -1;
    rc = sqlite3_open(DatabaseName, &db);
    if (rc)
        return -2;
    // 创建表
    char sql[160] = {0};
    snprintf(sql, 159, "CREATE TABLE IF NOT EXISTS %s (ID INT PRIMARY KEY NOT NULL, NFCID1 CHAR(50) NOT NULL,NAME CHAR(50) NOT NULL,IDNUMBER CHAR(50) NOT NULL)", TableName);
    rc = sqlite3_exec(db, sql, 0, 0, NULL);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return -3;
    }
    sqlite3_close(db);
    return 1;
}

int User_Add(const char *DatabaseName, const char *NFC_Data)
{
    sqlite3 *db;
    int rc;

    if (DatabaseName == NULL)
        return -1;
    rc = sqlite3_open(DatabaseName, &db);
    if (rc)
        return -2;
    /*NFC_Data format:INSERT INTO MyTable (ID,NFCID1,NAME,IDNUMBER) VALUES (%d,%s,%s,%s)*/
    const char *sql = NFC_Data;
    rc = sqlite3_exec(db, sql, 0, 0, NULL);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return -3;
    }
    printf("User data OK\n");
    sqlite3_close(db);
    return 1;
}

int Identity_Confirm(const char *DatabaseName, struct UserMessage *msg, const char *TableName)
{
    sqlite3 *db;
    int rc;
    int state = 0;

    if (DatabaseName == NULL || TableName == NULL || strlen(TableName) >= 40)
        return -1;
    rc = sqlite3_open(DatabaseName, &db);
    if (rc)
        return -2;
    char sql[160];
    snprintf(sql, 159, "SELECT * FROM %s WHERE IDNUMBER = %s", TableName, msg->IDNUMBER);
    rc = sqlite3_exec(db, sql, callback, &state, NULL);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return -3;
    }
    printf("the state is %d\n", state);
    sqlite3_close(db);
    return state;
}

int GetUserIdentityData(const char *DatabaseName, struct UserMessage *DataMsg, const char *TableName)
{
    sqlite3 *db;
    int rc;
    rc = sqlite3_open(DatabaseName, &db);
    if (rc)
        return (-1);
    char sql[249];
    if (TableName == NULL || strlen(TableName) >= 40)
        return -2;
    snprintf(sql, 248, "SELECT * FROM %s WHERE NFCID1=%s OR NAME=%s OR IDNUMBER=%s", TableName, DataMsg->NFCID1, DataMsg->NAME, DataMsg->IDNUMBER);
    rc = sqlite3_exec(db, sql, Save_UserData, DataMsg, NULL);
    return 1;
}

int User_Remove(const char *DatabaseName, const char *NFCID1, const char *IDNUMBER, const char *TableName)
{
    sqlite3 *db;
    int rc;
    char sql[100] = {0};

    if (!DatabaseName || !TableName || strlen(TableName) >= 40)
        return -1;

    rc = sqlite3_open(DatabaseName, &db);
    if (rc)
        return (-2);
    snprintf(sql, 100, "DELETE FROM %s WHERE NFCID1 = %s OR IDNUMBER = %s", TableName, NFCID1, IDNUMBER);

    rc = sqlite3_exec(db, sql, 0, 0, NULL);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return -3;
    }

    sqlite3_close(db);
    return 1;
}
int GetUserData(const char *DatabaseName,const char *TableName,const char *data,enum Query_Way QueryWay){
    char *buffer;
    size_t size;
    sqlite3 *db;
    int rc;
    
    if (!DatabaseName || !TableName || strlen(TableName) >= 40)
        return -1;

    rc = sqlite3_open(DatabaseName, &db);
    if (rc)
        return (-2);

    FILE *stream = open_memstream(&buffer,&size);
    if (!stream)
       return -3;
    fprintf(stream,"hello\n");
    fflush(stream);

    
    switch (QueryWay)
    {
    case IDNUMBER:
        rc = GetUserData_By_Id(stream,db,data,TableName);
        break;
    case NAME:
        break;
    case NFCID1:
        break;   
    case LIMIT:
        rc = GetUserData_By_Limit(stream,db,data,TableName);
        break;
    case ALL:
        rc = GetUserData_By_All(stream,db,data,TableName);
        break;
    default:
        break;
    }
    fclose(stream);
    printf("buffer = %s\n",buffer);
    sqlite3_close(db);
   
    return rc;
}

int UpdateData(const char *DatabaseName,const char *sql,const char *TableName){
    sqlite3 *db;
    int rc;
    char sql1[160] = {0};

    if (!DatabaseName || !TableName || strlen(TableName) >= 40)
        return -1;

    rc = sqlite3_open(DatabaseName, &db);
    if (rc)
        return (-2);
    snprintf(sql1, 160, "UPDATE %s %s",TableName,sql);
    printf("%s \n",sql1);
    rc = sqlite3_exec(db, sql1, 0, 0, NULL);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return -3;
    }

    sqlite3_close(db);
    return 1;
}