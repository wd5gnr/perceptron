#include "learn.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

// Constructor
Learn::Learn(unsigned int stimulusct, unsigned int resultct)
{
  stimct=stimulusct+1;  // include one extra input for bias
  resct=resultct;
  weights=new float *[resct];
  for (unsigned int i=0;i<resct;i++) weights[i]=new float[stimct];
  result=new float[resct];
  // holding area for stimulus including bias
  _stim=new float[stimct];
  _stim[stimulusct]=1.0; // last element is always 1
  init();
}

Learn::~Learn()
{
  for (unsigned int i=0;i<resct;i++) delete [] weights[i];
  delete [] weights;
  delete [] result;
}

// Helper to set _stim to whaver stimulus + the bias
void Learn::set_stim(float s[])
{
  for (unsigned int i=0;i<stimct-1;i++) _stim[i]=s[i];
}
  

// Zero weights and reset threshold to 1.0
int Learn::init(void)
{
  unsigned i, j;
  threshold=1.0f;
  for (i=0;i<resct;i++)
    for (j=0;j<stimct;j++)
      weights[i][j]=0.0f;
  return 0;
}

// Do a training
int Learn::train(float *stim, unsigned int _result, float wt)
{
  unsigned i,j;
  unsigned int v=fetch(stim); // see what we think it is now
  int notneeded=1;  // assume we don't need training
  if (v!=_result) notneeded=0;  // if we got the wrong answer, we need training
  for (i=0;i<resct&&(notneeded==1);i++)
    {
      // if the correct response has a result< threshold, we do need training
      if (i==_result && result[i]<threshold)  notneeded=0;
      // if an incorrect response has a result>=threshold, we do need training
      if (i!=_result && result[i]>=threshold) notneeded=0;
    }
  // If not needed, we are done
  if (notneeded) return 0;
  // Get stimulus + bias
  set_stim(stim);
  // Prevent wt from being 0
  if (wt==0) wt=1.0;
  // for each result...
  for (i=0;i<resct;i++)
    {
      float w=wt;
      // if this is the correct result and it is >= threshold, then skip
      if (i==_result && result[i]>=threshold) continue;
      // if this is the incorrect result and it is < threshold, then skip
      if (i!=_result &&  result[i]<threshold) continue;
      // if we are on ann incorrect result, flip weight around (negative)
      if (i!=_result) w=-w;
      // Update weights
      for (j=0;j<stimct;j++)
	{
	  weights[i][j]+=w*_stim[j];
	}
    }
  return 0;
}

// Given a stimulus, get an answer
unsigned int Learn::fetch(float *stim)
{
  unsigned i,j;
  unsigned int nomax=1;  // assume we have not found a max
  unsigned int maxindex=0;
  float maxval;
  set_stim(stim);  // Get input
  // compute values
  for (i=0;i<resct;i++)
    {
      result[i]=0.0f;
      for (j=0;j<stimct;j++)
	{
	  result[i]+=weights[i][j]*_stim[j];
	}
      // Find maximum value
      if (nomax || result[i]>maxval)
	{
	  nomax=0;
	  maxindex=i;
	  maxval=result[i];
	}
    }
  return maxindex;  // return maximum index
}

// Load a file: threshold then weights
int Learn::load(const char *fn)
{
  unsigned i,j;
  ifstream ifile(fn);
  if (ifile.is_open())
    {
      ifile>>threshold;
      for (i=0;i<resct;i++)
	for (j=0;j<stimct;j++)
	  ifile>>weights[i][j];
    }
  ifile.close();
  return 0;
}


// Save a file: thresholds then weights
int Learn::save(const char *fn)
{
  unsigned i,j;
  ofstream ofile(fn);
  if (ofile.is_open())
    {
      ofile<<threshold<<" ";
      for (i=0;i<resct;i++)
	for (j=0;j<stimct;j++)
	  ofile<<weights[i][j]<<" ";
      ofile<<endl;
    }
  ofile.close();
  return 0;

}


// Display debug info including weights
void Learn::dump(void)
{
  unsigned i,j;
  cout<<"Stimct="<<stimct<<endl;
  cout<<"Resct="<<resct<<endl;
  cout<<"Threshold="<<threshold<<endl;
  cout<<"Weights\n";
  for (j=0;j<stimct;j++)
    {
    for (i=0;i<resct;i++)
      cout<<weights[i][j]<<"\t";
    cout<<endl;
    }
  cout<<"Results\n";
  for (j=0;j<resct;j++) cout<<result[j]<<"\t";
  cout<<endl;
}


