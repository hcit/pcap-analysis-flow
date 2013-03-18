#ifndef __TSTAT_H__
#define __TSTAT_H__

#include <iostream>
using namespace std;

typedef struct _tstat_log_t TstatLog;
struct _tstat_log_t{
};

class Tstat{
public:
	Tstat(string &s);
	
	// Client side
	string host_client;
	unsigned int port_client;
	unsigned int total_pkts_client;
	bool reset_sent_client;
	unsigned int acks_sent_client;
	unsigned int pure_acks_sent_client;
	unsigned int unique_bytes_client;
	unsigned int data_pkts_client;
	unsigned int data_bytes_client;
	unsigned int rexmit_pkts_client;
	unsigned int rexmit_bytes_client;
	unsigned int outoford_pkts_client;
	unsigned int syns_client;
	unsigned int fins_clinet;
	bool rfc_1323_ws_client;
	bool rfc_1323_ts_client;
	unsigned int win_scale_factor_client;
	bool sack_sent_client;
	unsigned int sacks_sent_client;
	unsigned int mss_declared_client;
	unsigned int max_mss_observed_client;
	unsigned int min_mss_observed_client;
	unsigned int max_rcvwin_client;
	unsigned int min_rcvwin_client;
	unsigned int zerowin_client;
	unsigned int max_cwin_client;
	unsigned int min_cwin_client;
	unsigned int init_cwin_client;
	float avg_rtt_client;
	float min_rtt_client;
	float max_rtt_client;
	float stddev_rtt_client;
	unsigned int valid_rtt_client;
	unsigned int min_ttl_client;
	unsigned int max_ttl_client;
	unsigned int rexmit_pkts_timeout_client;
	unsigned int rexmit_pkts_fast_client;
	unsigned int pkt_rec_client;
	unsigned int net_dups_client;
	unsigned int unknown_pkts_client;
	unsigned int rexmit_pkts_control_client;
	unsigned int unnece_rexmit_timeout_client;
	unsigned int unnece_rexmit_fast_client;
	unsigned int rexmit_diff_init_seq_client;
	// Server side
	string host_server;
	unsigned int port_server;
	unsigned int total_pkts_server;
	bool reset_sent_server;
	unsigned int acks_sent_server;
	unsigned int pure_acks_sent_server;
	unsigned int unique_bytes_server;
	unsigned int data_pkts_server;
	unsigned int data_bytes_server;
	unsigned int rexmit_pkts_server;
	unsigned int rexmit_bytes_server;
	unsigned int outoford_pkts_server;
	unsigned int syns_server;
	unsigned int fins_server;
	bool rfc_1323_ws_server;
	bool rfc_1323_ts_server;
	unsigned int win_scale_factor_server;
	bool sack_sent_server;
	unsigned int sacks_sent_server;
	unsigned int mss_declared_server;
	unsigned int max_mss_observed_server;
	unsigned int min_mss_observed_server;
	unsigned int max_rcvwin_server;
	unsigned int min_rcvwin_server;
	unsigned int zerowin_server;
	unsigned int max_cwin_server;
	unsigned int min_cwin_server;
	unsigned int init_cwin_server;
	float avg_rtt_server;
	float min_rtt_server;
	float max_rtt_server;
	float stddev_rtt_server;
	unsigned int valid_rtt_server;
	unsigned int min_ttl_server;
	unsigned int max_ttl_server;
	unsigned int rexmit_pkts_timeout_server;
	unsigned int rexmit_pkts_fast_server;
	unsigned int pkt_rec_server;
	unsigned int net_dups_server;
	unsigned int unknown_pkts_server;
	unsigned int rexmit_pkts_control_server;
	unsigned int unnece_rexmit_timeout_server;
	unsigned int unnece_rexmit_fast_server;
	unsigned int rexmit_diff_init_seq_server;
	// Flow
	float flow_duration;
	float first_pkt_offset;
	float last_pkt_offset;
	float first_payload_offset_client;
	float first_payload_offset_server;
	float last_payload_offset_client;
	float last_payload_offset_server;
	float first_pure_ack_offset_client;
	float first_pure_ack_offset_server;
	float first_pkt_time;
	bool has_internel_ip_client;
	bool has_internel_ip_server;
	unsigned int flow_type_mask;
	unsigned int p2p_type_mask;
	unsigned int p2p_subtype_mask;
	unsigned int p2p_ed2k_data_msgs;
	unsigned int p2p_ed2k_sig_msgs;
	unsigned int p2p_ed2k_msgs_client;
	unsigned int p2p_ed2k_msgs_server;
	unsigned int p2p_ed2k_chat_msgs;
	unsigned int http_type_mask;
	string ssl_hello_client;
	string ssl_hello_server;
};

#endif