# pcap2csv
Use PyShark and scapy to read fields from a pcap file and populate a CSV

    Usage: pcap2csv --pcap <input pcap file> --csv <output pcap file>

Each packet in the pcap is rendered into one row of the csv file.
The specific items to extract, and the order in which they are rendered
in the csv are hard-coded in the script, in the 'render_csv_row' function.
Also note that the separators in the csv are '|' characters, not commas.

This script uses *both* PyShark (https://kiminewt.github.io/pyshark/) and
Scapy to do its work. PyShark because we want to leverage tshark's powerful
protocol decoding ability to generate the "textual description" field of
the CSV (like "Standard query 0xf3de A www.cisco.com", "Client Hello" etc.), 
and Scapy because at the same time we want to access the "payload"
portion of the packet (PyShark seems to be unable to provide this).

https://github.com/vnetman/pcap2csv

!Important!
Make sure to reorder your trace if you are using Wireshark
reordercap -n testfile.pcap testfile_sorted.pcap 
6793 frames, 62 out of order

CSV file content

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





