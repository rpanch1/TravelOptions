#include "TravelOptions.h"

#include <stdlib.h>
#include <iostream>

// using namespace std;

/*
simple demo program exercising some of the TravelOptions functions.

to compile:  g++ -std=c++11 toy.cpp

purpose:  just to make sure you have an idea of how to write 
driver/tester programs (this is not itself a tester program
per-se, just a pretty random sequence of calls..
*/


int main(int argc, char *argv[]){
   
   TravelOptions *options_p;
   TravelOptions *options_p2;
   TravelOptions *options;

   /*std::vector<std::pair<double, double>>
      vec{ {2, 7.8}, {3, 7.7}, {4, 7.6}, 
            {5, 7.5}, {6, 7.4}, {7, 7.3}, 
            {8, 7.2}, {9, 7.1}, {0,0} };*/
   
   std::vector<std::pair<double, double>> vec1{{10,9},{11,8},{12,7},{13,6},{14,5}};//sorted and pareto  
   std::vector<std::pair<double, double>> vec2{{0,0},{0,0},{0,0},{0,0},{0,0}};    //sorted
   std::vector<std::pair<double, double>> vec3{{0,0},{1,1.1},{2,2}, {3,3}, {4,4}}; //sorted but not paerto
   std::vector<std::pair<double, double>> vec4{{12,15},{8,20}, {9,17}};            //not sorted but paerto
   std::vector<std::pair<double, double>> vec5{{8,20},{9,17}, {12,15}};            //sorted and paerto
   std::vector<std::pair<double, double>> vec6{{4,2},{1,8}, {2,8}, {1,0}, {6,1}};  //not sorted or paerto
   std::vector<std::pair<double, double>> vec7{{1,8},{2,4},{3,1}};                 //sorted and paerto
   std::vector<std::pair<double, double>> vec8{{1,6},{2,3},{3,2}};                             //sorted and paerto
   std::vector<std::pair<double, double>> vec9{};                                 //sorted and paerto
   
   options_p2 = TravelOptions::from_vec(vec7);
   options_p = TravelOptions::from_vec(vec8);
   options = options_p2->join_plus_max(*options_p);
   if(options == nullptr)
      std::cout<<"nullptr\n";
   else
      options->display();

   // remember:  from_vec is a static function (hence, TravelOptions:: form)
   //options_p = TravelOptions::from_vec(vec3);
   
   /*bool a = options_p->insert_pareto_sorted(1,1);
   if(a == false){
      std::cout << "false\n";
   }
   else{
      std::cout << "true\n";
   }
   options_p->display();*/

   /*TravelOptions::Relationship r;
   r = TravelOptions::compare(0,0,0,0);
   if(r == TravelOptions::incomparable) 
      std::cout << "INCOMPARABLE!" << std::endl;
   if(r == TravelOptions::better) 
      std::cout << "BETTER!" << std::endl;
   if(r == TravelOptions::worse) 
      std::cout << "WORSE!" << std::endl;
   if(r == TravelOptions::equal) 
      std::cout << "EQUAL!" << std::endl;*/

   //options_p->clear();
   //delete options_p;
   //options_p2->clear();
   //delete options_p2;
   
   /*TravelOptions *options_p, *options_p2, *options_p3;

   std::vector<std::pair<double, double>>
      vec{ {1, 7}, {2, 8}, {2, 9}, 
         {3, 5}, {5, 8}, {5, 8}, 
         {5, 9}, {6, 12} };

   std::vector<std::pair<double, double>> *vec2;

   // remember:  from_vec is a static function (hence, TravelOptions:: form)
   options_p = TravelOptions::from_vec(vec);
   options_p2 = TravelOptions::from_vec(vec);

   options_p->display();
   std::cout << "size: " << options_p->size() << std::endl; 




   vec2 = options_p->to_vec();

   std::cout << "CONTENTS OF EXPORTED VECTOR:\n";
   for(int i=0; i<vec2->size(); i++) {
      std::cout << (*vec2)[i].first << (*vec2)[i].second << std::endl;
   }

   std::cout << "\n";

   if(options_p->is_sorted() ) 
      std::cout << "INCOMPARABLE!" << std::endl;

   if( options_p->is_pareto() )
      std::cout << "PARETO!" << std::endl;

   if(options_p->is_pareto_sorted())
      std::cout << "PARETO!" << std::endl;

   if(options_p->insert_sorted(22.0, 9.7))
      std::cout << "INSERT_SORTED WORKED!" << std::endl;

   if(options_p->insert_pareto_sorted(21.0, 9.7))
      std::cout << "INSERT_PARETO_SORTED WORKED!" << std::endl;

   options_p3= options_p->union_pareto_sorted(*options_p2);

   if(options_p3->prune_sorted())
      std::cout << "prune_sorted worked!" << std::endl;

   delete options_p3;

   options_p3 = options_p->join_plus_plus(*options_p2);

   delete options_p3;
   options_p3 = options_p->join_plus_max(*options_p2);


   options_p->clear();
   delete options_p;
   delete options_p2;
   delete options_p3;*/

   return 0;
}
