# turtlebinning

## Introduction
This is a wrapper around ROOT's __TKDTreeBinning__ which bins n-dimensional data using recursive binary partitioning. At
each step, the algorithm splits every bin into two bins with equal numbers of
entries in
each bin. This continues until the specified number of bins is reached. The number of entries in every bin will be the same. For example, if you have 1,000,000 points in a 10-dimensional space and ask for 1,000 bins in that space, then the number of points per bin will be a 1,000. Typically, the volume of bins will vary across the space. The density at the center of each bin is approximated by the number of points per bin / bin volume.


## Dependencies
This package, which is written in C++ but can be used with Python, depends on the data analysis package [ROOT](https://root.cern.ch) from CERN and the C++ Standard Template Library.

## Installation
Download the code as follows.
```bash
git clone https://github.com/hbprosper/turtle
cd turtle
make
```
If you have the software environment system [Miniconda](https://docs.conda.io/en/latest/miniconda.html) installed, you can install the Python module __turtlebinning__ in your current miniconda environment.
```bash
make install
```
and uninstall it with
```bash
make uninstall
```
Otherwise, source __setup.sh__ to define a few environment variables. If you are running a shell other than bash, you will have to make a version of setup.sh for your shell.

__turtlebinning__ can work directly on flat ROOT files (basically,
tables). However, it can also work directly on an appropriately structured
array as in the example below. This is convenient if you are using dataframes.

## Example
Suppose you have a __pandas__ dataframe, __df__, and you wish to bin
points defined by the variables *x* , *y*, and *z*. Here are the steps
assuming you already have access to __df__.

### Step 1

Import the relevant modules.
```python
import turtlebinning as tt
from array import array
```

### Step 2
Create an array and fill it as follows.
```python
data  = array('d')
data.extend(df['x'])
data.extend(df['y'])
data.extend(df['z'])
```

### Step 3
Bin the data.
```python
nbins   = 1000
npoints = len(data)
nparams = 3
ttb     = tt.Turtle(data, nbins, npoints, nparams)
```

Suppose you want to access the indices of the points that lie in bin number 42. You
can do so as follows.
```python
bin = 42
ind = ttb.indices(bin)
```
This returns an object of type __vector\<int\>__ containing the
indices (that is, the ordinal values of the points in the dataframe
__df__) of the points that lie in bin 42. If you want the points
themselves, do:
```python
bin = 42
pts = ttb.points(bin)
```
which returns the points in an object of type
__vector\<vector\<double\> \>__.
