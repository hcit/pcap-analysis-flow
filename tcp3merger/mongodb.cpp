#include <boost/lexical_cast.hpp>
#include <string>
#include "mongodb.h"

using namespace std;
using namespace mongo;

int MongoUtils::connect_db(string addr, unsigned short port){
	string errmsg;
    if ( ! conn.connect(addr + ":" + boost::lexical_cast<string>(port) , errmsg) ) {
        cout << "couldn't connect : " << errmsg << endl;
        return EXIT_FAILURE;
    }
	return EXIT_SUCCESS;
}

void MongoUtils::use_table(string tname){
	table_name = tname;
}

void MongoUtils::clean_table(string tname){
	table_name = tname;
	cout << "Clean up old data from any previous running" << endl;
	BSONObjBuilder query;
	conn.remove( table_name , query.obj() );
}

int MongoUtils::insert_tcptrace_log(Tcptrace &tt){
	BSONObjBuilder obj;
	obj.append("host_client", tt.host_client);
	obj.append("host_server", tt.host_server);
	obj.append("port_client", tt.port_client);
	obj.append("port_server", tt.port_server);
	// obj.append("first_packet_time", tt.first_packet_time);
	// obj.append("last_packet_time", tt.last_packet_time);
	// obj.append("total_packets_client", tt.total_packets_client);
	// obj.append("total_packets_server", tt.total_packets_server);
	// obj.append("resets_sent_client", tt.resets_sent_client);
	// obj.append("resets_sent_server", tt.resets_sent_server);
	// obj.append("ack_pkts_sent_client", tt.ack_pkts_sent_client);
	// obj.append("ack_pkts_sent_server", tt.ack_pkts_sent_server);
	// obj.append("pure_acks_sent_client", tt.pure_acks_sent_client);
	// obj.append("pure_acks_sent_server", tt.pure_acks_sent_server);
	// obj.append("sack_pkts_sent_client", tt.sack_pkts_sent_client);
	// obj.append("sack_pkts_sent_server", tt.sack_pkts_sent_server);
	// obj.append("dsack_pkts_sent_client", tt.dsack_pkts_sent_client);
	// obj.append("dsack_pkts_sent_server", tt.dsack_pkts_sent_server);
	// obj.append("max_sack_blks_client", tt.max_sack_blks_client);
	// obj.append("max_sack_blks_server", tt.max_sack_blks_server);
	// obj.append("unique_bytes_sent_client", tt.unique_bytes_sent_client);
	// obj.append("unique_bytes_sent_server", tt.unique_bytes_sent_server);
	// obj.append("actual_data_pkts_client", tt.actual_data_pkts_client);
	// obj.append("actual_data_pkts_server", tt.actual_data_pkts_server);
	// obj.append("actual_data_bytes_client", tt.actual_data_bytes_client);
	// obj.append("actual_data_bytes_server", tt.actual_data_bytes_server);
	// obj.append("rexmt_data_pkts_client", tt.rexmt_data_pkts_client);
	// obj.append("rexmt_data_pkts_server", tt.rexmt_data_pkts_server);
	// obj.append("rexmt_data_bytes_client", tt.rexmt_data_bytes_client);
	// obj.append("rexmt_data_bytes_server", tt.rexmt_data_bytes_server);
	// obj.append("zwnd_probe_pkts_client", tt.zwnd_probe_pkts_client);
	// obj.append("zwnd_probe_pkts_server", tt.zwnd_probe_pkts_server);
	// obj.append("zwnd_probe_bytes_client", tt.zwnd_probe_bytes_client);
	// obj.append("zwnd_probe_bytes_server", tt.zwnd_probe_bytes_server);
	// obj.append("outoforder_pkts_client", tt.outoforder_pkts_client);
	// obj.append("outoforder_pkts_server", tt.outoforder_pkts_server);
	// obj.append("pushed_data_pkts_client", tt.pushed_data_pkts_client);
	// obj.append("pushed_data_pkts_server", tt.pushed_data_pkts_server);
	// obj.append("syn_pkts_sent_client", tt.syn_pkts_sent_client);
	// obj.append("syn_pkts_sent_server", tt.syn_pkts_sent_server);
	// obj.append("fin_pkts_sent_client", tt.fin_pkts_sent_client);
	// obj.append("fin_pkts_sent_server", tt.fin_pkts_sent_server);
	// obj.append("req_1323_ws_client", tt.req_1323_ws_client);
	// obj.append("req_1323_ws_server", tt.req_1323_ws_server);
	// obj.append("req_1323_ts_client", tt.req_1323_ts_client);
	// obj.append("req_1323_ts_server", tt.req_1323_ts_server);
	// obj.append("adv_wind_scale_client", tt.adv_wind_scale_client);
	// obj.append("adv_wind_scale_server", tt.adv_wind_scale_server);
	// obj.append("req_sack_client", tt.req_sack_client);
	// obj.append("req_sack_server", tt.req_sack_server);
	// obj.append("sacks_sent_client", tt.sacks_sent_client);
	// obj.append("sacks_sent_server", tt.sacks_sent_server);
	// obj.append("urgent_data_pkts_client", tt.urgent_data_pkts_client);
	// obj.append("urgent_data_pkts_server", tt.urgent_data_pkts_server);
	// obj.append("urgent_data_bytes_client", tt.urgent_data_bytes_client);
	// obj.append("urgent_data_bytes_server", tt.urgent_data_bytes_server);
	// obj.append("mss_requested_client", tt.mss_requested_client);
	// obj.append("mss_requested_server", tt.mss_requested_server);
	// obj.append("max_segm_size_client", tt.max_segm_size_client);
	// obj.append("max_segm_size_server", tt.max_segm_size_server);
	// obj.append("min_segm_size_client", tt.min_segm_size_client);
	// obj.append("min_segm_size_server", tt.min_segm_size_server);
	// obj.append("avg_segm_size_client", tt.avg_segm_size_client);
	// obj.append("avg_segm_size_server", tt.avg_segm_size_server);
	// obj.append("max_win_adv_client", tt.max_win_adv_client);
	// obj.append("max_win_adv_server", tt.max_win_adv_server);
	// obj.append("min_win_adv_client", tt.min_win_adv_client);
	// obj.append("min_win_adv_server", tt.min_win_adv_server);
	// obj.append("zero_win_adv_client", tt.zero_win_adv_client);
	// obj.append("zero_win_adv_server", tt.zero_win_adv_server);
	// obj.append("avg_win_adv_client", tt.avg_win_adv_client);
	// obj.append("avg_win_adv_server", tt.avg_win_adv_server);
	// obj.append("initial_window_bytes_client", tt.initial_window_bytes_client);
	// obj.append("initial_window_bytes_server", tt.initial_window_bytes_server);
	// obj.append("initial_window_pkts_client", tt.initial_window_pkts_client);
	// obj.append("initial_window_pkts_server", tt.initial_window_pkts_server);
	// obj.append("ttl_stream_length_client", tt.ttl_stream_length_client);
	// obj.append("ttl_stream_length_server", tt.ttl_stream_length_server);
	// obj.append("missed_data_client", tt.missed_data_client);
	// obj.append("missed_data_server", tt.missed_data_server);
	// obj.append("truncated_data_client", tt.truncated_data_client);
	// obj.append("truncated_data_server", tt.truncated_data_server);
	// obj.append("truncated_packets_client", tt.truncated_packets_client);
	// obj.append("truncated_packets_server", tt.truncated_packets_server);
	// obj.append("data_xmit_time_client", tt.data_xmit_time_client);
	// obj.append("data_xmit_time_server", tt.data_xmit_time_server);
	// obj.append("idletime_max_client", tt.idletime_max_client);
	// obj.append("idletime_max_server", tt.idletime_max_server);
	// obj.append("hardware_dups_client", tt.hardware_dups_client);
	// obj.append("hardware_dups_server", tt.hardware_dups_server);
	// obj.append("throughput_client", tt.throughput_client);
	// obj.append("throughput_server", tt.throughput_server);
	conn.insert(table_name, obj.obj());
	
    string e = conn.getLastError();
    if( !e.empty() ) {
        cout << "insert failed: " << e << endl;
        exit(EXIT_FAILURE);
    }
	
	conn.ensureIndex(table_name, BSON( "host_client" << 1 << "port_client" << 1));
	conn.ensureIndex(table_name, BSON( "host_server" << 1 << "port_server" << 1));
	
	return EXIT_SUCCESS;
}

int MongoUtils::insert_tstat_log(Tstat &ts){
	BSONObjBuilder query, rev_query;
	query.append("host_client", ts.host_client);
	query.append("port_client", ts.port_client);
	query.append("host_server", ts.host_server);
	query.append("port_server", ts.port_server);
	
	rev_query.append("host_client", ts.host_server);
	rev_query.append("port_client", ts.port_server);
	rev_query.append("host_server", ts.host_client);
	rev_query.append("port_server", ts.port_client);
	
	bool found =false, dup = false;
	auto_ptr<DBClientCursor> cursor = conn.query(table_name, query.obj());
	if (cursor->more() == false){
		cursor = conn.query(table_name, rev_query.obj());
	}
	
	cout << cursor->more() << endl;
	// int item_cnt = cursor->itcount();
	// 
	// if (item_cnt == 0 ){}
	// else if (item_cnt == 1) found = true;
	// else {found = true; dup = true;}
	// 
	// if (found == true){
	// 	cout << "item found";
	// 	if (dup == true)
	// 		cout << ", item dup";
	// 	cout << endl;
	// }
	
	// conn.insert(table_name, obj.obj());
	//     string e = conn.getLastError();
	//     if( !e.empty() ) {
	//         cout << "insert failed: " << e << endl;
	//         exit(EXIT_FAILURE);
	//     }
	
	return EXIT_SUCCESS;
	
}