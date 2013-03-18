#ifndef __MONGODB_H__
#define __MONGODB_H__
#include <string>

#include "tcptrace.h"
#include "tstat.h"
#include "mongo/client/dbclient.h"

using namespace std;

class MongoUtils{
public:
	int connect_db(string addr = string("127.0.0.1"), unsigned short port = 27017);
	void use_table(string tname);
	void clean_table(string tname);
	int insert_tcptrace_log(Tcptrace &tt);
	int insert_tstat_log(Tstat &ts);
private:
	mongo::DBClientConnection conn;
	string table_name;
};

#endif
