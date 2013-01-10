#ifndef FUNC_SIMULATOR_H_INCLUDED
#define FUNC_SIMULATOR_H_INCLUDED

void simulator( map<int,int>&, map<int,int>&, vector<DataItemVF>& , deque<Seq_min_fields *>&, int );
void searching( int , int , int , deque<Seq_min_fields *>& );
void simulator_initial( vector<DataItemVF>& , int, int );
int find_range_pmf( vector<double>& , double );

#endif // FUNC_SIMULATOR_H_INCLUDED
