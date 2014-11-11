
    .data
    
	FF			proto	:dword,:dword,:dword,:dword,:dword,:byte,:dword
	GG			proto	:dword,:dword,:dword,:dword,:dword,:byte,:dword
	HH			proto	:dword,:dword,:dword,:dword,:dword,:byte,:dword
	II			proto	:dword,:dword,:dword,:dword,:dword,:byte,:dword
	procMD5hash		proto	:dword,:dword,:dword
	szMD5Format		db	'%.8x%.8x%.8x%.8x',0  
	
	stMD5Result		MD5RESULT <0>

	stMD5First      MD5RESULT <9EBFCBBDh,0D2052D6Eh,3EA7669Ah,890ADEB1h>