// ---------------------------------------------------------------------------
// File: Turtle.cc
// Description: Bin data in n-dimensions using bins created by recursive
// binary partitioning, such that the count in each bin is the same.
// Created May 11, 2011 by Harrison Prosper and Sezen Sekmen
// Updated May 21, 2015 HBP - Implement Fill
// ---------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <cassert>
#include "TMath.h"
#include "TChain.h"
#include "Turtle.h"
// ---------------------------------------------------------------------------

using namespace std;

Turtle::Turtle()
  : _btree(0),
    _rootfilenames(vector<string>()),
    _variablenames(vector<string>()),
    _treename(""),
    _counts(vector<double>()),
    _variances(vector<double>()),
    _datasize(0),
    _data(0)
{
}

Turtle::~Turtle()
{
  if ( _btree ) delete _btree;
  if ( _data )  delete _data;
}

Turtle::Turtle(vector<string>& rootfilenames,
               vector<string>& variablenames,
               string treename,
               int numberofbins,
               int numberofpoints)
  : _btree(0),
    _rootfilenames(vector<string>()),
    _variablenames(vector<string>()),
    _treename(""),
    _counts(vector<double>()),
    _variances(vector<double>()),
    _datasize(0),
    _data(0)
{
  build(rootfilenames,
	variablenames,
	treename, 
	numberofbins,
	numberofpoints);
}

Turtle::Turtle(string rootfilename,
               vector<string>& variablenames,
               string treename,
               int numberofbins,
               int numberofpoints)
  : _btree(0),
    _rootfilenames(vector<string>()),
    _variablenames(vector<string>()),
    _treename(""),
    _counts(vector<double>()),
    _variances(vector<double>()),
    _datasize(0),
    _data(0)
{
  build(rootfilename,
	variablenames,
	treename, 
	numberofbins,
	numberofpoints);
}


void Turtle::build(vector<string>& rootfilenames,
		   vector<string>& variablenames,
		   string treename,
		   int numberofbins,
		   int numberofpoints)
{
  _rootfilenames = rootfilenames;
  _variablenames = variablenames;
  _treename      = treename;
  _counts        = vector<double>(numberofbins, 0);
  _variances     = vector<double>(numberofbins, 0);

  double* data = _readTree(rootfilenames,
			   variablenames,
			   treename, 
			   numberofbins,
			   numberofpoints);
  
 _btree = new TKDTreeBinning(_datasize,
			     variablenames.size(), 
			     data, 
			     numberofbins);
 
  _numberofbins = _btree->GetNBins();
}

void Turtle::build(string rootfilename,
		   vector<string>& variablenames,
		   string treename,
		   int numberofbins,
		   int numberofpoints)
{
  vector<string> rootfilenames(1, rootfilename);
  build(rootfilenames, 
	variablenames,
	treename, 
	numberofbins,
	numberofpoints);
}


namespace  {
  double zero(double) { return 0; }
};

void Turtle::fill(string rootfilename, string weightname)
{
  vector<string> rootfilenames(1, rootfilename);
  fill(rootfilenames, weightname);
}

void Turtle::fill(std::vector<std::string>& rootfilenames,
		  std::string weightname)
{
  assert( _btree );
  assert( (size_t)_btree->GetNBins() == _counts.size() );
  
  // Histogram data from given files and store values in _counts
  TChain chain(_treename.c_str());
  for (size_t i=0; i < rootfilenames.size(); i++)
    chain.Add(rootfilenames[i].c_str());

  int numberofpoints = chain.GetEntries();

  // Allocate enough space for the number of variables
  vector<double> row(_variablenames.size());

  // create a branch for each variable
  for (size_t i=0; i < _variablenames.size(); i++)
    chain.SetBranchAddress(_variablenames[i].c_str(), &row[i]);

  double weight = 1.0;
  if ( weightname != "" )
    chain.SetBranchAddress(weightname.c_str(), &weight);
  
  // clear _counts, _variances
  clear();

  for (int entry=0; entry < numberofpoints; entry++)
    {
      chain.GetEntry(entry);
      if ( entry % 25000 == 0 ) 
        cout << entry << endl;
      int bin = _btree->FindBin(&row[0]);
        if ( bin < 0 ) continue;
	if ( (size_t)bin >= _counts.size() ) continue;
      _counts[bin] += weight;
      _variances[bin] += weight*weight;
    }
}

void Turtle::fill(std::vector<double>& point, double weight)
{
  // Histogram data and store values in _counts
  assert( _btree );
  assert( (size_t)_btree->GetNBins() == _counts.size() );
  int bin = _btree->FindBin(&point[0]);
  if ( bin < 0 ) return;
  if ( (size_t)bin >= _counts.size() ) return;
  _counts[bin] += weight;
  _variances[bin] += weight*weight;
}

void Turtle::clear()
{
  // clear _counts
  transform(_counts.begin(), _counts.end(), _counts.begin(), zero);
  transform(_variances.begin(), _variances.end(), _variances.begin(), zero);
}

double* Turtle::_readTree(vector<string>& rootfilenames, 
                          vector<string>& variablenames, 
                          string treename, 
                          int numberofbins,
                          int numberofpoints)
{
  TChain chain(treename.c_str());
  for (size_t i=0; i<rootfilenames.size(); i++)
    chain.Add(rootfilenames[i].c_str());

  _numberofbins =  numberofbins;
  _datasize = chain.GetEntries();
  if (numberofpoints > 0)
    _datasize = TMath::Min(_datasize, (size_t)numberofpoints); 
  _entries_per_bin = _datasize / _numberofbins;
  _datasize  = _entries_per_bin * _numberofbins;
  
  // Allocate enough space for the number of points times the 
  // number of variables
  int buffersize = _datasize * variablenames.size();
  _data = new double[buffersize];

  // Allocate enough space for the number of variables
  vector<double> row(variablenames.size());

  for (size_t i=0; i < variablenames.size(); i++)
    chain.SetBranchAddress(variablenames[i].c_str(), &row[i]);

  for (size_t entry=0; entry < _datasize; entry++)
    {
      chain.GetEntry(entry);
      if ( entry % 25000 == 0 ) 
        cout << entry << endl;

      for (size_t j=0; j< variablenames.size(); j++)
        _data[entry+j*_datasize] = row[j];
    }
  cout << "number of bins: " << _numberofbins << endl;
  cout << "entries/bin:    " << _entries_per_bin << endl;
  cout << "data size:      " << _datasize << endl;
  return &_data[0];
}
