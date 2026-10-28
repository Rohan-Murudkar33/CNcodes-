#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    char ip[16];
    printf("Enter the 32-bit IP address: ");
    scanf("%s",ip);

    char class;
    int NID,HID;
    int i=0;
    int int_ip=0,x;
    while(ip[i]!='.'){
        x=ip[i]-'0';
        int_ip=int_ip*10+x;
        i++;
    }

    if(int_ip>0 && int_ip<=123){
            class='A';
            NID=8;
            HID=24;
        }
        else if(int_ip<=193){
            class='B';
            NID=16;
            HID=16;
        }
        else if(int_ip<=223){
            class='C';
            NID=24;
            HID=8;
        }
        else if(int_ip<=239){
            class='D';
            NID=-1;
            HID=-1;
        }
        else if(int_ip<=255){
            class='E';
            NID=-1;
            HID=-1;
        }
        else{
            printf("Invalid IP address");
            return 1;
        }

    char DBA[100];
    int count=0;
    int temp_index=0;
    if (class=='A'){
        while(count!=1){
            DBA[temp_index]=ip[temp_index];
            temp_index++;
            if(ip[temp_index]=='.'){
                count++;
            }
        }
        int j=0;
        while(j<=(3-count)){
            DBA[temp_index]='.';
            temp_index++;
            DBA[temp_index]='2';
            temp_index++;
            DBA[temp_index]='5';
            temp_index++;
            DBA[temp_index]='5';
            temp_index++;

            j++;
        }
        DBA[temp_index]='\0';
    }

    if (class=='B'){
        while(count!=2){
            DBA[temp_index]=ip[temp_index];
            temp_index++;
            if(ip[temp_index]=='.'){
                count++;
            }
        }
        int j=0;
        while(j<=(3-count)){
            DBA[temp_index]='.';
            temp_index++;
            DBA[temp_index]='2';
            temp_index++;
            DBA[temp_index]='5';
            temp_index++;
            DBA[temp_index]='5';
            temp_index++;

            j++;
        }
        DBA[temp_index]='\0';
    }

    if (class=='C'){
        while(count!=3){
            DBA[temp_index]=ip[temp_index];
            temp_index++;
            if(ip[temp_index]=='.'){
                count++;
            }
        }
        int j=0;
        while(j<=(3-count)){
            DBA[temp_index]='.';
            temp_index++;
            DBA[temp_index]='2';
            temp_index++;
            DBA[temp_index]='5';
            temp_index++;
            DBA[temp_index]='5';
            temp_index++;

            j++;
        }
        DBA[temp_index]='\0';
    }
    if (class=='D' || class=='E'){
        DBA[temp_index]='\0';
    }

    printf("\nIP Address: %s",ip);
    printf("\nClass: %c",class);
    printf("\nBits in network id :%d",NID);
    printf("\nBits in host id :%d",HID);
    printf("\nLBA :%s",DBA);
    printf("\nDBA :%s",DBA);

    return 0;
}