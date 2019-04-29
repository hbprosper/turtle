#ifndef TURTLE_H
#define TURTLE_H
// ---------------------------------------------------------------------------
// Created May 11, 2011 by Harrison Prosper and Sezen Sekmen
// Updated May 21, 2015 HBP - Implement Fill
//         May 18, 2019 HBP - add FindBin method
// ---------------------------------------------------------------------------
#include <vector>
#include <string>
#include "TKDTreeBinning.h"
// ---------------------------------------------------------------------------
///
class Turtle
{
public:
  ///
  Turtle();

  ///
  Turtle(std::string rootfilename, 
	 std::vector<std::string>& variablenames, 
	 std::string treename,
	 int numberofbins,
	 int numberofpoints=-1);
  
  ///
  Turtle(std::vector<std::string>& rootfilenames, 
	 std::vector<std::string>& variablenames, 
	 std::string treename,
	 int numberofbins,
	 int numberofpoints=-1);

  ///
  void Build(std::string rootfilename, 
	     std::vector<std::string>& variablenames, 
	     std::string treename,
	     int numberofbins,
	     int numberofpoints=-1);
  
  ///
  void Build(std::vector<std::string>& rootfilenames, 
	     std::vector<std::string>& variablenames, 
	     std::string treename,
	     int numberofbins,
	     int numberofpoints=-1);

  virtual ~Turtle();
  
  ///
  double GetBinDensity(int bin) { return _btree->GetBinDensity(bin); }

  ///
  double GetBinVolume(int bin)  { return _btree->GetBinVolume(bin); }

  ///
  const double* GetBinCenter(int bin) { return _btree->GetBinCenter(bin); }

  ///
  const double* GetBinWidth(int bin)  { return _btree->GetBinWidth(bin); }

  ///
  void SortBinsByDensity(bool ascend=true) { _btree->SortBinsByDensity(ascend); }

  ///
  int FindBin(std::vector<double>& point)
  { return (int)_btree->FindBin(&point[0]); }

  ///
  std::vector<std::vector<double> >  GetPointsInBin(int bin)
    { return _btree->GetPointsInBin( bin ); }
  

  /// Clear counts and variances
  void clear();
  
  /// Histogram data from specified file.
  void fill(std::string rootfilename,
	    std::string weightname="");

    /// Histogram data from specified files.
  void fill(std::vector<std::string>& rootfilenames,
	    std::string weightname="");

  /// Histogram data.
  void fill(std::vector<double>& point, double weight=1);
  
  /// Return bin counts for histogrammed data.
  std::vector<double> counts() { return _counts; }

  /// Return bin variances for histogrammed data.
  std::vector<double> variances() { return _variances; }

  
  ClassDef(Turtle,0)
  
 private:
  TKDTreeBinning*          _btree;
  std::vector<std::string> _rootfilenames;
  std::vector<std::string> _variablenames; 
  std::string              _treename;
  std::vector<double>      _counts;
  std::vector<double>      _variances;
  int _numberofbins;

  size_t  _datasize;
  double* _data;
  double* _ReadTree(std::vector<std::string>& rootfilenames, 
                    std::vector<std::string>& variablenames, 
                    std::string treename,
                    int numberofbins,
                    int numberofpoints);
};

#endif
