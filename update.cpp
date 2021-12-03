#include <fstream>
#include <string>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <iomanip>
using namespace std;

typedef char String[25];
struct BookRec
{
  unsigned int isbn;
  String name;
  String author;
  int onhand;
  float price;
  String type;
};
enum TransactionType {Add, Delete, ChangeOnhand, ChangePrice};
struct TransactionRec
{
  TransactionType ToDo;
  BookRec B;
};

void runTransaction(BookRec &buffer, TransactionRec &action, BookRec &buffer2, int &transactionNum, fstream &newMast);

int main(int argc, char const *argv[]) {
  if(argc!=4)
  {
    cerr<<"command line error"<<endl;
    return 1;
  }
  string master=argv[1]; string transaction=argv[2]; string newMaster=argv[3];

  fstream inMast(master.c_str(), ios::in | ios::binary);
  fstream inTrans(transaction.c_str(), ios::in | ios::binary);
  fstream newMast(newMaster.c_str(), ios::out | ios::binary);

  fstream err("ERRORS", ios::out);  //send error messages to ERRORS
  cerr.rdbuf(err.rdbuf());

  TransactionRec action;
  int transactionNum=1;
  BookRec buffer, buffer2;
  buffer.isbn=0;
  buffer2.isbn=0;

  while(inTrans.read((char*) &action, sizeof(TransactionRec))) //loop through transactions
  {
    while(buffer2.isbn!=0 && action.B.isbn==buffer2.isbn) //multiple actions on one record
    {
      BookRec buffer3;
      runTransaction(buffer2, action, buffer3, transactionNum, newMast); //complete action store updated data in a temp variable
      inTrans.read((char*) &action, sizeof(TransactionRec));
      buffer2=buffer3;
    }
    if(buffer2.isbn!=0 && action.B.isbn!=buffer2.isbn) //no more actions
    {
      newMast.write((char *) &buffer2, sizeof(BookRec));
      if(buffer2.isbn==buffer.isbn)
      {
        buffer.isbn=0;
      }
      buffer2.isbn=0;
    }
    if(action.B.isbn>buffer.isbn) //process files smaller than transaction isbn
    {
      if(buffer.isbn!=0)
      {
        newMast.write((char *) &buffer, sizeof(BookRec));
      }
      while(inMast.read((char*) &buffer, sizeof(buffer)) && action.B.isbn>buffer.isbn)
      {
        newMast.write((char *) &buffer, sizeof(BookRec));
      }//now action isbn = or >
    }

    runTransaction(buffer, action, buffer2, transactionNum, newMast);
  }
  if(buffer2.isbn!=0) //get leftover in buffer 2 if needed
  {
    newMast.write((char *) &buffer2, sizeof(BookRec));
  }
  while(inMast.read((char*) &buffer, sizeof(buffer))) //ensure all files collected from master
  {
    newMast.write((char *) &buffer2, sizeof(BookRec));
  }
  newMast.close();
  fstream binfile (newMaster.c_str(), ios::in | ios::binary);
  while(binfile.read ((char *) &buffer, sizeof(buffer)) ) //write to screen
  {
    cout<<setw(10)<<setfill('0')<<buffer.isbn<<setw(25)<<setfill(' ')<<buffer.name<<setw(25)<<buffer.author
      <<setw(3)<<buffer.onhand<<setw(6)<<fixed<<setprecision(2)<<buffer.price<<setw(10)<<buffer.type<<endl;
  }
  return 0;
}
void runTransaction(BookRec &buffer, TransactionRec &action, BookRec &buffer2, int &transactionNum, fstream &newMast)
{
  switch (action.ToDo) {
    case Add:
    {
      if(action.B.isbn==buffer.isbn)
      {
        cerr<<"Error in transaction number "<<transactionNum<<": cannot add-duplicate key "<<action.B.isbn<<endl;
        buffer2=buffer;
      }
      else //buffer>action
      {
        buffer2=action.B; //you want to keep buffer the same
      }
          break;
    }
    case Delete:
    {
      if(action.B.isbn==buffer.isbn)
      {
        //dont add to file
        buffer2.isbn=0;
        buffer.isbn=0;
      }
      else
      {
        cerr<<"Error in transaction number "<<transactionNum<<": cannot delete-no such key "<<action.B.isbn<<endl;
      }
          break;
    }
    case ChangeOnhand:
    {
      if(action.B.isbn==buffer.isbn)
      {
        if(buffer.onhand+action.B.onhand>=0)
        {
          buffer.onhand=buffer.onhand+action.B.onhand;
        }
        else
        {
          cerr<<"Error in transaction number "<<transactionNum<<": count = "<<buffer.onhand+action.B.onhand<<" for key "<<action.B.isbn<<endl;
          buffer.onhand=0; //change onhand to 0 and store
        }
        buffer2=buffer;
      }
      else if(action.B.isbn!=buffer.isbn)
      {
        cerr<<"Error in transaction number "<<transactionNum<<": cannot change count-no such key "<<action.B.isbn<<endl;
      }
          break;
    }
    case ChangePrice:
    {
      if(action.B.isbn==buffer.isbn)
      {
          buffer.price=action.B.price;
      }
      else if(action.B.isbn!=buffer.isbn)
      {
        cerr<<"Error in transaction number "<<transactionNum<<": cannot change price-no such key "<<action.B.isbn<<endl;
      }
      buffer2=buffer;
          break;
    }
  }
  transactionNum++;
}
