#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>

using namespace std;

/* ---------- Turbo C++ replacements ---------- */
void clrscr() { cout << "\033[2J\033[1;1H"; }
void gotoxy(int x,int y){ cout << "\033["<<y<<";"<<x<<"H"; }
void pause(){ cin.ignore(); cin.get(); }
int strcmpi(const char* a,const char* b){ return _stricmp(a,b); }

/* ---------- BOOK CLASS ---------- */
class book {
    char bno[6];
    char bname[50];
    char aname[20];
public:
    void create() {
        cout<<"Enter Book No: "; cin>>bno;
        cin.ignore();
        cout<<"Enter Book Name: "; cin.getline(bname,50);
        cout<<"Enter Author Name: "; cin.getline(aname,20);
    }
    void show() {
        cout<<"\nBook No: "<<bno
            <<"\nBook Name: "<<bname
            <<"\nAuthor: "<<aname<<endl;
    }
    void modify() {
        cin.ignore();
        cout<<"New Book Name: "; cin.getline(bname,50);
        cout<<"New Author Name: "; cin.getline(aname,20);
    }
    char* retbno(){ return bno; }
    void report() {
        cout<<bno<<setw(30)<<bname<<setw(25)<<aname<<endl;
    }
};

/* ---------- STUDENT CLASS ---------- */
class student {
    char admno[6];
    char name[20];
    char stbno[6];
    int token;
public:
    void create() {
        cout<<"Enter Admission No: "; cin>>admno;
        cin.ignore();
        cout<<"Enter Student Name: "; cin.getline(name,20);
        token=0; stbno[0]='\0';
    }
    void show() {
        cout<<"\nAdmission No: "<<admno
            <<"\nName: "<<name
            <<"\nBooks Issued: "<<token;
        if(token==1) cout<<"\nBook No: "<<stbno;
        cout<<endl;
    }
    void modify() {
        cin.ignore();
        cout<<"New Name: "; cin.getline(name,20);
    }
    char* retadmno(){ return admno; }
    char* retstbno(){ return stbno; }
    int rettoken(){ return token; }
    void issue(char b[]){ strcpy(stbno,b); token=1; }
    void deposit(){ token=0; stbno[0]='\0'; }
    void report(){
        cout<<admno<<setw(25)<<name<<setw(10)<<token<<endl;
    }
};

/* ---------- GLOBAL OBJECTS ---------- */
fstream fp,fp1;
book bk;
student st;

/* ---------- BOOK FUNCTIONS ---------- */
void writebook() {
    clrscr();
    fp.open("book.dat",ios::out|ios::app);
    bk.create();
    fp.write((char*)&bk,sizeof(book));
    fp.close();
    pause();
}

void displayallb() {
    clrscr();
    fp.open("book.dat",ios::in);
    cout<<"\nBOOK LIST\n";
    cout<<"------------------------------------------------------------\n";
    while(fp.read((char*)&bk,sizeof(book))) bk.report();
    fp.close();
    pause();
}

void displayspb() {
    char n[6]; cout<<"Enter Book No: "; cin>>n;
    fp.open("book.dat",ios::in);
    while(fp.read((char*)&bk,sizeof(book)))
        if(strcmpi(bk.retbno(),n)==0) bk.show();
    fp.close(); pause();
}

void modifybook() {
    char n[6];
    fp.open("book.dat",ios::in|ios::out);
    cout<<"Enter Book No: "; cin>>n;
    while(fp.read((char*)&bk,sizeof(book))) {
        if(strcmpi(bk.retbno(),n)==0) {
            bk.modify();
            fp.seekp(-sizeof(book),ios::cur);
            fp.write((char*)&bk,sizeof(book));
            break;
        }
    }
    fp.close(); pause();
}

void deletebook() {
    char n[6];
    cout<<"Enter Book No: "; cin>>n;
    fp.open("book.dat",ios::in);
    fp1.open("temp.dat",ios::out);
    while(fp.read((char*)&bk,sizeof(book)))
        if(strcmpi(bk.retbno(),n)!=0)
            fp1.write((char*)&bk,sizeof(book));
    fp.close(); fp1.close();
    remove("book.dat"); rename("temp.dat","book.dat");
    pause();
}

/* ---------- STUDENT FUNCTIONS ---------- */
void writestudent() {
    clrscr();
    fp.open("student.dat",ios::out|ios::app);
    st.create();
    fp.write((char*)&st,sizeof(student));
    fp.close();
    pause();
}

void displayalls() {
    clrscr();
    fp.open("student.dat",ios::in);
    cout<<"\nSTUDENT LIST\n";
    cout<<"------------------------------------------------------------\n";
    while(fp.read((char*)&st,sizeof(student))) st.report();
    fp.close();
    pause();
}

void displaysps() {
    char n[6]; cout<<"Enter Admission No: "; cin>>n;
    fp.open("student.dat",ios::in);
    while(fp.read((char*)&st,sizeof(student)))
        if(strcmpi(st.retadmno(),n)==0) st.show();
    fp.close(); pause();
}

void modifystudent() {
    char n[6];
    cout<<"Enter Admission No: "; cin>>n;
    fp.open("student.dat",ios::in|ios::out);
    while(fp.read((char*)&st,sizeof(student))) {
        if(strcmpi(st.retadmno(),n)==0) {
            st.modify();
            fp.seekp(-sizeof(student),ios::cur);
            fp.write((char*)&st,sizeof(student));
            break;
        }
    }
    fp.close(); pause();
}

void deletestudent() {
    char n[6];
    cout<<"Enter Admission No: "; cin>>n;
    fp.open("student.dat",ios::in);
    fp1.open("temp.dat",ios::out);
    while(fp.read((char*)&st,sizeof(student)))
        if(strcmpi(st.retadmno(),n)!=0)
            fp1.write((char*)&st,sizeof(student));
    fp.close(); fp1.close();
    remove("student.dat"); rename("temp.dat","student.dat");
    pause();
}

/* ---------- ISSUE / DEPOSIT ---------- */
void bookissue() {
    char sn[6], bn[6];
    fp.open("student.dat",ios::in|ios::out);
    fp1.open("book.dat",ios::in);
    cout<<"Enter Admission No: "; cin>>sn;
    while(fp.read((char*)&st,sizeof(student))) {
        if(strcmpi(st.retadmno(),sn)==0 && st.rettoken()==0) {
            cout<<"Enter Book No: "; cin>>bn;
            while(fp1.read((char*)&bk,sizeof(book))) {
                if(strcmpi(bk.retbno(),bn)==0) {
                    st.issue(bn);
                    fp.seekp(-sizeof(student),ios::cur);
                    fp.write((char*)&st,sizeof(student));
                    cout<<"Book Issued Successfully\n";
                    fp.close(); fp1.close(); pause(); return;
                }
            }
        }
    }
    cout<<"Issue Failed\n";
    fp.close(); fp1.close(); pause();
}

void bookdeposit() {
    char sn[6];
    int days;
    cout<<"Enter Admission No: "; cin>>sn;
    fp.open("student.dat",ios::in|ios::out);
    while(fp.read((char*)&st,sizeof(student))) {
        if(strcmpi(st.retadmno(),sn)==0 && st.rettoken()==1) {
            cout<<"Enter Days Late: "; cin>>days;
            if(days>15) cout<<"Fine: Rs "<<(days-15)<<endl;
            st.deposit();
            fp.seekp(-sizeof(student),ios::cur);
            fp.write((char*)&st,sizeof(student));
            break;
        }
    }
    fp.close(); pause();
}

/* ---------- ADMIN MENU (11 OPTIONS) ---------- */
void adminmenu() {
    int ch;
    do {
        clrscr();
        cout<<"\nADMINISTRATOR MENU\n";
        cout<<"1.CREATE STUDENT RECORD\n";
        cout<<"2.DISPLAY ALL STUDENT RECORD\n";
        cout<<"3.DISPLAY SPECIFIC STUDENT RECORD\n";
        cout<<"4.MODIFY STUDENT RECORD\n";
        cout<<"5.DELETE STUDENT RECORD\n";
        cout<<"6.CREATE BOOK\n";
        cout<<"7.DISPLAY ALL BOOKS\n";
        cout<<"8.DISPLAY SPECIFIC BOOK\n";
        cout<<"9.MODIFY BOOK RECORD\n";
        cout<<"10.DELETE BOOK RECORD\n";
        cout<<"11.BACK TO MAIN MENU\n";
        cout<<"Enter Choice(1-11): ";
        cin>>ch;

        switch(ch){
            case 1: writestudent(); break;
            case 2: displayalls(); break;
            case 3: displaysps(); break;
            case 4: modifystudent(); break;
            case 5: deletestudent(); break;
            case 6: writebook(); break;
            case 7: displayallb(); break;
            case 8: displayspb(); break;
            case 9: modifybook(); break;
            case 10: deletebook(); break;
        }
    } while(ch!=11);
}

/* ---------- MAIN ---------- */
int main() {
    int ch;
    do {
        clrscr();
        cout<<"\nMAIN MENU\n";
        cout<<"1 BOOK ISSUE\n";
        cout<<"2 BOOK DEPOSIT\n";
        cout<<"3 ADMINISTRATOR MENU\n";
        cout<<"4 EXIT\n";
        cout<<"Select Option(1-4): ";
        cin>>ch;

        switch(ch){
            case 1: bookissue(); break;
            case 2: bookdeposit(); break;
            case 3: adminmenu(); break;
            case 4: exit(0);
        }
    } while(true);

    return 0;
}
