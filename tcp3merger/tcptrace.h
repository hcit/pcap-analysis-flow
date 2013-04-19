#ifndef __TCPTRACE_H__
#define __TCPTRACE_H__

#include <string>
using namespace std;

class Tcptrace{
public:
	Tcptrace(string &s);
	
	string host_client;
	string host_server;
	unsigned int port_client;
	unsigned int port_server;
	float first_packet_time;
	float last_packet_time;
	unsigned int total_packets_client;
	unsigned int total_packets_server;
	unsigned int resets_sent_client;
	unsigned int resets_sent_server;
	unsigned int ack_pkts_sent_client;
	unsigned int ack_pkts_sent_server;
	unsigned int pure_acks_sent_client;
	unsigned int pure_acks_sent_server;
	unsigned int sack_pkts_sent_client;
	unsigned int sack_pkts_sent_server;
	unsigned int dsack_pkts_sent_client;
	unsigned int dsack_pkts_sent_server;
	unsigned int max_sack_blks_client;
	unsigned int max_sack_blks_server;
	unsigned int unique_bytes_sent_client;
	unsigned int unique_bytes_sent_server;
	unsigned int actual_data_pkts_client;
	unsigned int actual_data_pkts_server;
	unsigned int actual_data_bytes_client;
	unsigned int actual_data_bytes_server;
	unsigned int rexmt_data_pkts_client;
	unsigned int rexmt_data_pkts_server;
	unsigned int rexmt_data_bytes_client;
	unsigned int rexmt_data_bytes_server;
	unsigned int zwnd_probe_pkts_client;
	unsigned int zwnd_probe_pkts_server;
	unsigned int zwnd_probe_bytes_client;
	unsigned int zwnd_probe_bytes_server;
	unsigned int outoforder_pkts_client;
	unsigned int outoforder_pkts_server;
	unsigned int pushed_data_pkts_client;
	unsigned int pushed_data_pkts_server;
	unsigned int syn_pkts_sent_client;
	unsigned int syn_pkts_sent_server;
	unsigned int fin_pkts_sent_client;
	unsigned int fin_pkts_sent_server;
	bool req_1323_ws_client;
	bool req_1323_ws_server;
	bool req_1323_ts_client;
	bool req_1323_ts_server;
	unsigned int adv_wind_scale_client;
	unsigned int adv_wind_scale_server;
	bool req_sack_client;
	bool req_sack_server;
	unsigned int sacks_sent_client;
	unsigned int sacks_sent_server;
	unsigned int urgent_data_pkts_client;
	unsigned int urgent_data_pkts_server;
	unsigned int urgent_data_bytes_client;
	unsigned int urgent_data_bytes_server;
	unsigned int mss_requested_client;
	unsigned int mss_requested_server;
	unsigned int max_segm_size_client;
	unsigned int max_segm_size_server;
	unsigned int min_segm_size_client;
	unsigned int min_segm_size_server;
	unsigned int avg_segm_size_client;
	unsigned int avg_segm_size_server;
	unsigned int max_win_adv_client;
	unsigned int max_win_adv_server;
	unsigned int min_win_adv_client;
	unsigned int min_win_adv_server;
	unsigned int zero_win_adv_client;
	unsigned int zero_win_adv_server;
	unsigned int avg_win_adv_client;
	unsigned int avg_win_adv_server;
	unsigned int initial_window_bytes_client;
	unsigned int initial_window_bytes_server;
	unsigned int initial_window_pkts_client;
	unsigned int initial_window_pkts_server;
	unsigned int ttl_stream_length_client;
	unsigned int ttl_stream_length_server;
	unsigned int missed_data_client;
	unsigned int missed_data_server;
	unsigned int truncated_data_client;
	unsigned int truncated_data_server;
	unsigned int truncated_packets_client;
	unsigned int truncated_packets_server;
	float data_xmit_time_client;
	float data_xmit_time_server;
	float idletime_max_client;
	float idletime_max_server;
	unsigned int hardware_dups_client;
	unsigned int hardware_dups_server;
	unsigned int throughput_client;
	unsigned int throughput_server;
};

#endif