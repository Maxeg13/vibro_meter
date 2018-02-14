#include "serialqobj.h"
//#include "stand_dev.h"

#include "ftt.h"
int stop_bit=1;
int stop_cnt;
bool hear=1;
float time;


serial_obj::serial_obj(QString qstr, myCurve* _MC, vector<fcomplex>& _ft):ft(_ft)
{


    perc_in_dim=10;
    perc_out_dim=4;
    vector<int> constr;

    constr.push_back(perc_in_dim);
    constr.push_back(5);
    constr.push_back(5);
    constr.push_back(perc_out_dim);//outputs
    perc=new perceptron(constr);

    perc_targ=new float*[perc_out_dim+1];
    for(int i=0;i<perc_out_dim+1;i++)
    {
        perc_targ[i]=new float[perc_out_dim];
    }


    for(int i=-1;i<perc_out_dim;i++)
    {
        for(int j=0;j<perc_out_dim;j++)
        {
            perc_targ[i+1][j]=(i==j)?1:0;
        }
    }

//    for(int i=0;i<perc_out_dim;i++)
//        cout<<perc_targ[1][i]<<endl;

    MC=_MC;
    std::string str1=qstr.toUtf8().constData();
    std::wstring str(str1.begin(),str1.end());

    hSerial.InitCOM(str.c_str());//was L"COM5"
    WT=Wavelet();
}

void serial_obj::init(QString qstr)
{


    std::string str1=qstr.toUtf8().constData();;
    std::wstring str(str1.begin(),str1.end());

    hSerial.InitCOM(str.c_str());//was L"COM5"

    //    featureOut.resize(3);
}

void serial_obj::close()
{
    hSerial.close();
}
serial_obj::~serial_obj()
{};
void serial_obj::doWork()
{

    while(1)
    {
        bool readVarON;
        readVar=(int8_t)hSerial.ReadCOM(readVarON);
        if(readVarON)
        {



            if(hear)
            {
                work();
            }
            else if(stop_bit)
            {
                work();
            }

            if((int8_t)readVar==127)
                stop_bit=1;
            //qDebug()<<fabs(ft[4]);
        }
        //            qDebug()<<(int8_t)readVar;
    }
}

void serial_obj::work()
{
    stop_cnt++;
    if(stop_cnt>MC->data.size()/2)
    {
        stop_bit=0;
        stop_cnt=0;
    }

    time+=dt;
    if((int8_t)readVar!=127)
    {
        readVar=killRange(readVar,9);
        MC->dataRefresh((int8_t)readVar);
        float xx=(int8_t)readVar;
        WT.extract(xx);
        //       for(int i=5;i<7;i++)
        //        qDebug()<<WT.mas[10][WT.im];
    }
    //            FTC->dataRefresh();
    if(cnt<100)
    {
        cnt++;
        ftt( (int8_t)readVar,ft,time);
    }
}
