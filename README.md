# TravelOptions
Data Structures - Project1

For this project I had to implement a C++ class called TravelOptions.
This class has several functions to compare and sort different travel options.
Suppose you are planning a trip and have numerous different travel options. Each option has two traits called price and travel time.
This class uses a singly linked list data structure to store each option as a node.

The TravelOptions.h file has the class implementation
The toy.cpp was used to test the class functions.

Member Functions:

- compare
  static function which takes two options and determines their relationship (better, worse, equal, incomparable).
  runtime: constant
  
- is_sorted
  determines if calling object is sorted
  runtime: linear
 
 - is_pareto
  determines if travel options are pareto
  runtime: quadratic
  
 - is_pareto_sorted
  determines if options are both pareto and sorted
  runtime: linear
  
 - insert_sorted
  insertes new option into a sorted option list
  runtime: linear
  
 - insert_pareto_sorted
   Takes new option and, if it is  not dominated by a  pre-existing option, inserts  it and, in turn 
   deletes any  pre-existing options which  have become dominated.
   runtime: linear
   
 - union_pareto_sorted
   Takes two lists (calling  object and a parameter) and  constructs their "pruned  union" as a new list.  
   Returns  new TravelOptions object as a  pointer.
   runtime: linear
   
 - prune_sorted
   takes sorted option list and  deletes all dominated elements  (if any).  
   runtime: linear
   
 - join_plus_plus
   Takes two option lists  (calling object and a  parameter).  One list  gives options for the  first leg 
   of a trip;  the other gives options  for the second.
 
 - join_plus_max
   Takes two option lists  (calling object and a  parameter).  One list  gives options for  traveler A and the  
   other gives options for  traveler B.
   runtime: linear
   
 - split_sorted_pareto
   takes max_price as a  parameter.  Splits  option list into  options with price no  greater than 
   max_price  and those greater than  max_price.
   runtime: linear
   

