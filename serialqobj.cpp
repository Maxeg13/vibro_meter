#include "serialqobj.h"
//#include "stand_dev.h"
#include "ftt.h"
int stop_bit=1;
int stop_cnt;
bool hear=1;
float time;


serial_obj::serial_obj(QString qstr, myCurve* _MC, vector<fcomplex>& _ft):ft(_ft)
{
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
        readVar=killRange(readVar,12);
        MC->dataRefresh((int8_t)readVar);
        float xx=(int8_t)readVar;
        WT.extract(xx);
//       for(int i=5;i<7;i++)
//        qDebug()<<WT.mas[10][WT.im];
    }
    //            FTC->dataRefresh();
    if(cnt<3000)
    {
        cnt++;
        ftt( (int8_t)readVar,ft,time);
    }
}
