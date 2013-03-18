#include <vector>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "tstat.h"
using namespace std;
using namespace boost;

Tstat::Tstat(string &s){
	vector<string> vStr;
	split(vStr, s, is_any_of(" "), token_compress_on);
	string curstr;
	for (int i = 1; i <= vStr.size(); i++){
		curstr = vStr[i-1];
		trim(curstr);
		if (curstr == "") continue;
		switch(i){
		// Client side
		case 1: host_client = curstr; break;
		case 2: port_client = lexical_cast<unsigned int>(curstr); break;
		case 3: total_pkts_client = lexical_cast<unsigned int>(curstr); break;
		case 4: reset_sent_client = (curstr.c_str() == "0" ? false : true); break;
		case 5: acks_sent_client = lexical_cast<unsigned int>(curstr); break;
		case 6: pure_acks_sent_client = lexical_cast<unsigned int>(curstr); break;
		case 7: unique_bytes_client = lexical_cast<unsigned int>(curstr); break;
		case 8: data_pkts_client = lexical_cast<unsigned int>(curstr); break;
		case 9: data_bytes_client = lexical_cast<unsigned int>(curstr); break;
		case 10: rexmit_pkts_client = lexical_cast<unsigned int>(curstr); break;
		case 11: rexmit_bytes_client = lexical_cast<unsigned int>(curstr); break;
		case 12: outoford_pkts_client = lexical_cast<unsigned int>(curstr); break;
		case 13: syns_client = lexical_cast<unsigned int>(curstr); break;
		case 14: fins_clinet = lexical_cast<unsigned int>(curstr); break;
		case 15: rfc_1323_ws_client = (curstr.c_str() == "0" ? false : true); break;
		case 16: rfc_1323_ts_client = (curstr.c_str() == "0" ? false : true); break;
		case 17: win_scale_factor_client = lexical_cast<unsigned int>(curstr); break;
		case 18: sack_sent_client = (curstr.c_str() == "0" ? false : true); break;
		case 19: sacks_sent_client = lexical_cast<unsigned int>(curstr); break;
		case 20: mss_declared_client = lexical_cast<unsigned int>(curstr); break;
		case 21: max_mss_observed_client = lexical_cast<unsigned int>(curstr); break;
		case 22: min_mss_observed_client = lexical_cast<unsigned int>(curstr); break;
		case 23: max_rcvwin_client = lexical_cast<unsigned int>(curstr); break;
		case 24: min_rcvwin_client = lexical_cast<unsigned int>(curstr); break;
		case 25: zerowin_client = lexical_cast<unsigned int>(curstr); break;
		case 26: max_cwin_client = lexical_cast<unsigned int>(curstr); break;
		case 27: min_cwin_client = lexical_cast<unsigned int>(curstr); break;
		case 28: init_cwin_client = lexical_cast<unsigned int>(curstr); break;
		case 29: avg_rtt_client = lexical_cast<float>(curstr); break;
		case 30: min_rtt_client = lexical_cast<float>(curstr); break;
		case 31: max_rtt_client = lexical_cast<float>(curstr); break;
		case 32: stddev_rtt_client = lexical_cast<float>(curstr); break;
		case 33: valid_rtt_client = lexical_cast<unsigned int>(curstr); break;
		case 34: min_ttl_client = lexical_cast<unsigned int>(curstr); break;
		case 35: max_ttl_client = lexical_cast<unsigned int>(curstr); break;
		case 36: rexmit_pkts_timeout_client = lexical_cast<unsigned int>(curstr); break;
		case 37: rexmit_pkts_fast_client = lexical_cast<unsigned int>(curstr); break;
		case 38: pkt_rec_client = lexical_cast<unsigned int>(curstr); break;
		case 39: net_dups_client = lexical_cast<unsigned int>(curstr); break;
		case 40: unknown_pkts_client = lexical_cast<unsigned int>(curstr); break;
		case 41: rexmit_pkts_control_client = lexical_cast<unsigned int>(curstr); break;
		case 42: unnece_rexmit_timeout_client = lexical_cast<unsigned int>(curstr); break;
		case 43: unnece_rexmit_fast_client = lexical_cast<unsigned int>(curstr); break;
		case 44: rexmit_diff_init_seq_client = lexical_cast<unsigned int>(curstr); break;
		// Server side
		case 45: host_server = curstr; break;
		case 46: port_server = lexical_cast<unsigned int>(curstr); break;
		case 47: total_pkts_server = lexical_cast<unsigned int>(curstr); break;
		case 48: reset_sent_server = (curstr.c_str() == "0" ? false : true); break;
		case 49: acks_sent_server = lexical_cast<unsigned int>(curstr); break;
		case 50: pure_acks_sent_server = lexical_cast<unsigned int>(curstr); break;
		case 51: unique_bytes_server = lexical_cast<unsigned int>(curstr); break;
		case 52: data_pkts_server = lexical_cast<unsigned int>(curstr); break;
		case 53: data_bytes_server = lexical_cast<unsigned int>(curstr); break;
		case 54: rexmit_pkts_server = lexical_cast<unsigned int>(curstr); break;
		case 55: rexmit_bytes_server = lexical_cast<unsigned int>(curstr); break;
		case 56: outoford_pkts_server = lexical_cast<unsigned int>(curstr); break;
		case 57: syns_server = lexical_cast<unsigned int>(curstr); break;
		case 58: fins_server = lexical_cast<unsigned int>(curstr); break;
		case 59 : rfc_1323_ws_server = (curstr.c_str() == "0" ? false : true); break;
		case 60: rfc_1323_ts_server = (curstr.c_str() == "0" ? false : true); break;
		case 61: win_scale_factor_server = lexical_cast<unsigned int>(curstr); break;
		case 62: sack_sent_server = (curstr.c_str() == "0" ? false : true); break;
		case 63: sacks_sent_server = lexical_cast<unsigned int>(curstr); break;
		case 64: mss_declared_server = lexical_cast<unsigned int>(curstr); break;
		case 65: max_mss_observed_server = lexical_cast<unsigned int>(curstr); break;
		case 66: min_mss_observed_server = lexical_cast<unsigned int>(curstr); break;
		case 67: max_rcvwin_server = lexical_cast<unsigned int>(curstr); break;
		case 68: min_rcvwin_server = lexical_cast<unsigned int>(curstr); break;
		case 69: zerowin_server = lexical_cast<unsigned int>(curstr); break;
		case 70: max_cwin_server = lexical_cast<unsigned int>(curstr); break;
		case 71: min_cwin_server = lexical_cast<unsigned int>(curstr); break;
		case 72: init_cwin_server = lexical_cast<unsigned int>(curstr); break;
		case 73: avg_rtt_server = lexical_cast<float>(curstr); break;
		case 74: min_rtt_server = lexical_cast<float>(curstr); break;
		case 75: max_rtt_server = lexical_cast<float>(curstr); break;
		case 76: stddev_rtt_server = lexical_cast<float>(curstr); break;
		case 77: valid_rtt_server = lexical_cast<unsigned int>(curstr); break;
		case 78: min_ttl_server = lexical_cast<unsigned int>(curstr); break;
		case 79: max_ttl_server = lexical_cast<unsigned int>(curstr); break;
		case 80: rexmit_pkts_timeout_server = lexical_cast<unsigned int>(curstr); break;
		case 81: rexmit_pkts_fast_server = lexical_cast<unsigned int>(curstr); break;
		case 82: pkt_rec_server = lexical_cast<unsigned int>(curstr); break;
		case 83: net_dups_server = lexical_cast<unsigned int>(curstr); break;
		case 84: unknown_pkts_server = lexical_cast<unsigned int>(curstr); break;
		case 85: rexmit_pkts_control_server = lexical_cast<unsigned int>(curstr); break;
		case 86: unnece_rexmit_timeout_server = lexical_cast<unsigned int>(curstr); break;
		case 87: unnece_rexmit_fast_server = lexical_cast<unsigned int>(curstr); break;
		case 88: rexmit_diff_init_seq_server = lexical_cast<unsigned int>(curstr); break;
		// Flow
		case 89: flow_duration = lexical_cast<float>(curstr); break;
		case 90: first_pkt_offset = lexical_cast<float>(curstr); break;
		case 91: last_pkt_offset = lexical_cast<float>(curstr); break;
		case 92: first_payload_offset_client = lexical_cast<float>(curstr); break;
		case 93: first_payload_offset_server = lexical_cast<float>(curstr); break;
		case 94: last_payload_offset_client = lexical_cast<float>(curstr); break;
		case 95: last_payload_offset_server = lexical_cast<float>(curstr); break;
		case 96: first_pure_ack_offset_client = lexical_cast<float>(curstr); break;
		case 97: first_pure_ack_offset_server = lexical_cast<float>(curstr); break;
		case 98: first_pkt_time = lexical_cast<float>(curstr); break;
		case 99: has_internel_ip_client = (curstr.c_str() == "0" ? false : true); break;
		case 100: has_internel_ip_server = (curstr.c_str() == "0" ? false : true); break;
		case 101: flow_type_mask = lexical_cast<unsigned int>(curstr); break;
		case 102: p2p_type_mask = lexical_cast<unsigned int>(curstr); break;
		case 103: p2p_subtype_mask = lexical_cast<unsigned int>(curstr); break;
		case 104: p2p_ed2k_data_msgs = lexical_cast<unsigned int>(curstr); break;
		case 105: p2p_ed2k_sig_msgs = lexical_cast<unsigned int>(curstr); break;
		case 106: p2p_ed2k_msgs_client = lexical_cast<unsigned int>(curstr); break;
		case 107: p2p_ed2k_msgs_server = lexical_cast<unsigned int>(curstr); break;
		case 108: p2p_ed2k_chat_msgs = lexical_cast<unsigned int>(curstr); break;
		case 109: http_type_mask = lexical_cast<unsigned int>(curstr); break;
		case 110: ssl_hello_client = curstr; break;
		case 111: ssl_hello_server = curstr; break;
		default: break;
		};
	}
}