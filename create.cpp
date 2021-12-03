#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <stdio.h>
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

int main(int argc, char const *argv[]) {
  fstream infile (argv[1],ios::in);
  fstream outfile ("library.out", ios::out | ios::binary);

  BookRec buffer;
  set <int> ISBNs;
  int count=1;
  String s;
  while(infile >> s)
  {
    bool trueWrite=true;
    if(s[0]=='-')    //(testIsbn[0]=='-' || testIsbn.compare("0000000000")==0)
    {
      cerr<<"Illegal isbn number encountered on line "<< count<< " of data file - record ignored."<<endl;
      trueWrite=false;
    }
    sscanf(s, "%d", &buffer.isbn);
    //infile.get();
    String Name, Name2;
    infile.getline(Name, 100, '|');
    int i=11;
    int k=0;


    while(s[i]!='\0'){
      buffer.name[k]=s[i];
      i++;
      k++;
    }
    int j=0;
    while( k<25 ){
      buffer.name[k]=Name[j];
      j++;
      k++;
    }

    //cout<<"here with name: "<<buffer.name<<endl;
    infile.getline(buffer.author, 100, '|');
    //cout<<"here with author: "<<buffer.author<<endl;
    infile >> buffer.onhand;

    //cout<<"here with onhand: "<<buffer.onhand<<endl;

    infile.get();
    infile >> buffer.price;
    infile.get();
    infile.getline(buffer.type, 300, '\n');

    set<int>::iterator it=ISBNs.end();
    if(!ISBNs.empty() && (ISBNs.count(buffer.isbn)!=0 || buffer.isbn<*it) )
    {
      cerr<<"Isbn number out of sequence on line "<< count << " of data file."<<endl;
      //cout<<buffer.isbn<<" "<<buffer.name<<" "<<buffer.author<<" "<<buffer.onhand<<" "<<buffer.price<<" "<<buffer.type<<endl;
      //printf("%u | %20s | %25s | %d | %f | %10s \n", buffer.isbn, buffer.name, buffer.author, buffer.onhand, buffer.price, buffer.type);
      cout<<setw(10)<<setfill('0')<<buffer.isbn<<setw(25)<<setfill(' ')<<buffer.name<<setw(25)<<buffer.author
	      <<setw(3)<<buffer.onhand<<setw(6)<<buffer.price<<setw(10)<<buffer.type<<endl;
    }
    else if(trueWrite)
    {
      ISBNs.insert(buffer.isbn);
    }
    if(buffer.onhand < 0 || buffer.price < 0)
    {
      cerr<<"Illegal onhand or price value on line "<<count<<" of the data file - record ignored"<<endl;
      //cout<<buffer.isbn<<" "<<buffer.name<<" "<<buffer.author<<" "<<buffer.onhand<<" "<<buffer.price<<" "<<buffer.type<<endl;
      //printf("%u | %20s | %25s | %d | %f | %10s \n", buffer.isbn, buffer.name, buffer.author, buffer.onhand, buffer.price, buffer.type);
      cout<<setw(10)<<setfill('0')<<buffer.isbn<<setw(25)<<setfill(' ')<<buffer.name<<setw(25)<<buffer.author
	      <<setw(3)<<buffer.onhand<<setw(6)<<buffer.price<<setw(10)<<buffer.type<<endl;
      trueWrite=false;
    }
    if(trueWrite)
    {
      outfile.write((char *) &buffer, sizeof(BookRec));
    }
    count++;
  }
  outfile.close();
  fstream binfile ("library.out", ios::in | ios::binary);
  cout<<endl;
  while(binfile.read ((char *) &buffer, sizeof(buffer)) )
  {
    //cout<<buffer.isbn<<" "<<buffer.name<<" "<<buffer.author<<" "<<buffer.onhand<<" "<<buffer.price<<" "<<buffer.type<<endl;
    //printf("%u | %20s | %25s | %d | %f | %10s \n", buffer.isbn, buffer.name, buffer.author, buffer.onhand, buffer.price, buffer.type);
    cout<<setw(10)<<setfill('0')<<buffer.isbn<<setw(25)<<setfill(' ')<<buffer.name<<setw(25)<<buffer.author
      <<setw(3)<<buffer.onhand<<setw(6)<<fixed<<setprecision(2)<<buffer.price<<setw(10)<<buffer.type<<endl;
  }
  return 0;
}
