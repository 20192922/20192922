! \mainpage 
## Sqlite Basic Command
**1.Create Table:**  
CREATE TABLE IF NOT EXISTS %s (ID INT PRIMARY KEY NOT NULL, NFCID1 CHAR(50) NOT NULL,NAME  
CHAR(50) NOT NULL,IDNUMBER CHAR(50) NOT NULL)"  
**2.Query Table And Exit:**   
.table .quit   
**3.Select User Data:**    
SELECT * FROM %s WHERE NFCID1=%s OR NAME=%s OR IDNUMBER=%s  
SELECT * FROM YourTable LIMIT 2  
SELECT * FROM YourTable ORDER BY ID ASC LIMIT 2   
**4.User Add:**    
INSERT INTO YourTable (*,*,...) VALUES (*,*,...)  
INSERT INTO YourTable (ID,NFCID1,NAME,IDNUMBER) VALUES (%d,%s,%s,%s)  
INSERT INTO YourTable (ID,NFCID1,NAME,IDNUMBER) VALUES (1,'01020304','LLP','20270112')  
**5.User Remove:**   
DELETE FROM %s WHERE NFCID1 = %s OR IDNUMBER = %s  
DELETE FROM %s WHERE NFCID1 = %s AND IDNUMBER = %s  
**6.User Update:**  
UPDATE YourTable SET NAME='llh' WHERE ID=0 AND IDNUMBER='20280112'  
UPDATE YourTable SET ID='2' WHERE ID=-1; LLP:Mind the single quote  

## A Simple
```bash
cgel@ubuntu:~/Documents/base_c/sqlite/builddir$ ./sqlite3 LLP
SQLite version 3.44.0 2023-11-01 11:23:50
Enter ".help" for usage hints.
sqlite> .table
YourTable
sqlite> SELECT * FROM YourTable
   ...> ;
1|01020304|llp|20270112
2|01020305|llh|20280112
0|01020305|llk|20280112
-1|01020305|LLh|20280112
3|01020305|llg|20280112
sqlite> SELECT * FROM YourTable LIMIT 2;
1|01020304|llp|20270112
2|01020305|llh|20280112
sqlite> INSERT INTO YourTable (ID,NFCID1,NAME,IDNUMBER) VALUES (-2,'030303','kk','20202020');
sqlite> SELECT * FROM YourTable WHERE NAME='kk';
-2|030303|kk|20202020
sqlite> DELETE FROM YourTable WHERE NAME='llk';
sqlite> UPDATE YourTable SET NAME='lkk' WHERE ID=-2 AND NAME='kk';
sqlite> SELECT * FROM YourTable;
1|01020304|llp|20270112
2|01020305|llh|20280112
-1|01020305|LLh|20280112
3|01020305|llg|20280112
-2|030303|lkk|20202020
sqlite> .quit
cgel@ubuntu:~/Documents/base_c/sqlite/builddir$ 
```

## The C Interface
1.sqlite3_open ==>get sqlite3 *db;
2.use sql and db ==>sqlite3_exec(db, sql, 0, 0, NULL);
or  rc = sqlite3_exec(db, sql, callback, &state, NULL);
3.sqlite3_close(db);
```c
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
```