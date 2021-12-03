README Project 1
Samantha Scholz

Purpose:
	-The purpose of this project is to read in a binary master file and a binary transaction file, and complete the valid transactions. 
	The valid transactions are then incorporated into the original data in the new master file. The file will is sorted by ascending isbn values.
Input:
	-update.cpp takes in 3 input file names
		-the old master file- an input binary file of Book Records
		-the transaction file- an input binary file of an instructions and their associated book records
		-the new master file-used to create a binary output file
Output:
	-if a transaction is valid, the change is made to the record and it is recorded in the new master binary file
	-all unaltered records from the original master file are also added to the new master list
	-all files are sorted by isbn number
	-if an error is detected the message is sent to an error text file
Outline:
	-first the file stream objects are created to read binary files and write to a new binary file and a new text file
	-the main portion runs until end of file of the transaction file:
		-check for multiple actions needed flag i.e., buffer2
			-while buffer2 is flagged keep updating record with new transactions
		-if record was not deleted write record to new file and unflag buffer2
	-when the data is not flagged
		-write files to output file that have smaller isbns than the latest transaction
		-find the buffer that is greater than or equal to the transaction	
	-complete transaction for new file and store the value in buffer2
		if errors detected send to ERRORS file
	-return to loop

	once loop is exited:
	determine if data remains in the buffer2 flag and write it to the output
	determine if data remains in the old master file and write it to the output
	
	display the data on the screen

	runTransaction Method:
		-Add:
			-compares the isbns of the transaction and master file
			-if they are equal, only the original is stored
			-otherwise, the transaction value is stored
		-Delete:
			-compares the isbns of the transaction and master file
			-if the isbns equal the buffer2 flag is cleared
			-otherwise, an error is flagged
		-ChangeOnhand:
			-compares the onhand values of the transaction and master file
			-if the isbn is valid it stores the record with the sum of the onhand values
			-otherwise, the onhand value is stored at 0 and an error is sent
		-ChangePrice:
			-if the isbn is valid the price is changed and stored
			-otherwise an error is sent