//#include <QtCore/QCoreApplication>
#include<iostream>
#include<fstream>
#include<signal.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<string>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>

using namespace std;
struct cherg
{
 int pi;
 int type;
 int num;//number in order in the list
 char fileName[20];
 int countElem;
 cherg *next,*pred;
};
int stat;
void addqueu(cherg **head, int pid,int type,int num,char 
*filename, int countElem)
{
 cherg* cur=(*head);
 cherg* tmp=(cherg*)malloc(sizeof(cherg));
 if ((*head) != NULL)
 {
 while (cur->next != NULL)
 cur = cur->next;
 tmp->pi=pid;
 tmp->type=type;
 tmp->num=num;
 tmp->countElem=countElem;
 strcpy(tmp->fileName,filename);
 cur->next = tmp;
 tmp->pred=cur;
 tmp->next = NULL;
 //return head;
 }
else
 {
 (*head) = tmp;
 (*head)->next = NULL;
 (*head)->pred-NULL;
 (*head)->pi=pid;
 (*head)->type=type;
 (*head)->num=num;
 (*head)->countElem=countElem;
 strcpy((*head)->fileName,filename);
 }
}
void addProcess(cherg **head,int num,int type, char *file,int 
countElem)
{
 char filename[20]="",buf[20]="",buf1[10]="";
 //int countElem=1;
 if (type==1)
{
 countElem=rand()%100;
 sprintf(buf1,"%d",countElem);
 strcat(filename,"file_#");// file_#
 sprintf(buf,"%d",num);//num in buf
 strcat(filename,buf);// file_#num
 strcat(filename,".txt"); //file_#num.txt
 pid_t pid = fork();
 if (!pid) {//child
 //cout<<getpid()<<endl;
 kill(getpid(),SIGSTOP);
 execl("a1","a1",buf1,filename,NULL);
 exit(0);//вихід з дочірнього процесу
 }
if (pid){
 sleep(1);
 
addqueu(head,pid,type,num,filename,countElem);
 }
 }
if (type==2)
 {
 pid_t pid = fork();
 if (!pid) {//child
 //cout<<" "<<getpid()<<endl;
 kill(getpid(),SIGSTOP);//create sleeping process
 execl("a2","a2",file,NULL);
 exit(0);
 }
 if (pid){//parent
 sleep(1);
addqueu(head,pid,type,num,file,countElem);
 }
 }
 if (type==3)
 {
 pid_t pid = fork();
 if (!pid) {//child
 // cout<<" "<<getpid()<<endl;
 kill(getpid(),SIGSTOP);//create sleeping process
 execl("a3","a3",file,NULL);
 exit(0);
 }
 if (pid){//parent
 sleep(1);
 addqueu(head,pid,type,num,file,countElem);
 }
 }
}
int select_proc(cherg **head,int *type, char *fileName, int *countElem, int *num)
{
 int pi;
 pi=(*head)->pi;
 *type=(*head)->type;
 strcpy(fileName,(*head)->fileName);
 *countElem=(*head)->countElem;
 *num=(*head)->num;
if ((*head)->next==NULL)//only one element
 {
 delete (*head);//delete him
 (*head)=NULL;
 }
else
 {
 (*head)=(*head)->next;//head element is next element
 delete (*head)->pred;//delete previous head
 (*head)->pred=NULL;
 }
 return pi;
}
int main() {
cherg *head=NULL;
 int type=1,num;
 char fileName[20];
 int countElem;
 int pi;
 srand(time(NULL));
for(int i=0;i<3;i++)
 {
 addProcess(&head,i,type,fileName,countElem);
 }
 while(head != NULL){
 pi=select_proc(&head,&type,fileName,&countElem,&num);
 kill(pi,SIGCONT);
 waitpid(pi,&stat,WCONTINUED | WSTOPPED);
 cout<<"execute "<<num<<" "<<pi<<" "<<type<<endl;
 sleep(1);
 if (type!=3)
 {
  addProcess(&head,num+3,type+1,fileName,countElem);
 };
 }
 return 0;
}