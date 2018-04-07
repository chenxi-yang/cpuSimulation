#include<QApplication>
#include<QDialog>
#include<QtGui>
#include<iostream>
#include<cstdio>
#include<string>
#include<cstring>
#include<fstream>
#include<map>
#include<stdio.h>
#include<fstream>
using namespace std;
/*my thought
 *first we need get some regs ahead,cause we will use them in the control
 *we call them P_balabala,them we use them and
 *use control to judge whether stall or bubble
 *second we use the have been controlled things to renew the FDENW_things
 *if they are stall,at this clk cycle,we do nothing
 *            bubble,we set FDENW_things as NOP
 *            SAOK,we use those fdenw_things of the pre-stage to renew FDENW_things
 *ok,now we use those FDENW_things to renew those fdenw_ings of the same stage
 *at the same time,every Write stage,we need to renew the Stat,which is of the total system
 *we do the things above again and again,until the Stat does not equal SAOK
*/
#define IHALT 0
#define INOP 1
#define IRRMOVL 2
#define IIRMOVL 3
#define IRMMOVL 4
#define IMRMOVL 5
#define IOPL 6
#define IJXX 7
#define ICALL 8
#define IRET 9
#define IPUSHL 10
#define IPOPL 11
#define FNONE 0
#define RESP 4
#define RNONE 15
#define ALUADD 0
#define SAOK 1
#define SADR 2
#define SINS 3
#define SHLT 4

#define SBUB 0

const int code_head=0x00000000;
const int MAXLEN=10000000;

static map<int,int> mem;
static string s;
//ifstream in("prog5.yo",ios::in);
//static ofstream out("out.txt", ios::app);

class total{
    public:
    //according to my feeling
   // slots:
   // void ClickButton();
    //according to my feeling
    char ch_code[MAXLEN];
    int circle_time;
    int temp_circle_time;
    char ch_use[MAXLEN];
    int Reg[16];
    int instructions;
    double CPI;

    int Stat;
    bool ZF,OF,SF,CF;
    bool instr_valid,imem_error,need_regids,need_valC;

    //use for the first store,preserve them at first
    int P_d_srcA, P_d_srcB;
    bool P_need_use_ALU, P_e_set_cc, P_ZF, P_SF, P_OF, P_CF;
    int P_e_aluA, P_e_aluB, P_e_alufun, P_e_Cnd, P_e_valE;
    int P_m_mem_addr, P_m_mem_data, P_m_valM, P_m_stat;
    bool P_m_mem_read, P_m_mem_write, P_m_dimem_error;


    int F_predPC;
    int f_stat,f_icode,f_ifun,f_rA,f_rB,f_valC,f_valP,f_pc,f_predPC;
    bool F_stall,F_bubble;

    int D_stat,D_icode,D_ifun,D_rA,D_rB,D_valC,D_valP;
    int d_stat,d_icode,d_ifun,d_valC,d_valA,d_valB,d_dstE,d_dstM,d_srcA,d_srcB,d_rvalA,d_rvalB;
    bool D_stall,D_bubble;

    int E_stat,E_icode,E_ifun,E_valC,E_valA,E_valB,E_dstE,E_dstM,E_srcA,E_srcB;
    int e_stat,e_icode,e_Cnd,e_valE,e_valA,e_dstE,e_dstM,e_aluA,e_aluB,e_alufun;
    bool e_set_cc;//judge whether we need renew the conditonal code
    bool E_stall,E_bubble;

    bool need_use_ALU;
    int M_stat,M_icode,M_Cnd,M_valE,M_valA,M_dstE,M_dstM;
    int m_stat,m_icode,m_mem_addr,m_mem_data,m_valE,m_valM,m_dstE,m_dstM;
    bool m_mem_read,m_mem_write,m_dimem_error;
    bool M_stall,M_bubble;

    int W_stat,W_icode,W_valE,W_valM,W_dstE,W_dstM;
    int W_stall,W_bubble;

    void show_register(){
        printf("eax------0x%x\n",Reg[0]);
        printf("ecx------0x%x\n",Reg[1]);
        printf("edx------0x%x\n",Reg[2]);
        printf("ebx------0x%x\n",Reg[3]);
        printf("esp------0x%x\n",Reg[4]);
        printf("ebp------0x%x\n",Reg[5]);
        printf("esi------0x%x\n",Reg[6]);
        printf("edi------0x%x\n",Reg[7]);
        printf("stat------%d\n",Stat);
        printf("--------------------------\n");
    }

    string select_icode(int x) {       //for the output,cpu.not num,cpu.but instruction
        string word;
        if (x == 0) word = "I_HALT";
        if (x == 1) word = "I_NOP";
        if (x == 2) word = "I_RRMOVL";
        if (x == 3) word = "I_IRMOVL";
        if (x == 4) word = "I_RMMOVL";
        if (x == 5) word = "I_MRMOVL";
        if (x == 6) word = "I_OPL";
        if (x == 7) word = "I_JXX";
        if (x == 8) word = "I_CALL";
        if (x == 9) word = "I_RET";
        if (x == 10) word = "I_PUSHL";
        if (x == 11) word = "I_POPL";
        return word;
    }
    string select_reg(int x) {
        string word;
        if (x == 0) word = "R_EAX";
        if (x == 1) word = "R_ECX";
        if (x == 2) word = "R_EDX";
        if (x == 3) word = "R_EBX";
        if (x == 4) word = "R_ESP";
        if (x == 5) word = "R_EBP";
        if (x == 6) word = "R_ESI";
        if (x == 7) word = "R_EDI";
        if (x >= 8) word = "R_NONE";
        return word;
    }
    string select_stat(int x) {
        string word;
        if (x == 0) word = "STAT_BUB";
        if (x == 1) word = "STAT_AOK";
        if (x == 2) word = "STAT_ADR";
        if (x == 3) word = "STAT_INS";
        if (x == 4) word = "STAT_HLT";
        return word;
    }


    void prepare(){  //initialize every instruction to nop
                     //initialize every register to 15==RNONE
       // memset(this,0,sizeof(*this));  //use for
        circle_time=0;
        instructions=0;
        CPI=1;
        memset(ch_code,0,sizeof(char));
        f_predPC=code_head;
        f_icode=d_icode=e_icode=m_icode=0;
        f_stat=d_stat=e_stat=m_stat=0;
        Stat=1;
        f_rA=f_rB=RNONE;
        d_dstE=d_dstM=d_srcA=d_srcB=RNONE;
        e_dstE=e_dstM=RNONE;
        m_dstE=m_dstM=RNONE;
        ZF=1;OF=SF=0;
    }
 /***********************these are about read/write/address**********************/
    void mem_read(int head,int len,int &data,bool &imem_error){//small endian read
        data=0;
        for(int i=0;i<len;i++){
            if(head+i>=0){
               int x=mem[head+i];
               x=x&0xFF;
               data=data^(x<<(i*8));
            }
            else{
               imem_error=1;
               break;
            }
        }
    }

    void mem_read_try(int head,int len,int &data,bool &imem_error){
        for(int i=0;i<len;i++){
            if(head+i<0){
                imem_error=1;
                break;
            }
        }
    }

    void mem_write(int head,int len,int data,bool &imem_error){
        for(int i=0;i<len;i++){
            if(head+i>=0){
               mem[head+i]=data&(0xFF);
               data=data>>8;
            }
            else{
               imem_error=1;
               break;
            }
        }
    }

    void mem_write_try(int head,int len,int data,bool &imem_error){
        for(int i=0;i<len;i++){
            if(head+i<0){
               imem_error=1;
               break;
            }
        }
    }
     /*read the txt file*/
     /*here we need to trans sth*/
/*important!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!****************/
     void read_in(char* ch){//char* ch
        // FILE *ch_stream;

         ifstream in(ch);
         int head=code_head;
         int a, b;
         int len = 0;
         int lenuse=0;
         while (getline(in,s)) {
          /*   for(int i=0;i<s.size();i++){
                 ch_code[len++]=s[i];
             }*/
             for (int i = 0; i < s.size(); i+=2) {
                 while(s[i]==' ')i++;
                 if(s[i]=='0'&&s[i+1]=='x'){
                     while(s[i]!=':')
                         i++;
                     i++;
                 }
                 while(s[i]==' ')i++;
                 if(s[i]=='|') break;
                 if ('0' <= s[i] && '9' >= s[i]) a = s[i] - '0';
                 else if ('a' <= s[i] && s[i] <= 'z') a = s[i] - 'a'+10;
                 else a = s[i] - 'A'+10;
                 ch_use[lenuse++]=s[i];
                 if ('0' <= s[i+1] && '9' >= s[i+1]) b = s[i+1] - '0';
                 else if ('a' <= s[i+1] && s[i+1] <= 'z') b = s[i+1] - 'a'+10;
                 else b = s[i+1] - 'A'+10;
                 ch_use[lenuse++]=s[i+1];
                 ch_code[len++] = (char)((a << 4) + b);
             }
         }//read the hex code,turn them into chars
         for(int i=0;i<len;i++){
            mem_write(head,1,ch_code[i],imem_error);
            head++;
         }
         //we need add the halt instruction at the end of mem
         mem_write(head,1,0x00,imem_error);
         head++;
     }
     void read_in_2(char* ch){//char* ch
        // FILE *ch_stream;

         ifstream in(ch);
         int head=code_head;
         int a, b;
         int len = 0;
         int lenuse=0;
         //file read
       //  ch_stream=fopen("1.trick.y86","r");//this is about use file read
       //  int len=fread(ch_code,sizeof(char),MAXLEN,ch_stream);
         while (getline(in,s)) {
             for(int i=0;i<s.size();i++){
                 ch_code[len++]=s[i];
                 ch_use[lenuse++]=s[i];
             }
         }//read the hex code,turn them into chars

         for(int i=0;i<len;i++){
            mem_write(head,1,ch_code[i],imem_error);
            head++;
         }
         //we need add the halt instruction at the end of mem
         mem_write(head,1,0x00,imem_error);
         head++;
     }

/********************Some neccessary preperations************************************/
     void select_PC(){
         //mispredicted branch.Fetch at incremented PC
         if(M_icode==IJXX&&!M_Cnd) f_pc=M_valA;
         //Completion of RET instruction
         else if(W_icode==IRET) f_pc=W_valM;
         //Default:Use predicted value of PC
         else f_pc=F_predPC;
     }
     void Split(int f_pc,int &f_a,int &f_b,bool &imem_error){
         int x;
         f_a=f_b=0;
         mem_read(f_pc,1,x,imem_error);
         if(imem_error) return;
         f_a=x>>4;
         f_b=x&0xF;
     }

     void SelFwdA(){
         if(D_icode==ICALL||D_icode==IJXX) d_valA=D_valP;
         else if(d_srcA==e_dstE) d_valA=e_valE;
         else if(d_srcA==M_dstM) d_valA=m_valM;
         else if (d_srcA == M_dstE) d_valA = M_valE;
         else if (d_srcA == W_dstM) d_valA = W_valM;
         else if (d_srcA == W_dstE) d_valA = W_valE;
         else d_valA = d_rvalA;
     }

     void FwdB(){
     if (d_srcB == e_dstE) d_valB = e_valE;
     else if (d_srcB == M_dstM) d_valB = m_valM;
     else if (d_srcB == M_dstE) d_valB = M_valE;
     else if (d_srcB == W_dstM) d_valB = W_valM;
     else if (d_srcB == W_dstE) d_valB = W_valE;
     else d_valB = d_rvalB;
     }

     int get_Register(int src){
         if(src<8) return Reg[src];
         return 0xf;
         //cause it means no reg,or error ???
     }

     void set_Register(int src,int val){
         if(src<8) Reg[src]=val;
     }

     void setConditionCode(int a,int b,int t,int alufun,bool &ZF,bool &SF,bool &OF,bool &CF){
          ZF=(t==0);
          SF=(t<0);
          if(alufun==0){
             OF=((a<0)==(b<0))&&((t<0)!=(a<0));
             CF=((unsigned int)t<(unsigned int)a);
          }else if(alufun==1){
             OF=((a<0)!=(b<0))&&((t<0)==(a<0));
             CF=((unsigned int)t>(unsigned int)b);
          }else{
             OF=0;
             CF=0;
          }
     }

     void ALU(int alua,int alub,int alufun,int &valE,bool setCC,bool &ZF,bool &SF,bool &OF,bool &CF){
         if(alufun==0) valE=alua+alub;
         else if(alufun==1) valE=alub-alua;
         else if(alufun==2) valE=alua&alub;
         else if(alufun==3) valE=alua^alub;
         if(setCC)
            setConditionCode(alua,alub,valE,alufun,ZF,SF,OF,CF);
     }

     void cond(int &e_Cnd,bool ZF,bool SF,bool OF,bool CF){
         if(E_icode==IRRMOVL||E_icode==IJXX){
             if(E_ifun==0) e_Cnd=1;
             if(E_ifun==1) e_Cnd=(SF^OF)|ZF;
             if(E_ifun==2) e_Cnd=(SF^OF);
             if(E_ifun==3) e_Cnd=ZF;
             if(E_ifun==4) e_Cnd=!ZF;
             if(E_ifun==5) e_Cnd=!(SF^OF);
             if(E_ifun==6) e_Cnd=!(SF^OF)&!ZF;
         }else
             e_Cnd=0;
     }

     void D_prepare(int &d_srcA,int &d_srcB){
         if(D_icode==IRRMOVL||D_icode==IRMMOVL||D_icode==IOPL||D_icode==IPUSHL) d_srcA=D_rA;
         else if(D_icode==IPOPL||D_icode==IRET) d_srcA=RESP;
         else d_srcA=RNONE;

         if (D_icode == IOPL || D_icode == IRMMOVL || D_icode == IMRMOVL) d_srcB = D_rB;
     else if (D_icode == IPUSHL || D_icode == IPOPL || D_icode == ICALL || D_icode == IRET) d_srcB = RESP;
     else d_srcB = RNONE;
     }

     void E_prepare(bool &need_use_ALU,int &e_aluA,int &e_aluB,int &e_alufun,bool &e_set_cc,int &e_Cnd,int &e_valE,bool &ZF,bool &SF,bool &OF,bool &CF){
         need_use_ALU=1;
     /********************aluA****************/
         if (E_icode == IRRMOVL || E_icode == IOPL) e_aluA = E_valA;
         else if (E_icode == IIRMOVL || E_icode == IRMMOVL || E_icode == IMRMOVL) e_aluA = E_valC;
         else if (E_icode == ICALL || E_icode == IPUSHL) e_aluA = -4;
         else if (E_icode == IRET || E_icode == IPOPL) e_aluA = 4;
         else e_aluA = need_use_ALU = 0;
     /********************aluB******************/
         if (E_icode == IRMMOVL || E_icode == IMRMOVL || E_icode == IOPL || E_icode == ICALL || E_icode == IPUSHL || E_icode == IRET || E_icode == IPOPL) e_aluB = E_valB;
         else if (E_icode == IRRMOVL || E_icode == IIRMOVL) e_aluB = 0;
         else e_aluB = need_use_ALU = 0;
     /*********alufun***********/
         if(E_icode==IOPL) e_alufun=E_ifun;
         else e_alufun=ALUADD;

         e_set_cc=((E_icode==IOPL)&&!(m_stat==SADR||m_stat==SINS||m_stat==SHLT)&&!(W_stat==SADR||W_stat==SINS||W_stat==SHLT));

         if(need_use_ALU)
            ALU(e_aluA,e_aluB,e_alufun,e_valE,e_set_cc, ZF, SF, OF, CF);
         else
            e_valE=0;

         cond(e_Cnd,ZF,SF,OF,CF);
     }

     void M_prepare(int &m_mem_addr, int &m_mem_data, bool &m_mem_read, bool &m_mem_write){
         /*******m_mem_addr************/
         if(M_icode==IRMMOVL||M_icode==IPUSHL||M_icode==ICALL||M_icode==IMRMOVL) m_mem_addr=M_valE;
         else if(M_icode==IPOPL||M_icode==IRET) m_mem_addr=M_valA;
         else m_mem_addr=0;
         /*******m_mem_data**********/
         if(M_icode==IRMMOVL||M_icode==IPUSHL||M_icode==ICALL) m_mem_data=M_valA;
         else m_mem_data=0;

         m_mem_read=((M_icode==IMRMOVL)||(M_icode==IPOPL)||(M_icode==IRET));
         m_mem_write=((M_icode==IRMMOVL)||(M_icode==IPUSHL)||(M_icode==ICALL));
     }

     /**************total   control*********************/
     void F_Control(){
         F_stall=(((E_icode==IMRMOVL||E_icode==IPOPL)&&(E_dstM==P_d_srcA||E_dstM==P_d_srcB))||(IRET==D_icode||IRET==E_icode||IRET==M_icode));
     }

     void D_Control(){
         D_stall=(E_icode==IMRMOVL||E_icode==IPOPL)&&(E_dstM==P_d_srcA||E_dstM==P_d_srcB);
         D_bubble = (E_icode == IJXX && !P_e_Cnd);
     D_bubble |= (!((E_icode == IMRMOVL || E_icode == IPOPL) && (E_dstM == P_d_srcA || E_dstM == P_d_srcB))) && (IRET == D_icode || IRET == E_icode || IRET == M_icode);
     }

     void E_Control(){
         E_bubble=(E_icode==IJXX&&!P_e_Cnd);
         E_bubble|=(E_icode==IMRMOVL||E_icode==IPOPL)&&(E_dstM==P_d_srcA||E_dstM==P_d_srcB);
     }

     void M_Control(){
         M_bubble=(P_m_stat == SADR || P_m_stat == SINS || P_m_stat == SHLT);
         M_bubble|=(W_stat == SADR || W_stat == SINS || W_stat == SHLT);
     }

     void W_Control(){
         W_stall=(W_stat==SADR||W_stat==SINS||W_stat==SHLT);
     }

     void Control(){
         P_ZF = ZF;
     P_SF = SF;
     P_OF = OF;
     P_CF = CF;
     P_e_Cnd = e_Cnd;
     P_e_set_cc = e_set_cc;
    //if I dont use the following functions,is ok?????????????????????
     D_prepare(P_d_srcA,P_d_srcB);
     E_prepare(P_need_use_ALU,P_e_aluA,P_e_aluB,P_e_alufun,P_e_set_cc,P_e_Cnd,P_e_valE,P_ZF,P_SF,P_OF,P_CF);

     P_m_dimem_error=m_dimem_error;
     M_prepare(P_m_mem_addr, P_m_mem_data, P_m_mem_read, P_m_mem_write);

     if (P_m_mem_read)
        mem_read_try(P_m_mem_addr, 4, P_m_valM, P_m_dimem_error);

     if (P_m_mem_write)
        mem_write_try(P_m_mem_addr, 4, P_m_mem_data, P_m_dimem_error);

     if(m_dimem_error) P_m_stat=SADR;
     else P_m_stat=M_stat;

     F_Control();
     D_Control();
     E_Control();
     M_Control();
     W_Control();
     }

     void Send(){ //to refresh the FDEMW_things (according to the control)
         if(!F_stall&&!F_bubble)
            F_predPC=f_predPC;
         if(!D_stall&&!D_bubble){
            D_stat=f_stat;
            D_icode=f_icode;
            D_ifun=f_ifun;
            D_rA=f_rA;
            D_rB=f_rB;
            D_valC=f_valC;
            D_valP=f_valP;
         }
         //if bubble,set INOP
         if(!D_stall&&D_bubble){
            D_stat=SBUB;
            D_icode=1;
            D_ifun=0;
            D_rA=D_rB=RNONE;
          //  D_valC=D_valP=0;
         }
  /*what if STALL?if stall,at this clk cycle,we will do nothing*/
         if(!E_stall&&!E_bubble){
            E_stat=d_stat;
            E_icode=d_icode;
            E_ifun=d_ifun;
            E_valC=d_valC;
            E_valA=d_valA;
            E_valB=d_valB;
            E_dstE=d_dstE;
            E_dstM=d_dstM;
            E_srcA=d_srcA;
            E_srcB=d_srcB;
         }
         if(!E_stall&&E_bubble){
            E_stat = SBUB;
        E_icode = 1;
        E_ifun = 0;
        E_valA = E_valB = 0;
        E_dstE = E_dstM = RNONE;
        E_srcA = E_srcB = RNONE;
         }
         if(!M_stall&&!M_bubble){
            M_stat=e_stat;
            M_icode=e_icode;
            M_Cnd=e_Cnd;
            M_valE=e_valE;
            M_valA=e_valA;
            M_dstE=e_dstE;
            M_dstM=e_dstM;
         }
         if(!M_stall&&M_bubble){
            M_stat = SBUB;
        M_icode = 1;
            M_Cnd = 0;
        M_valE = M_valA = 0;
        M_dstE = M_dstM = RNONE;
         }
         if (!W_stall && !W_bubble){
        W_stat = m_stat;
        W_icode = m_icode;
        W_valE = m_valE;
        W_valM = m_valM;
        W_dstE = m_dstE;
        W_dstM = m_dstM;
     }

     }
     void show_vairable(){
         /*************************print part***********************/
         //cout<<"------------------------------Fetch Stage-----------------------------------"<<endl;
         cout << "   predPC--0x" << hex << F_predPC<<endl;
         //cout<<"------------------------------Decode Stage----------------------------------"<<endl;
         cout << "   stat--" << select_stat(D_stat) << " icode--" << select_icode(D_icode) << " ifun--" << D_ifun << " rA--" << select_reg(D_rA) << " rB--" << select_reg(D_rB) << " valC--" << hex << D_valC << " valP--" << hex << D_valP << endl;
         //cout<<"------------------------------Execute Stage---------------------------------"<<endl;
         cout << "stat--" << select_stat(E_stat) << " icode--" << select_icode(E_icode) << " ifun--" << E_ifun << " valC--" << hex << E_valC << " valA--" << hex << E_valA << " valB--" << hex << E_valB << " dst_E--" << select_reg(E_dstE) << " dst_M--" << select_reg(E_dstM) << " srcA--" << select_reg(E_srcA) << " srcB--" << select_reg(E_srcB) << endl;
         //cout<<"------------------------------Memory Stage----------------------------------"<<endl;
         cout << " stat--" << select_stat(M_stat) << " icode--" << select_icode(M_icode) << "valE--" << hex << M_valE << " valA--" << hex << M_valA << " dstE--" << select_reg(M_dstE) << " dstM--" << select_reg(M_dstM) << endl;
         //cout<<"------------------------------Write Stage-----------------------------------"<<endl;
         cout << "stat--" << select_stat(W_stat) << " icode--" << select_icode(W_icode) << " valE--" << hex << W_valE << " valM--" << hex << W_valM << " dstE--" << select_reg(W_dstE) << " dstM--" << select_reg(W_dstM) << endl;
         /*************************print part**********************/
     }
     /*************here we need to refresh the fdemw_things(according to the FDEMW_things)**********/
     void Fetch(){
       //  if(F_stall) return ;
         //printf("predPC--0x%08x\n", F_predPC);
        // cout << "f: predPC--0x" << hex << f_predPC <<"icode "<<select_icode(f_icode)<< endl;
        // cout << "   predPC--0x" << hex << F_predPC<<endl;

         select_PC();

         Split(f_pc,f_icode,f_ifun,imem_error);
         f_pc++;

         instr_valid=!((f_icode!=IRRMOVL&&f_icode!=IOPL&&f_icode!=IJXX&&f_ifun!=0)||(f_icode > 11));

         need_regids=(f_icode==IRRMOVL||f_icode==IOPL||f_icode==IPUSHL||f_icode==IPOPL||f_icode==IIRMOVL||f_icode==IRMMOVL||f_icode==IMRMOVL);

         if(need_regids){
            Split(f_pc,f_rA,f_rB,imem_error);
            f_pc++;
         }else{
            f_rA=RNONE;
            f_rB=RNONE;// no reg visit
         }

         need_valC=(f_icode==IIRMOVL||f_icode==IRMMOVL||f_icode==IMRMOVL||f_icode==IJXX||f_icode==ICALL);
         if(need_valC){
            mem_read(f_pc,4,f_valC,imem_error);
            f_pc+=4;
         }else
            f_valC=0;

         f_valP=f_pc;

         if(imem_error) f_stat=SADR;
         else if(!instr_valid) f_stat=SINS;
         else if(f_icode==IHALT) f_stat=SHLT;
         else  f_stat=SAOK;

         if (f_icode == IJXX || f_icode == ICALL) f_predPC = f_valC;
         else f_predPC = f_valP;
     }

     void Decode(){
         //printf("stat--%d  icode--%d  ifun--%d  rA--%d  rB--%d  valC--0x%08x  valP--0x%08x\n", D_stat, D_icode, D_ifun,D_rA, D_rB, D_valC, D_valP);
        // cout << "d: stat--" << select_stat(d_stat) << " icode--" << select_icode(d_icode) << " ifun--" << d_ifun << " valC--" << hex << d_valC << endl;
         //cout << "   stat--" << select_stat(D_stat) << " icode--" << select_icode(D_icode) << " ifun--" << D_ifun << " rA--" << select_reg(D_rA) << " rB--" << select_reg(D_rB) << " valC--" << hex << D_valC << " valP--" << hex << D_valP << endl;
         //printf("stat--%s  icode--%s  ifun--%d  rA--%s  rB--%s  valC--0x%08x  valP--0x%08x\n", select_stat(D_stat), select_icode(D_icode),D_ifun,select_reg(D_rA),select_reg(D_rB),D_valC, D_valP);
         D_prepare(d_srcA,d_srcB);//wait wait i need think
                                      //yes i am sure that i know sth about it
                                      //actually,sth draw in my mind
         d_rvalA=get_Register(d_srcA);
         d_rvalB=get_Register(d_srcB);

         if(D_icode==IRRMOVL||D_icode==IIRMOVL||D_icode==IOPL) d_dstE=D_rB;
         else if(D_icode==IPUSHL||D_icode==IPOPL||D_icode==ICALL||D_icode==IRET) d_dstE=RESP;
         else d_dstE=RNONE;

         if(D_icode==IMRMOVL||D_icode==IPOPL) d_dstM=D_rA;
         else d_dstM=RNONE;

         SelFwdA();
         FwdB();

         d_icode=D_icode;
         d_ifun=D_ifun;
         d_stat=D_stat;
         d_valC=D_valC;
     }//prepare function is based on what,some thing we can

     void Execute(){
        // printf("stat--%d  icode--%d  ifun--%d  valC--0x%08x  valA--0x%08x  valB--0x%08x  dstE--%d  dstM--%d  srcA--%d  srcB--%d\n", E_stat,E_icode, E_ifun,E_valC, E_valA,E_valB,E_dstE, E_dstM,E_dstM,E_srcA,E_srcB);
        // cout << "stat--" << select_stat(E_stat) << " icode--" << select_icode(E_icode) << " ifun--" << E_ifun << " valC--" << hex << E_valC << " valA--" << hex << E_valA << " valB--" << hex << E_valB << " dst_E--" << select_reg(E_dstE) << " dst_M--" << select_reg(E_dstM) << " srcA--" << select_reg(E_srcA) << " srcB--" << select_reg(E_srcB) << endl;

         E_prepare(need_use_ALU,e_aluA,e_aluB,e_alufun,e_set_cc,e_Cnd,e_valE,ZF,SF,OF,CF);

         if(E_icode==IRRMOVL&&!e_Cnd) e_dstE=RNONE;
         else e_dstE=E_dstE;

         e_icode=E_icode;
         e_stat=E_stat;
         e_dstM=E_dstM;
         e_valA=E_valA;
     }

     void Memory(){
         //cout << " stat--" << select_stat(M_stat) << " icode--" << select_icode(M_icode) << "valE--" << hex << M_valE << " valA--" << hex << M_valA << " dstE--" << select_reg(M_dstE) << " dstM--" << select_reg(M_dstM) << endl;
        // printf("stat--%d  icode--%d  valE--%08x  valA--%08x  dstE--%d  dstM--%d\n",M_stat, M_icode, M_valE, M_valA, M_dstE, M_dstM);
         //printf("stat--%s  icode--%s  valE--%08x  valA--%08x  dstE--%s  dstM--%s\n", select_stat(M_stat), select_icode(M_icode), M_valE, M_valA, select_reg(M_dstE), select_reg(M_dstM));
         M_prepare(m_mem_addr,m_mem_data,m_mem_read, m_mem_write);

         if(m_mem_read)
            mem_read(m_mem_addr,4,m_valM,m_dimem_error);

         if(m_mem_write)
            mem_write(m_mem_addr,4,m_mem_data,m_dimem_error);

         if(m_dimem_error) m_stat=SADR;
         else m_stat=M_stat;

         m_icode=M_icode;
         m_valE=M_valE;
         m_dstM=M_dstM;
         m_dstE=M_dstE;
     }
     void Write(){
        // cout << "stat--" << select_stat(W_stat) << " icode--" << select_icode(W_icode) << " valE--" << hex << W_valE << " valM--" << hex << W_valM << " dstE--" << select_reg(W_dstE) << " dstM--" << select_reg(W_dstM) << endl;
         //printf("stat--%d  icode--%d  valE--%08x  valM--%08x  dstE--%d  dstM--%d\n", W_stat, W_icode, W_valE, W_valM, W_dstE, W_dstM);
         //printf("stat--%s  icode--%s  valE--%08x  valM--%08x  dstE--%s  dstM--%s\n", select_stat(W_stat), select_icode(W_icode), W_valE, W_valM, select_reg(W_dstE), select_reg(W_dstM));
         if(W_stat==SBUB) Stat=SAOK;
         else Stat=W_stat;

         if(Stat!=SAOK) return;

         set_Register(W_dstE,W_valE);
         set_Register(W_dstM,W_valM);
     }
};
