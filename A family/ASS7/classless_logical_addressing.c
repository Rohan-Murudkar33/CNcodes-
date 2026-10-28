#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void toBinary( int num, int binary[], int start) {
    int i,len=0,temp=num;

    for (i = start+7; i >= start; i--) {
        binary[i] = num % 2;
        num = num / 2;
    }
}

int toDecimal(int binary[], int start) {
    int decimal = 0, power = 1;

    for (int i = start + 7; i >= start; i--) {
        decimal = decimal + binary[i] * power;
        power = power * 2;
    }

    return decimal;
}

void print_ip(int ip[]) {
    for (int i = 0; i < 32; i += 8) {
        printf("%d", toDecimal(ip, i));

        if (i < 24)
            printf(".");
    }
    printf("\n");
}

void copy_ip(int ip[],int copy[]){
    for(int i=0;i<32;i++){
        copy[i]=ip[i];
    }
}

int main(){
    char ip[16];
    int int_ip[32]={0};
    printf("Enter the 32-bit IP address: ");
    scanf("%s",ip);

    int temp=0,i=0,x,octet=0;
    while(ip[i]!='\0'){
        if(ip[i]=='.'){
            toBinary(temp,int_ip,octet*8);
            temp=0;
            octet++;
            i++;
            continue;
        }
        x=ip[i]-'0';
        temp=temp*10+x;
        i++;
    }
    toBinary(temp,int_ip,octet*8);

    printf("IP-Address in 32-bit Binary :");
    for(int i=0;i<32;i++){
        printf("%d ",int_ip[i]);
    }

    int choice,fixLen_flag=0,varLen_flag=0;
    int no_of_subnets,no_of_fixedBits;
    printf("\n1) Fixed Length Subnets\n");
    printf("2) Variable Length Subnets\n");
    printf("Choose an option: ");
    scanf("%d",&choice);

    switch(choice){
        case 1:
            fixLen_flag=1;
            
            printf("\nNumber of fixed bits:");
            scanf("%d",&no_of_fixedBits);
            printf("\nHow many equal subnets are required:");
            scanf("%d",&no_of_subnets);

            int n = no_of_subnets;

            if (n < 1){
                printf("Equal subnets cannot be created");
                return 0;
            }

            while (n % 2 == 0){
                n /= 2;
            }

            if (n != 1){
                printf("Equal subnets cannot be created");
                return 0;
            }

            int host_bits=0,borrowed_bits=0,original_prefix;
            original_prefix=no_of_fixedBits;
            borrowed_bits=(int)ceil(log2(no_of_subnets));
            no_of_fixedBits+=borrowed_bits;
            host_bits=32-no_of_fixedBits;

            int copy_int_1st_ip[32];
            int copy_int_last_ip[32];
            copy_ip(int_ip,copy_int_1st_ip);
            copy_ip(int_ip,copy_int_last_ip);

            for (int i = 0; i < no_of_subnets; i++) {
                copy_ip(int_ip, copy_int_1st_ip);
                copy_ip(int_ip, copy_int_last_ip);

                for (int j = no_of_fixedBits; j < 32; j++) {
                    copy_int_1st_ip[j] = 0;
                    copy_int_last_ip[j] = 1;
                }

                int num = i;

                for (int j = no_of_fixedBits - 1; j >= original_prefix; j--) {
                    copy_int_1st_ip[j]= num % 2;
                    copy_int_last_ip[j]=copy_int_1st_ip[j];
                    num = num / 2;
                }

                printf("\nIP address of S%d", i+1);
                printf("\n1st IP : ");
                print_ip(copy_int_1st_ip);
                printf("Last IP : ");
                print_ip(copy_int_last_ip);
            }

            printf("\n\nUsable IP range: 2^%d-2\n",host_bits);
            int subnet_mask[32];
            copy_ip(int_ip,subnet_mask);
            for(int i=0;i<no_of_fixedBits;i++){
                subnet_mask[i]=1;
            }
            printf("\nSubnet Mask - ");
            print_ip(subnet_mask);
            break;

        case 2:

            printf("Enter number of subnets: ");
            scanf("%d",&no_of_subnets);



            break;
    }

    return 0;
}