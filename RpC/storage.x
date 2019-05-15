
program STORAGE {
	version STORAGEVER {
		int INIT()				 		=1;
		int PUT(string topic<128>, string text<1024>)		=2;
		string GET(string topic<128>, string text<1024>) 	=3;
	} = 1;
} = 99;
