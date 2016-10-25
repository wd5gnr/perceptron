#include "learn.h"
#include <iostream>

using namespace std;

#define TRAINRATE 0.5
#define THRESHOLD 1.0

float and_trng[4][2] = {
  { 0.0, 0.0},
  { 0.0, 1.0 },
  { 1.0, 0.0 },
  { 1.0, 1.0}
};

int and_trngr[]={ 0, 0, 0, 1 };



float or_trng[][2] = {
  { 0.0, 0.0 },
  { 1.0, 0.0 },
  { 0.0, 1.0},
  { 1.0, 1.0}
};

int or_trngr[]={ 0, 1, 1, 1 };



  
float testvec[4][2]= {
  { 0.0, 0.0 },
  { 1.0, 0.0 },
  { 0.0, 1.0 },
  { 1.0, 1.0 }
};



void engine(Learn &lobj, float trainingdata[4][2],int trainresult[])
{
  int i, ct=0;
  int again=1;
  lobj.setThreshold(THRESHOLD);
  while (again)
    {
      int res;
      again=0;
      for (i=0;i<4;i++) lobj.train(trainingdata[i],trainresult[i],TRAINRATE);
      for (i=0;i<4;i++) if ((res=lobj.fetch(trainingdata[i]))!=trainresult[i]) again=1;	
      if (++ct>500 && again)
	{
	  again=0;
	  cout<<"500 training generations failed to converge"<<endl;
	}
	  
    }
  cout<<"Training done."<<endl;
  lobj.dump();
  for (i=0;i<sizeof(testvec)/sizeof(testvec[0]);i++)
    {
      cout<<"Case "<<i<<":"<<lobj.fetch(testvec[i])<<endl;
    }

}


int main(int argc, char *argv[])
{
  cout<<"Starting 'AND' case"<<endl;
  Learn lobj(2,2);
  engine(lobj,and_trng,and_trngr);

  cout<<"Starting 'OR' case"<<endl;
  lobj.init(); // NOTE: resets THRESHOLD
  engine(lobj,or_trng, or_trngr);
}



