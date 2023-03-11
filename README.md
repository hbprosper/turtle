# turtlebinning

## Introduction
This is a wrapper around ROOT's __TKDTreeBinning__ n-dimensional
binningclass, which bins data using recursive binary partitioning. At
every step, the algorithm splits each bin in two with equal numbers of
entries in
each bin. This continues until every bin has desired number of
entries.

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
This returns a vector of type __vector<int>__ containing the
indices, that is, the ordinal values of the points in the dataframe
__df__, of the points that lie in bin 42. If you want the points
themselves, do:
```python
bin = 42
pts = ttb.points(bin)
```
which returns the points in a vector of type
__vector<vector<double>__ __>__.
