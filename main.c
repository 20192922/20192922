#include "ldatabase.h"
/*sqlite command
1.Create Table:
CREATE TABLE IF NOT EXISTS %s (ID INT PRIMARY KEY NOT NULL, NFCID1 CHAR(50) NOT NULL,NAME CHAR(50) NOT NULL,IDNUMBER CHAR(50) NOT NULL)"
2.Query Table And Exit:
.table .quit
3.Select User Data:
SELECT * FROM %s WHERE NFCID1=%s OR NAME=%s OR IDNUMBER=%s
SELECT * FROM YourTable LIMIT 2
SELECT * FROM YourTable ORDER BY ID ASC LIMIT 2
4.User Add:
INSERT INTO YourTable (*,*,...) VALUES (*,*,...)
INSERT INTO YourTable (ID,NFCID1,NAME,IDNUMBER) VALUES (%d,%s,%s,%s)
INSERT INTO YourTable (ID,NFCID1,NAME,IDNUMBER) VALUES (1,'01020304','LLP','20270112')
5.User Remove:
DELETE FROM %s WHERE NFCID1 = %s OR IDNUMBER = %s
DELETE FROM %s WHERE NFCID1 = %s AND IDNUMBER = %s
6.User Update;
UPDATE YourTable SET NAME='llh' WHERE ID=0 AND IDNUMBER='20280112'
UPDATE YourTable SET ID='2' WHERE ID=-1; LLP:Mind the single quote
*/
int main()
{
    int n = 0;

    Create_Table("LLP","YourTable");
  
    n = User_Add("LLP","INSERT INTO YourTable (ID,NFCID1,NAME,IDNUMBER) VALUES (1,'01020304','LLP','20270112')");
    printf("-%d\n",n);
    n = User_Add("LLP","INSERT INTO YourTable (ID,NFCID1,NAME,IDNUMBER) VALUES (-1,'01020305','LLh','20280112')");
    printf("-%d\n",n);
    n = User_Add("LLP","INSERT INTO YourTable (ID,NFCID1,NAME,IDNUMBER) VALUES (0,'01020305','LLh','20280112')");
    printf("-%d\n",n);
    n = User_Add("LLP","INSERT INTO YourTable (ID,NFCID1,NAME,IDNUMBER) VALUES (3,'01020305','LLh','20280112')");
    printf("-%d\n",n);

    n = GetUserData("LLP","YourTable","20280112",IDNUMBER);
    printf("-%d\n",n);
    n = GetUserData("LLP","YourTable","20280112",ALL);
    printf("-%d\n",n);
    n = GetUserData("LLP","YourTable","2",LIMIT);
    printf("-%d\n",n);

    n = UpdateData("LLP","SET NAME='llg' WHERE ID=3 AND IDNUMBER='20280112'","YourTable");
    printf("-%d\n",n);
    n = GetUserData("LLP","YourTable","20280112",ALL);
}
