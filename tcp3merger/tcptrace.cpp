#include <iostream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "tcptrace.h"

using namespace std;
using namespace boost;

Tcptrace::Tcptrace(string &s){
	vector<string> vStr, couple;
	string curstr;
	
	split(vStr, s, is_any_of(","), token_compress_on);
	for (unsigned int i = 0; i < vStr.size(); i++){
		curstr = vStr[i];
		trim(curstr);
		if (i == 0)
			continue;
		if (i == 1) host_client = curstr;
		else if (i == 2) host_server = curstr;
		else if (i == 3) port_client = lexical_cast<unsigned int>(curstr);
		else if (i == 4) port_server = lexical_cast<unsigned int>(curstr);
		else if (i == 5) first_packet_time = lexical_cast<float>(curstr);
		else if (i == 6) last_packet_time = lexical_cast<float>(curstr);
		else if (i == 7) total_packets_client = lexical_cast<unsigned int>(curstr);
		else if (i == 8) total_packets_server = lexical_cast<unsigned int>(curstr);
		else if (i == 9) resets_sent_client = lexical_cast<unsigned int>(curstr);
		else if (i == 10) resets_sent_server = lexical_cast<unsigned int>(curstr);
		else if (i == 11) ack_pkts_sent_client = lexical_cast<unsigned int>(curstr);
		else if (i == 12) ack_pkts_sent_server = lexical_cast<unsigned int>(curstr);
		else if (i == 13) pure_acks_sent_client = lexical_cast<unsigned int>(curstr);
		else if (i == 14) pure_acks_sent_server = lexical_cast<unsigned int>(curstr);
		else if (i == 15) sack_pkts_sent_client = lexical_cast<unsigned int>(curstr);
		else if (i == 16) sack_pkts_sent_server = lexical_cast<unsigned int>(curstr);
		else if (i == 17) dsack_pkts_sent_client = lexical_cast<unsigned int>(curstr);
		else if (i == 18) dsack_pkts_sent_server = lexical_cast<unsigned int>(curstr);
		else if (i == 19) max_sack_blks_client = lexical_cast<unsigned int>(curstr);
		else if (i == 20) max_sack_blks_server = lexical_cast<unsigned int>(curstr);
		else if (i == 21) unique_bytes_sent_client = lexical_cast<unsigned int>(curstr);
		else if (i == 22) unique_bytes_sent_server = lexical_cast<unsigned int>(curstr);
		else if (i == 23) actual_data_pkts_client = lexical_cast<unsigned int>(curstr);
		else if (i == 24) actual_data_pkts_server = lexical_cast<unsigned int>(curstr);
		else if (i == 25) actual_data_bytes_client = lexical_cast<unsigned int>(curstr);
		else if (i == 26) actual_data_bytes_server = lexical_cast<unsigned int>(curstr);
		else if (i == 27) rexmt_data_pkts_client = lexical_cast<unsigned int>(curstr);
		else if (i == 28) rexmt_data_pkts_server = lexical_cast<unsigned int>(curstr);
		else if (i == 29) rexmt_data_bytes_client = lexical_cast<unsigned int>(curstr);
		else if (i == 30) rexmt_data_bytes_server = lexical_cast<unsigned int>(curstr);
		else if (i == 31) zwnd_probe_pkts_client = lexical_cast<unsigned int>(curstr);
		else if (i == 32) zwnd_probe_pkts_server = lexical_cast<unsigned int>(curstr);
		else if (i == 33) zwnd_probe_bytes_client = lexical_cast<unsigned int>(curstr);
		else if (i == 34) zwnd_probe_bytes_server = lexical_cast<unsigned int>(curstr);
		else if (i == 35) outoforder_pkts_client = lexical_cast<unsigned int>(curstr);
		else if (i == 36) outoforder_pkts_server = lexical_cast<unsigned int>(curstr);
		else if (i == 37) pushed_data_pkts_client = lexical_cast<unsigned int>(curstr);
		else if (i == 38) pushed_data_pkts_server = lexical_cast<unsigned int>(curstr);
		else if (i == 39){
			split(couple, curstr, is_any_of("/"), token_compress_on);
			syn_pkts_sent_client = lexical_cast<unsigned int>(couple[0]);
			fin_pkts_sent_client = lexical_cast<unsigned int>(couple[1]);
		}
		else if (i == 40){
			split(couple, curstr, is_any_of("/"), token_compress_on);
			syn_pkts_sent_server = lexical_cast<unsigned int>(couple[0]);
			fin_pkts_sent_server = lexical_cast<unsigned int>(couple[1]);
		}
		else if (i == 41){
			split(couple, curstr, is_any_of("/"), token_compress_on);
			req_1323_ws_client = couple[0].c_str() == "Y" ? true : false;
			req_1323_ts_client = couple[1].c_str() == "Y" ? true : false;
		}
		else if (i == 42){
			split(couple, curstr, is_any_of("/"), token_compress_on);
			req_1323_ws_server = couple[0].c_str() == "Y" ? true : false;
			req_1323_ts_server = couple[1].c_str() == "Y" ? true : false;
		}
		else if (i == 43) adv_wind_scale_client = lexical_cast<unsigned int>(curstr);
		else if (i == 44) adv_wind_scale_server = lexical_cast<unsigned int>(curstr);
		else if (i == 45) req_sack_client = curstr.c_str() == "Y" ? true : false;
		else if (i == 46) req_sack_server = curstr.c_str() == "Y" ? true : false;
		else if (i == 47) sacks_sent_client = lexical_cast<unsigned int>(curstr);
		else if (i == 48) sacks_sent_server = lexical_cast<unsigned int>(curstr);
		else if (i == 49) urgent_data_pkts_client = lexical_cast<unsigned int>(curstr);
		else if (i == 50) urgent_data_pkts_server = lexical_cast<unsigned int>(curstr);
		else if (i == 51) urgent_data_bytes_client = lexical_cast<unsigned int>(curstr);
		else if (i == 52) urgent_data_bytes_server = lexical_cast<unsigned int>(curstr);
		else if (i == 53) mss_requested_client = lexical_cast<unsigned int>(curstr);
		else if (i == 54) mss_requested_server = lexical_cast<unsigned int>(curstr);
		else if (i == 55) max_segm_size_client = lexical_cast<unsigned int>(curstr);
		else if (i == 56) max_segm_size_server = lexical_cast<unsigned int>(curstr);
		else if (i == 57) min_segm_size_client = lexical_cast<unsigned int>(curstr);
		else if (i == 58) min_segm_size_server = lexical_cast<unsigned int>(curstr);
		else if (i == 59) avg_segm_size_client = lexical_cast<unsigned int>(curstr);
		else if (i == 60) avg_segm_size_server = lexical_cast<unsigned int>(curstr);
		else if (i == 61) max_win_adv_client = lexical_cast<unsigned int>(curstr);
		else if (i == 62) max_win_adv_server = lexical_cast<unsigned int>(curstr);
		else if (i == 63) min_win_adv_client = lexical_cast<unsigned int>(curstr);
		else if (i == 64) min_win_adv_server = lexical_cast<unsigned int>(curstr);
		else if (i == 65) zero_win_adv_client = lexical_cast<unsigned int>(curstr);
		else if (i == 66) zero_win_adv_server = lexical_cast<unsigned int>(curstr);
		else if (i == 67) avg_win_adv_client = lexical_cast<unsigned int>(curstr);
		else if (i == 68) avg_win_adv_server = lexical_cast<unsigned int>(curstr);
		else if (i == 69) initial_window_bytes_client = lexical_cast<unsigned int>(curstr);
		else if (i == 70) initial_window_bytes_server = lexical_cast<unsigned int>(curstr);
		else if (i == 71) initial_window_pkts_client = lexical_cast<unsigned int>(curstr);
		else if (i == 72) initial_window_pkts_server = lexical_cast<unsigned int>(curstr);
		else if (i == 73) ttl_stream_length_client = lexical_cast<unsigned int>(curstr);
		else if (i == 74) ttl_stream_length_server = lexical_cast<unsigned int>(curstr);
		else if (i == 75) missed_data_client = lexical_cast<unsigned int>(curstr);
		else if (i == 76) missed_data_server = lexical_cast<unsigned int>(curstr);
		else if (i == 77) truncated_data_client = lexical_cast<unsigned int>(curstr);
		else if (i == 78) truncated_data_server = lexical_cast<unsigned int>(curstr);
		else if (i == 79) truncated_packets_client = lexical_cast<unsigned int>(curstr);
		else if (i == 80) truncated_packets_server = lexical_cast<unsigned int>(curstr);
		else if (i == 81) data_xmit_time_client = lexical_cast<float>(curstr);
		else if (i == 82) data_xmit_time_server = lexical_cast<float>(curstr);
		else if (i == 83) idletime_max_client = lexical_cast<float>(curstr);
		else if (i == 84) idletime_max_server = lexical_cast<float>(curstr);
		else if (i == 85) hardware_dups_client = lexical_cast<unsigned int>(curstr);
		else if (i == 86) hardware_dups_server = lexical_cast<unsigned int>(curstr);
		else if (i == 87) throughput_client = lexical_cast<unsigned int>(curstr);
		else if (i == 88) throughput_server = lexical_cast<unsigned int>(curstr);
		else break;
	}
}