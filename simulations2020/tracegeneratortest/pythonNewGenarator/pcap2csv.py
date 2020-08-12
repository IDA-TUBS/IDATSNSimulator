#!/usr/bin/env python3

"""pcap2csv
Script to extract specific pieces of information from a pcap file and
render into a csv file.

Usage: <program name> --pcap <input pcap file> --csv <output pcap file>

Each packet in the pcap is rendered into one row of the csv file.
The specific items to extract, and the order in which they are rendered
in the csv are hard-coded in the script, in the 'render_csv_row' function.
Also note that the separators in the csv are '|' characters, not commas.

This script uses *both* PyShark (https://kiminewt.github.io/pyshark/) and
Scapy to do its work. PyShark because we want to leverage tshark's powerful
protocol decoding ability to generate the "textual description" field of
the CSV, and Scapy because at the same time we want to access the "payload"
portion of the packet (PyShark seems to be unable to provide this).
"""

import argparse
import os.path
import sys

import pyshark
from scapy.utils import RawPcapReader
from scapy.layers.l2 import Ether, Dot1Q, Dot3
from scapy.layers.inet import IP, UDP, TCP

#--------------------------------------------------

def render_csv_row(pkt_sh, pkt_sc, fh_csv):
	"""Write one packet entry into the CSV file.
	pkt_sh is the PyShark representation of the packet

	pkt_sc is a 'bytes' representation of the packet as returned from
	scapy's RawPcapReader

	fh_csv is the csv file handle
	"""
	ether_pkt_sc = Ether(pkt_sc)
	if ether_pkt_sc.type != 0x800 and ether_pkt_sc.type != 0x8100:
		print('Ignoring non-IP packet')
		return False
	#extrackt source and destination
	l2_dst = ether_pkt_sc.dst
	l2_src = ether_pkt_sc.src  
	
	dot3_pkt_sc = Dot3(pkt_sc)	
	l2_len = dot3_pkt_sc.len	
	
	#check if fame is iee802.1q 	
	if ether_pkt_sc.type == 0x8100: 
		# extrackt vlan
		dot1q_pkt_sc = Dot1Q(pkt_sc)
		l2_prio = dot1q_pkt_sc.prio
		l2_id = dot1q_pkt_sc.id
		l2_vlan = dot1q_pkt_sc.vlan
	else:
		l2_prio = 0
		l2_id = 0
		l2_vlan = 0	


	
	#	 0                   1                   2                   3
	# 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	#| PRIO|I|          VLAN         |              TYPE             |
	#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

       
	# Each line of the CSV has this format
	fmt = '{0}|{1}|{2}|{3}|{4}|{5}|{6}|{7}'
	#       |   |   |   |   |   |   |   |  
	#       |   |   |   |   |   |   |   |   
	#       |   |   |   |   |   |   |   |  
	#       |   |   |   |   |   |   |   |  
	#       |   |   |   |   |   |   |   o--------------> {7}  idfield 
	#       |   |   |   |   |   |   o------------------> {6}  prioirty
	#       |   |   |   |   |   o----------------------> {5}  vlan id
	#       |   |   |   |   o--------------------------> {4]  frame lenght
	#       |   |   |   o------------------------------> {3}  source mac
	#       |   |   o----------------------------------> {2}  destination mac
	#       |   o--------------------------------------> {1}  time
	#       o------------------------------------------> {0}  frame number

	print(fmt.format(pkt_sh.no,			# {0}
		pkt_sh.time,				# {1}
		l2_dst,					# {2}
		l2_src,					# {3}
		l2_len,					# {4}
		l2_vlan,				# {5}
		l2_prio,				# {6}
		l2_id),					# {7}
		file=fh_csv)

	return True
    #--------------------------------------------------

def pcap2csv(in_pcap, out_csv):
    """Main entry function called from main to process the pcap and
    generate the csv file.

    in_pcap = name of the input pcap file (guaranteed to exist)
    out_csv = name of the output csv file (will be created)

    This function walks over each packet in the pcap file, and for
    each packet invokes the render_csv_row() function to write one row
    of the csv.
    """

    # Open the pcap file with PyShark in "summary-only" mode, since this
    # is the mode where the brief textual description of the packet (e.g.
    # "Standard query 0xf3de A www.cisco.com", "Client Hello" etc.) are
    # made available.
    pcap_pyshark = pyshark.FileCapture(in_pcap, only_summaries=True)
    pcap_pyshark.load_packets()
    pcap_pyshark.reset()

    frame_num = 0
    ignored_packets = 0
    with open(out_csv, 'w') as fh_csv:
        # Open the pcap file with scapy's RawPcapReader, and iterate over
        # each packet. In each iteration get the PyShark packet as well,
        # and then call render_csv_row() with both representations to generate
        # the CSV row.
        for (pkt_scapy, _) in RawPcapReader(in_pcap):
            try:
                pkt_pyshark = pcap_pyshark.next_packet()
                frame_num += 1
                if not render_csv_row(pkt_pyshark, pkt_scapy, fh_csv):
                    ignored_packets += 1
            except StopIteration:
                # Shouldn't happen because the RawPcapReader iterator should also
                # exit before this happens.
                break

    print('{} packets read, {} packets not written to CSV'.
          format(frame_num, ignored_packets))
#--------------------------------------------------

def command_line_args():
    """Helper called from main() to parse the command line arguments"""

    parser = argparse.ArgumentParser()
    parser.add_argument('--pcap', metavar='<input pcap file>',
                        help='pcap file to parse', required=True)
    parser.add_argument('--csv', metavar='<output csv file>',
                        help='csv file to create', required=True)
    args = parser.parse_args()
    return args
#--------------------------------------------------

def main():
    """Program main entry"""
    args = command_line_args()

    if not os.path.exists(args.pcap):
        print('Input pcap file "{}" does not exist'.format(args.pcap),
              file=sys.stderr)
        sys.exit(-1)

    if os.path.exists(args.csv):
        print('Output csv file "{}" already exists, '
              'won\'t overwrite'.format(args.csv),
              file=sys.stderr)
        sys.exit(-1)

    pcap2csv(args.pcap, args.csv)
#--------------------------------------------------

if __name__ == '__main__':
    main()
