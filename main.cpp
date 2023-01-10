#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>
#include <map>

using namespace std;


int main(int argc, char* argv[]){
    if(argc > 3){
        cout << "Incorrect number of arguments given. Correct format is \"mboxsplit.exe bigfile.mbox destination\"" << endl;
        return -1;
    }

    ifstream mailFile(argv[1]);

    if(!mailFile.is_open()){
        cout << "File " << argv[1] << " not found. Please check spelling or location and try again." << endl;
        return -1;
    }

    string destination(argv[2]);
    _mkdir(argv[2]);
    _mkdir((destination+"/inbox").c_str());
    _mkdir((destination+"/sent").c_str());
    _mkdir((destination+"/other").c_str());
    string line;
    string filePath = destination;
    string label = "other";
    string name = "";

    string month;
    string day;
    string time;
    string year;
    stringstream s("");
    map<string, string> months {{"Jan", "01"},{"Feb","02"},{"Mar","03"},{"Apr","04"},{"May","05"},{"Jun","06"},
        {"Jul","07"},{"Aug","08"},{"Sep","09"},{"Oct","10"},{"Nov","11"},{"Dec","12"}};

    string newfilename;
    ofstream smallFile; 

    int i = 1;
    while(!mailFile.eof()){
        getline(mailFile, line);
        if(line.rfind("From ", 0) == 0){
            //start of new email
            s << line;
            s >> month;
            s >> month;
            s >> month;
            s >> month;
            s >> day;
            s >> time;
            s >> year;
            s >> year;
            s.clear();
            
            cout << "Email "<<i<< "/???" << endl;
            newfilename = year+"."+months.at(month)+"."+day+"."+time.substr(0,2)+"."+time.substr(3,2)+"."+time.substr(6,2);
            while(line.rfind("X-Gmail-Labels:", 0) != 0){
                getline(mailFile,line);
            }
            if(line.rfind("X-Gmail-Labels: Inbox", 0) == 0){
                label = "inbox";
                cout << "inbox" << endl;
                while(line.rfind("From: ", 0) !=0){
                    getline(mailFile,line);
                }
                name = line.substr(6, line.rfind("<")-7);

            }else if(line.rfind("X-Gmail-Labels: Sent", 0) == 0){
                cout << "sent" << endl;
                label = "sent";
                while(line.rfind("To:", 0) !=0){
                    getline(mailFile,line);
                }
                name = line.substr(4, line.rfind("<")-5);
            }else{
                label = "other";
                while((line.rfind("From:",0) !=0)){
                    getline(mailFile,line);
                }
                name = line.substr(6, line.rfind("<")-7);
            }
            filePath = "./"+destination+"/" + label +"/"+newfilename + "."+name;
            smallFile.close();
            if(_mkdir(filePath.c_str()) == 0){
                smallFile.open(filePath+"/message.txt");
            }else{
                filePath = "./"+destination+"/" + label +"/"+newfilename;
                _mkdir(filePath.c_str());
                smallFile.open(filePath+"/message.txt");
            }
            
            cout << filePath << endl;
            smallFile<<line<<endl;
            i++;
        }else if(smallFile.is_open()){
            if(line.rfind("Content-Type: text/html", 0) == 0){
                while(line.rfind("--", 0) !=0){
                    getline(mailFile,line);
                }

            } else if(line.rfind("Content-Type: image/jpeg", 0) == 0){
                int firstindex = line.find("\"")+1;
                string imagename = line.substr(firstindex, line.rfind("\"")-firstindex);
                ofstream imageFile(filePath+"/"+imagename);
                while(line.rfind("/9j/", 0) !=0){
                    getline(mailFile,line);
                }
                while(line.rfind("--", 0) !=0){
                    imageFile << line;
                    getline(mailFile,line);
                }
                imageFile.close();
            }else if(line.rfind("--", 0) ==0){

            }else{
            smallFile<<line<<endl;
            }
        }else{
            cout << "ERORR: " << argv[2] << endl;
            return -1;
        }
    }

    smallFile.close();

    return 0;
}