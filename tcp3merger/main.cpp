#include <iostream>
#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>

#include "tcptrace.h"
#include "tstat.h"
#include "mongodb.h"

using namespace std;

int main(int argc, char *argv[]){
	ifstream file_tcptrace, file_tstat;
	MongoUtils mu;

	mu.connect_db("219.228.106.205", 27017);
	mu.clean_table("wifi.test");
	
	// Open tcptrace file
	cout << "opening tcptrace log file" << endl;
	file_tcptrace.open("/Users/chenxm/Jamin/workspace/SJTU-TRACE-LOG/2012120915/wifi_12091515-1620.pcap.tcptrace.TCP.out", ifstream::in);
	if ( !file_tcptrace.good()){
		cout << "Bad opening stream.\n";
		exit (-1);
	}
	
	string line;
	int i = 0;
	Tcptrace *ttlog;
	while(!file_tcptrace.eof()){
		getline(file_tcptrace, line);
		i++;
		try{
			ttlog = new Tcptrace(line);
			mu.insert_tcptrace_log(*ttlog);
			delete ttlog;
		}catch(boost::bad_lexical_cast){
			// Bad log value detected
			continue;
		}
	}
	
	// Open tstat file
	cout << "opening tstat log file" << endl;
	file_tstat.open("/Users/chenxm/Jamin/workspace/SJTU-TRACE-LOG/2012120915/wifi_12091515-1620.pcap.tstat.out/log_tcp_complete", ifstream::in);
	if ( !file_tstat.good()){
		cout << "Bad opening stream.\n";
		exit (-1);
	}
	
	i = 0;
	Tstat *tslog;
	while(!file_tstat.eof() && i < 100){
		getline(file_tstat, line);
		i++;
		try{
			tslog = new Tstat(line);
			mu.insert_tstat_log(*tslog);
			delete tslog;
		}catch(boost::bad_lexical_cast){
			continue;
		}
	}
	
	
	file_tcptrace.close();
	file_tstat.close();
	return 0;
}