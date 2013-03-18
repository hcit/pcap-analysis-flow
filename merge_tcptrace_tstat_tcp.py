#!/usr/bin/env python
# Merge Tcptrace output and Tstat output into one file
# We merge the tstat log into tcptrace because tcptrace treats flow 
# as closure by 3WHS+FIN/RST and tstat by only 3WHS 
# 
# By chenxm
#
import sys
import re

#############################
## Parsing options
#############################

def print_usage():
    print "Usage: program [--step] <tcptrace_out> <tstat_complete_tcp_out>"

step_match = False
if len(sys.argv) == 3:
    ttname = sys.argv[1]
    tsname = sys.argv[2]
elif len(sys.argv) == 4:
    if sys.argv[1] != '--step':
        print_usage()
        exit(-1)
    step_match = True
    ttname = sys.argv[2]
    tsname = sys.argv[3]
else:
    print_usage()
    exit(-1)


#############################
## Step1: Splitting tcptrace log file into tcp and udp logs
#############################

def process_one_udp_msg(s):
    return s

# Prepare output streams
name, suffix = ttname.rsplit('.', 1)
tt_tcp_file = "%s.tcp.tmp" % name
tt_udp_file = "%s.udp.tmp" % name
tcp_out_stream = open(tt_tcp_file, 'wb')
udp_out_stream = open(tt_udp_file, 'wb')

tcp_msg = True
udp_msg = False
one_udp_msg = None

onetcpstart = re.compile(r'^\d+,')
udpstart = re.compile(r'^UDP connection info')
oneudpstart = re.compile(r'^UDP connection \d+:')

for line in open(ttname, 'r'):
    if len(line.strip()) == 0 or line.strip()[0] in ['#', '=']:
        continue
        
    if udpstart.search(line):
        tcp_msg = False
        udp_msg = True
        
    if tcp_msg == True and udp_msg == False:
        # TCP msg critiria
        if onetcpstart.search(line):
            # Remove flow # in tcptrace log
            tcp_out_stream.write(' '.join([i.strip() for i in line.split(',')[1:]]))
            tcp_out_stream.write('\n')
    
    if tcp_msg == False and udp_msg == True:
        # UDP msg critiria
        if oneudpstart.search(line):
            if one_udp_msg != None:
                udp_out_stream.write(process_one_udp_msg(one_udp_msg))
            one_udp_msg = ''
        elif one_udp_msg != None:
            one_udp_msg += line
              
tcp_out_stream.flush() and tcp_out_stream.close()
udp_out_stream.flush() and udp_out_stream.close()


#############################
## Step2: Merging tcptrace and tstat log
#############################

class TTLog(object):
    def __init__(self, s):
        s = s.strip(" \r\n")
        parts = s.split(' ')
        assert(len(parts) == 88)
        
        a_host = parts[0].strip()
        b_host = parts[1].strip()
        a_port = parts[2].strip()
        b_port = parts[3].strip()
        ori_socket = ["%s:%s" % (a_host, a_port), "%s:%s" % (b_host, b_port)]
        
        socket = sorted(ori_socket)
        rev = False
        if socket != ori_socket:
            rev = True

        self.socket = "%s-%s" % (socket[0], socket[1])
        self.log = s
        self.time = float(parts[4])
        self.reversed = rev
        

class TSLog(object):
    def __init__(self, s):
        s = s.strip(" \r\n")
        parts = s.split(' ')
        assert(len(parts) == 111)
        
        a_host = parts[0].strip()
        a_port = parts[1].strip()
        b_host = parts[44].strip()
        b_port = parts[45].strip()
        ori_socket = ["%s:%s" % (a_host, a_port), "%s:%s" % (b_host, b_port)]
        
        socket = sorted(ori_socket)
        rev = False
        if socket != ori_socket:
            rev = True
            
        self.socket = "%s-%s" % (socket[0], socket[1])
        self.log = s
        self.time = float(parts[97])/1000   # seconds
        self.reversed = rev

class LogJoint(object):
    # Class variables
    HAS_TTLOG_ONLY = 1
    HAS_TSLOG_ONLY = 2
    HAS_BOTH = 3
    HAS_NONE = -1
    
    def __init__(self, ttlog, tslog):
        assert(isinstance(ttlog, TTLog) or isinstance(tslog, TSLog))
        self.ttlog = ttlog    # Tcptrace log
        self.tslog = tslog    # Tstat log
        
    def status(self):
        if self.ttlog != None and self.tslog == None:
            return LogJoint.HAS_TTLOG_ONLY
        elif self.ttlog == None and self.tslog != None:
            return LogJoint.HAS_TSLOG_ONLY
        elif self.ttlog != None and self.tslog != None:
            return LogJoint.HAS_BOTH
        else:
            return LogJoint.HAS_NONE
        
    def add_log(self, log):
        if isinstance(log, TTLog):
            matcher = self.tslog
        elif isinstance(log, TSLog):
            matcher = self.ttlog
        else:
            assert("Invalid log object")
            
        if abs(matcher.time - log.time) < 10:  # seconds
            if isinstance(log, TTLog):
                self.ttlog = log
            elif isinstance(log, TSLog):
                self.tslog = log
            return self
        else:
            return None
    
    def merged_log(self):
        if self.status() == LogJoint.HAS_BOTH:
            tsall = [i.strip() for i in self.tslog.log.split(' ') if i.strip() != '' ]
            ts_left_index = []
            ts_left_index_client = []
            ts_left_index_server = []
            ts_left_index_common = []
            # Refer to flowfeatures.xlsx.merged
            # considering the reversed sockets
            # Client
            ts_left_index_client.extend(range(13, 17)) # 13-16
            ts_left_index_client.extend(range(26, 28)) # 26-27
            ts_left_index_client.extend(range(29, 39)) # 29-38
            ts_left_index_client.extend(range(40, 45)) # 40-44
            ts_left_index_client.extend([92,94,96,99,106,110])
            # Server
            ts_left_index_server.extend(range(57, 61)) # 57-60
            ts_left_index_server.extend(range(70, 72)) # 70-71
            ts_left_index_server.extend(range(73, 83)) # 73-82
            ts_left_index_server.extend(range(84, 89)) # 84-88
            ts_left_index_server.extend([93,95,97,100,107,111])
            # Common
            ts_left_index_common.extend(range(89, 92)) # 89-91
            ts_left_index_common.extend([98,101,102,103,104,105,108,109])

            if self.ttlog.reversed == self.tslog.reversed:
                # The same order to original tstat log
                ts_left_index = ts_left_index_client + ts_left_index_server + ts_left_index_common
            else:
                # Switch the client and server fields
                ts_left_index = ts_left_index_server + ts_left_index_client + ts_left_index_common
            return '%s %s' % (self.ttlog.log.rstrip(), ' '.join([tsall[index-1] for index in ts_left_index]))
        else:
            return None


# Read a iterator by a fixed width of window
# The iterator must support next() method
def read_win(fileobj, winsize):
    lines = []
    eof = False
    while winsize > 0:
        try:
            next = fileobj.next()
            lines.append(next.strip(" \r\n"))
            winsize -= 1
        except StopIteration:
            eof = True
            break
    return eof, lines

def proc_ttlog_win(win_lines):
    global flow_box, unique_tt2ts, multiple_tt2ts, merge_tcp_os
    
    for line in win_lines:
        line = line.strip(" \r\n")
        ttlog = TTLog(line)
        key = ttlog.socket
        if key not in flow_box:
            flow_box[key] = [LogJoint(ttlog, None)]
        else:
            found = False
            for item in flow_box[key]:
                if item.status() == LogJoint.HAS_TSLOG_ONLY:
                    new_item = item.add_log(ttlog)
                    if new_item != None and new_item.status() == LogJoint.HAS_BOTH:
                        if len(flow_box[key]) == 1:
                            unique_tt2ts += 1
                        else:
                            multiple_tt2ts += 1
                        merge_tcp_os.write(new_item.merged_log()+'\n')
                        flow_box[key].remove(new_item)
                        found = True
                        break 
            if found == False:
                flow_box[key].append(LogJoint(ttlog, None))

def proc_tslog_win(win_lines):
    global flow_box, unique_ts2tt, multiple_ts2tt, merge_tcp_os
    
    for line in win_lines:
        line = line.strip(" \r\n")
        tslog = TSLog(line)
        key = tslog.socket
        if key not in flow_box:
            flow_box[key] = [LogJoint(None, tslog)]
        else:
            found = False
            for item in flow_box[key]:
                if item.status() == LogJoint.HAS_TTLOG_ONLY:
                    new_item = item.add_log(tslog)
                    if new_item != None and new_item.status() == LogJoint.HAS_BOTH:
                        if len(flow_box[key]) == 1:
                            unique_ts2tt += 1
                        else:
                            multiple_ts2tt += 1
                        merge_tcp_os.write(new_item.merged_log()+'\n')
                        flow_box[key].remove(new_item)
                        found = True
                        break 
            if found == False:
                flow_box[key].append(LogJoint(None, tslog))

# Alg. One:
# Add the tcptrace and tstat logs into the flow hash table
# with step block. The one is matched to the other and removed from
# the hash table if they belong to the same flow
def merge_win():
    global tt_tcp_is, ts_tcp_is
    
    stop_tt = False
    stop_ts = False
    winsize = 100
    while not (stop_tt or stop_ts):
        stop_tt, lines1 = read_win(tt_tcp_is, winsize)
        proc_ttlog_win(lines1)
        
        stop_ts, lines2 = read_win(ts_tcp_is, winsize)
        proc_tslog_win(lines2)

# Alg. Two:
# Add the tcptrace log into the flow hash table first
# Then add the tstat log and match the latter to the former and removed
# from the hash table if they are the same flow
def merge_ts2tt():
    global unique_ts2tt, multiple_ts2tt, merge_tcp_os
    
    # Process Tcptrace log
    for line in tt_tcp_is:
        line = line.strip(" \r\n")
        ttlog = TTLog(line)
        key = ttlog.socket
        if key not in flow_box:
            flow_box[key] = [LogJoint(ttlog, None)]
        else:
            flow_box[key].extend([LogJoint(ttlog, None)])
            
    # Process Tstat log
    for line in ts_tcp_is:
        line = line.strip(" \r\n")
        tslog = TSLog(line)
        key = tslog.socket
        if key in flow_box:
            for item in flow_box[key]:
                if (item.status() == LogJoint.HAS_TTLOG_ONLY):
                    new_item = item.add_log(tslog)
                    if new_item != None and new_item.status() == LogJoint.HAS_BOTH:
                        if len(flow_box[key]) == 1:
                            unique_ts2tt += 1
                        else:
                            multiple_ts2tt += 1
                        merge_tcp_os.write(new_item.merged_log()+'\n')
                        flow_box[key].remove(new_item)
                        break
    #EOM

# Main
unique_tt2ts = 0
unique_ts2tt = 0
multiple_tt2ts = 0
multiple_ts2tt = 0
flow_box = {
### 'socket' : [LogJoint, LogJoint, ...]
}

# Prepare streams
tt_tcp_is = open(tt_tcp_file, 'rb')
ts_tcp_is = open(tsname, 'rb')
merge_tcp_os = open(name.rsplit('.', 1)[0]+'.tcp.out', 'wb')

# Performing mathcing
if step_match:
    merge_win()
else:
    merge_ts2tt()

# Print results
print 'Unique socket:'
print '\ttt2ts: %d' % unique_tt2ts
print '\tts2tt: %d' % unique_ts2tt
print '\ttotal: %d' % (unique_tt2ts + unique_ts2tt)
print 'Multiple socket:'
print '\ttt2ts: %d' % multiple_tt2ts
print '\tts2tt: %d' % multiple_ts2tt
print '\ttotal: %d' % (multiple_tt2ts + multiple_ts2tt)

# Close opened files
tt_tcp_is.close()
ts_tcp_is.close()
merge_tcp_os.flush() and merge_tcp_os.close()

# EOF