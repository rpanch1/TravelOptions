
#ifndef _TRVL_OPTNS_H
#define _TRVL_OPTNS_H

#include <iostream>
#include <vector>
#include <utility>

class TravelOptions{

public:
  enum Relationship { better, worse, equal, incomparable};

private:
  struct Node {
    double price;
    double time;
    Node *next;

    Node(double _price=0, double _time=0, Node* _next=nullptr){
      price = _price; 
      time = _time; 
      next = _next;
    }

  };

  /* TravelOptions private data members */
  Node *front;  // pointer for first node in linked list (or null if list is empty)
  int _size;

public:
  // constructors
  TravelOptions() {
    front = nullptr;
    _size=0;
  }

  ~TravelOptions( ) {
    clear();
  }

/**
 * func: clear
 * desc: Deletes all Nodes currently in the list
 */
  void clear(){
    Node *p, *pnxt;
    p = front;
    while(p != nullptr) {
      pnxt = p->next;
      delete p;
      p = pnxt;
    }
    _size = 0;
    front = nullptr;
  }

  /**
 * func: size
 * desc: returns the number of elements in the list
 */
  int size( ) const {
    return _size;
  }

  /**
  * func: compare
  * desc: compares option A (priceA, timeA) with option B (priceB, timeA) and
  *		returns result (see enum Relationship above):
  *
  *       There are four possible scenarios:
  *		- A and B are identical:  option A and option B have identical price and time: 
  *			 		ACTION:  return equal
  *		- A is better than B:  option A and B are NOT equal/identical AND 
  *					option A is no more expensive than option B AND
  *					option A is no slower than option B 
  *					ACTION:  return better
  *		- A is worse than B:  option A and B are NOT equal/identical AND 
  *					option A is at least as expensive as option B AND
  *					option A is no faster than option B 
  *					ACTION:  return worse
  *					NOTE:  this means B is better than A
  *		- A and B are incomparable:  everything else:  one option is cheaper and 
  *					      the other is faster.
  *					ACTION:  return incomparable 
  *
  * COMMENTS:  since this is a static function, there is no calling object.  
  *            To call it from a client program, you would do something like this:
  *
  */          
  static Relationship compare(double priceA, double timeA, double priceB, double timeB) {

    if(priceA == priceB && timeA == timeB){
      return equal;
    }
    if(priceA <= priceB && timeA <= timeB){
      return better;
    }
    if(priceA >= priceB && timeA >= timeB){
      return worse;
    }
    else{
      return incomparable;
    }
  }

private:

/**
 * func: compare(Node*, Node*)
 * desc: private utilty function for comparing two options given as 
 *       Node pointers.
 *
 * COMMENT:  depends on public compare(double,double,double,double) being implemented.
 *           You might find this version handy when manipulating lists
 */
  static Relationship compare(Node *a, Node *b) {
    if(a==nullptr || b==nullptr) {
      std::cout << "ERR: compare(Node*,Node*);  null pointer passed!!! Whoops!" << std::endl;
      return incomparable;
    }
    return compare(a->price, a->time, b->price, b->time);
  }
    
  public:

  /**
 * func: push_front
 * desc: Adds a <price,time> option to the front of the list (simple primitive for building lists)
 */
  void push_front(double price, double time) {
    front = new Node(price, time, front);
    _size++;
  }

  /**
 * func: from_vec
 * desc: This function accepts a C++ standard libary vector of pair<double,double>.
 *       Each pair is interpreted as a <price,time> option and a TravelOptions object
 *       is containing exactly the same options as in the vector (and in the same order).
 *	
  * returns: a pointer to the resulting TravelOptions object
  */
  static TravelOptions * from_vec(std::vector<std::pair<double, double> > &vec) {
    TravelOptions *options = new TravelOptions();

    for(int i=vec.size()-1; i>=0; i--) {
      options->push_front(vec[i].first, vec[i].second);
    }
    return options;
  }

  /**
 * func: to_vec
 * desc: Utility function which creates a C++ standard libary vector of pair<double,double>.
 *       and populates it with the options in the calling object (in the same order).
 *       As in from_vec the "first" field of each pair maps to price and the "second"
 *         field maps to time.
 *	
  * returns: a pointer to the resulting vector
  */
  std::vector<std::pair<double, double>> * to_vec() const {
    std::vector<std::pair<double, double>> *vec = new std::vector<std::pair<double, double>>();
    Node *p = front;

    while(p != nullptr) {
        vec->push_back(std::pair<double,double>(p->price, p->time));
        p = p->next;
    }
    return vec;
  }

  /**
  * func: is_sorted
  * desc: we consider an option list sorted under the following conditions:
  *	
  *		- the options are in non-decreasing order of price AND
  *		- time is used as a tie-breaker for options with identical price.
  *
  *       For example, using the notation <price, time> to represent an option:
  *
  *             <5.1, 10.0>  must be before <5.6, 9.0>    (price is less, so time ignored)
  *             <6.2, 4.1>   must be AFTER  <6.2, 3.9>    (identical price; tie broken by
  *                                                         smaller time (3.9 in this case)).
  *
  *       If two or more options are identical in BOTH price and time, they are 
  *       indistinguishible and must appear as a consecutive "block" if the list is
  *       to be considered sorted.
  *
  * returns:  true if sorted by the rules above; false otherwise.
  *
  */
  bool is_sorted()const{
    //if list is empty or has one option
    if (size()== 0 || size() == 1)
      return true;
    
    Node* tmp = front;
    Node* tmp2 = front->next;

    while(tmp2 != nullptr){
      if(tmp->price > tmp2->price){
        return false;
      }
      if(tmp->price == tmp2->price){
        if(tmp->time > tmp2->time){
          return false;
        }
      }
      tmp = tmp2;
      tmp2 = tmp->next;
    }
    return true;
  }

  /**
  * func: is_pareto
  * desc: returns true if and only if:
  *
  *        all options are distinct  (no duplicates)  AND
  *        none of the options are 'suboptimal' (i.e., for each option X, there DOES NOT EXIST
  *           any other option Y such that Y dominates X).  There are several equivalent
  *           ways of stating this property...
  *
  * REQUIREMENTS:
  *    - the list must be unaltered
  *    - no memory allocation, arrays, etc. allowed
  *    - RUNTIME:  quadratic in number of options n (i.e., O(n^2)).
  *
  * REMEMBER:  the list does not need to be sorted in order to be pareto
  */
  bool is_pareto() const{
    //if list is empty or has one option
    if (size() == 0 || size() == 1)
      return true;

    Node* tmp = front;
    Node* tmp2 = front->next;

    while(tmp2 != nullptr){
      while(tmp2 != nullptr){
        if(TravelOptions::compare(tmp, tmp2) != incomparable)
          return false;    
        tmp2 = tmp2->next;
      }
      tmp = tmp->next;
      tmp2 = tmp->next;
    }
    return true;
  }

  /**
  * func: is_pareto_sorted() 
  * desc: returns true if and only if the list is:
  *    - STRICTLY INCREASING IN price AND
  *    - STRICTLY DECREASING IN time 
  *
  * COMMENTS:  notice that because of the runtime requirement, you cannot simply do this:
  *
  */
  bool is_pareto_sorted() const{
    //if list is empty or has one option
    if (size() == 0 || size() == 1)
      return true;

    Node* tmp = front;
    Node* tmp2 = front->next;

    while(tmp2 != nullptr){
      if(tmp->price > tmp2->price)
        return false;

      if(TravelOptions::compare(tmp, tmp2) != incomparable)
        return false;

      tmp = tmp2;
      tmp2 = tmp->next;
    }
    return true;
  }

/**
 * func: insert_sorted
 * preconditions:  given collection (calling object) must be sorted (but not necessarily
 *                   pareto).  If this is not the case, false is returned (code provided).
 *                   (returns true otherwise, after insertion complete).                 
 *                 
 * desc:  inserts option <price,time> (given as parameters) into option list (calling object)
 *          while keeping it sorted.  Recall:  ordering by price; tie-breaker is time.
 *
 * RUNTIME:  O(n).
 */
bool insert_sorted(double price, double time) {
  if(!is_sorted()) 
    return false;
  
  //if list is empty push from front
  if(size() == 0){
    push_front(price,time);
    return true;
  }

  //check if the node can be inserted in the front
  if(front->price > price || (front->price == price && front->time >= time)){
    push_front(price,time);
    return true;
  }

  Node *tmp = front;
  Node *tmp2 = front;
  Node *insertNode = new Node(price,time,nullptr);

  //iterate to the node before insertNode
  while(tmp2->price < price){
    //if last node is reached, insert at the end
    if(tmp2->next == nullptr){
        tmp2->next = insertNode;
        _size++;
        return true;
    }
    tmp = tmp2;
    tmp2 = tmp2->next;
  }

  //check if the next nodes are equal to insertion Node
  while(tmp2->price == price){
    //compare the times
    if(tmp2->time < insertNode->time){;
      if(tmp2->next == nullptr){
        tmp2->next = insertNode;
        _size++;
        return true;
      }
      tmp = tmp2;
      tmp2 = tmp2->next;
    }
    else if (tmp2->time > insertNode->time){
      insertNode->next = tmp2;
      tmp->next = insertNode;
      _size++;
      return true;
    }
    else{
      if(tmp2->next == nullptr){
        tmp2->next = insertNode;
        _size++;
        return true;
      }
      tmp = tmp2;
      tmp2 = tmp2->next;
    }
  }
  insertNode->next = tmp2;
  tmp->next = insertNode;
  _size++;
  return true;
}

/**
 * func: insert_pareto_sorted
 * preconditions:  given collection (calling object) must be sorted AND pareto (pruned).
 *                 if this is not the case, false is returned.
 *                 (code segment for this test given).
 * desc: (assuming the list is sorted and pareto): if the option given by the parameters 
 *       price and time is NOT dominated by already existing options, the following results:
 *            - new option <price,time> is inserted maintaining the sorted property of the
 *                list, AND
 *            - any pre-existing options which are now suboptimal (i.e., dominated by the
 *                newly added option) are deleted.
 *       If the new option is suboptimal, the list is simply unchanged.
 *       In either case, true is returned (i.e., as long as the preconditions are met).
 *       
 * RUNTIME REQUIREMENT:  O(n)       
 *
 */
bool insert_pareto_sorted(double price, double time) {
  if(!is_pareto_sorted()) 
      return false;

  Node *tmp = front;
  Node *insertNode = new Node(price,time,nullptr);

  //if list is empty
  if(tmp == nullptr){
    push_front(price,time);
    return true;
  }
    
  while(tmp != nullptr){
    //check if input is dominated by existing options
    if(TravelOptions::compare(insertNode, tmp) == worse || TravelOptions::compare(insertNode, tmp) == equal){
      delete insertNode;
      return true;
    }
    tmp = tmp->next;
  }
    
  tmp = front;
  Node *before_tmp = front;
    
  while(tmp != nullptr && before_tmp != nullptr){
    if(TravelOptions::compare(insertNode, tmp) == better){
      if(tmp == front){
        Node* erase = front;
        front = front->next;
        tmp = front;
        before_tmp = tmp;
        delete erase;
        _size--;
      }
      else{
        Node *erase = tmp;
        before_tmp->next = tmp->next;
        tmp = tmp->next;
        delete erase;
        _size--;
      }
    }else{
      before_tmp = tmp;
      tmp = tmp->next;
    };
  }
  return insert_sorted(price,time);
}

  /**
 * func: union_pareto_sorted
 * precondition:  calling object and parameter collections must both be sorted and pareto (if not, nullptr is returned).
 * desc: constructs "the sorted, pareto" union (as in set-union excluding dominated options) of the two collections and returns 
 *               it as a newly created object.
 * RUNTIME:  linear in the length of the two lists O(n+m)
 * 
 */
  TravelOptions * union_pareto_sorted(const TravelOptions &other)const{
  if(!is_pareto_sorted() || !other.is_pareto_sorted())
    return nullptr;

  TravelOptions *unionList = new TravelOptions();

  //if both lists are empty
  if(front == nullptr && other.front == nullptr){
    return unionList;
  }

  Node* tmp = front;
  Node* tmp2 = unionList->front;
  //insert the first list into unionList
  while(tmp != nullptr){
    if(tmp2 == nullptr){
      unionList->push_front(tmp->price, tmp->time);
      tmp = tmp->next;
      unionList->_size++;
      tmp2 = unionList->front;
    }
    else{
      Node* insertNode = new Node(tmp->price, tmp->time, nullptr);
      tmp2->next = insertNode;
      tmp2 = tmp2->next;
      tmp = tmp->next;
      unionList->_size++;
    }
  }

  //insert the second list into unionList in sorted order
  tmp = other.front;
  tmp2 = unionList->front;
  Node *before_tmp2 = unionList->front;

  //if the unionList is empty, copy list2 into unionList
  if(unionList->size() == 0){
    while(tmp != nullptr){
      if(unionList->size() == 0){
        unionList->push_front(tmp->price, tmp->time);
        tmp = tmp->next;
        unionList->_size++;
      }
      else{
        Node* insertNode = new Node(tmp->price, tmp->time, nullptr);
        tmp2->next = insertNode;
        tmp2 = tmp2->next;
        tmp = tmp->next;
        unionList->_size++;
      }
    }
    return unionList;
  }
  else{
    while(tmp != nullptr && tmp2 != nullptr){
      if (tmp2->price < tmp->price){
        before_tmp2 = tmp2;
        tmp2 = tmp2->next;
      }
        
      else if((tmp2->price == tmp->price) && (tmp2->time == tmp->time)){
        tmp = tmp->next;
      }
      else if((tmp2->price == tmp->price) && (tmp2->time < tmp->time)){
        tmp = tmp->next;
      }
      else if((tmp2->price == tmp->price) && (tmp2->time > tmp->time)){
        if(tmp2 == unionList->front){
          unionList->push_front(tmp->price,tmp->time);
          tmp = tmp->next;
          before_tmp2 = unionList->front;
          tmp2 = before_tmp2->next;
          unionList->_size++;
        }else{
          Node* insertNode = new Node(tmp->price, tmp->time, nullptr);
          insertNode->next = tmp2;
          before_tmp2->next = insertNode;
          unionList->_size++;
          tmp = tmp->next;
          before_tmp2 = before_tmp2->next;
        }
    }
    else if(tmp2->price > tmp->price){
      if(tmp2 == unionList->front){
        unionList->push_front(tmp->price,tmp->time);
        tmp = tmp->next;
        before_tmp2 = unionList->front;
        unionList->_size++;
      }
      else{
        Node* insertNode = new Node(tmp->price, tmp->time, nullptr);
        insertNode->next = tmp2;
        before_tmp2->next = insertNode;
        unionList->_size++;
        tmp = tmp->next;
        before_tmp2 = before_tmp2->next;
        }
      }
    }
  }
  if (tmp2 == nullptr && tmp != nullptr){
    while(tmp != nullptr){
      Node* insertNode = new Node(tmp->price, tmp->time, nullptr);
      before_tmp2->next = insertNode;
      tmp = tmp->next;
      unionList->_size++;
      before_tmp2 = before_tmp2->next;
    }
  }
  //prune the sorted unionList
  bool x = unionList->prune_sorted();
  return unionList;
}
    
  /**
 * func:  prune_sorted
 * precondition:  given collection must be sorted (if not, false is returned).
 * desc: takes sorted list of options and removes dominated entries
 *         (and eliminates any duplicates).
 * RUNTIME:  linear in the length of the list (O(n))
 * 
 */
  bool prune_sorted(){
    if(!is_sorted()) 
      return false;

    //if list is empty or only one node in list
    if(front == nullptr || front->next == nullptr){
      return true;
    }

    Node *before_tmp = front;
    Node *tmp = before_tmp->next;

    while(tmp != nullptr){
      if(compare(before_tmp, tmp) == equal || compare(before_tmp, tmp) == better){
        Node* erase = tmp;
        tmp = tmp->next;
        before_tmp->next = tmp;
        delete erase;
        _size--;
      }
      else if(compare(before_tmp, tmp) == worse){
        if(before_tmp == front){
          Node* erase = before_tmp;
          front = front->next;
          before_tmp = before_tmp->next;
          tmp = tmp->next;
          delete erase;
          _size--;
        }
        else{
          Node* erase = before_tmp;
          before_tmp = before_tmp->next;
          tmp = tmp->next;
          delete erase;
          _size--;
        }
      }
      else{
        before_tmp = before_tmp->next;
        tmp = tmp->next;
      }
    }
    return true;
  }

  /**
 * func: join_plus_plus
 * preconditions:  none -- both the calling object and parameter are just TravelOptions objects (not necessarily
 *                 sorted or pareto).
 * param: other; a reference to another to a list of TravelOptions (thus, there are two option lists: the calling 
 *               object and the parameter). 
 * desc: suppose you are planning a trip composed of two "legs":
 *
 *         first you travel from city X to city Y (this is part of your plan - you need to stop in city Y)
 *         then you continue from city Y to city Z
 *
 *       Let the calling object be the options for the X-to-Y leg and the parameter be the options for the
 *       Y-to-Z leg.
 *   
 *       Your job is to construct a pareto-sorted set of options for the entire trip and return the resulting
 *       TravelOptions object as a pointer.
 *
 *       In principle, any option <p1,t1> for the X-to-Y leg can be paird with any option <p2,t2> for the Y-to-Z leg.
 *       In such a pairing, the total price will be p1+p2 and the total time will be t1+t2, (i.e., option <p1+p2,t1+t2>).
 *       (This is why the operation is called join_plus_plus)
 *
 *       This function's  job is to determine the sorted-pareto list of options for the entire trip and return it
 *       as a pointer to the object.
 * returns:  a pointer to a TravelOptions object capturing all non-dominated options for the entire trip from X-to-Z
 *              (i.e., even though the given lists may not be sorted or pareto, the resulting list will be both).
 *
 */
  TravelOptions * join_plus_plus(const TravelOptions &other) const {
    TravelOptions *plusplus = new TravelOptions();
    Node* L1 = front;
    Node* L2 = other.front;

    while(L1 != nullptr){
      while(L2 != nullptr){
        plusplus->insert_pareto_sorted(L1->price+L2->price, L1->time+L2->time);
        L2 = L2->next;
      }
      L1 = L1->next;
      L2 = other.front;
    }
    return plusplus;
}

  /**
 * func: join_plus_max
 * preconditions:  both the calling object and the parameter are sorted-pareto lists (if not, nullptr is returned).
 * desc: imagine a different scenario (vs. join_plus_plus):  you are a parent with two children -- one living in city A and
 *         the other living in city C -- and they are both coming home for the holidays (let's call your home C).  
 *       You have a pareto-sorted option list for the A-to-C trip (this is the calling object) and a pareto-sorted option list
 *         for the B-to-C trip.
 *       Consider a particular option for the A-to-C trip <p1, t1> and a particular option for the B-to-C trip <p2,t2>.
 *         Clearly, the total price for selecting these two options is p1+p2.
 *         However (unlike in the plus_plus case), adding the t1 and t2 doesn't make sense right (the trips are in "parallel").
 *         What you care about as a parent is when BOTH of your children are home.  This is determine by MAX(t1,t2).
 *         Thus, the resulting "composite" option in this case would be <p1+p2, MAX(t1,t2)> (hence the name join_plus_max).
 *  
 * RUNTIME:  let N and M be the lengths of the respective lists given; your runtime must be linear in N+M (O(N+M)).
 *
 */
  TravelOptions * join_plus_max(const TravelOptions &other) const {
  if(!is_pareto() || !(other.is_pareto()))
    return nullptr;
    
  TravelOptions* plusmax = new TravelOptions();
  Node* L1 = front;
  Node* L2 = other.front;
  Node* tmp = plusmax->front;

  while(L1 != nullptr && L2 != nullptr){
    if(L1->time > L2->time){
      Node* insertNode = new Node(L1->price+L2->price, L1->time, nullptr);
      if(plusmax->_size == 0){
        plusmax->front = insertNode;
        tmp = plusmax->front;
        plusmax->_size++;
        L1 = L1->next;
      }
      else{
        tmp->next = insertNode;
        tmp = tmp->next;
        plusmax->_size++;
        L1 = L1->next;
      }
    }
    else if(L1->time < L2->time){
      Node* insertNode = new Node(L1->price+L2->price, L2->time, nullptr);
      if(plusmax->_size == 0){
        plusmax->front = insertNode;
        tmp = plusmax->front;
        plusmax->_size++;
        L2 = L2->next;
      }
      else{
        tmp->next = insertNode;
        tmp = tmp->next;
        plusmax->_size++;
        L2 = L2->next;
      }
    }
    else if (L1->time == L2->time){
      Node* insertNode = new Node(L1->price+L2->price, L1->time, nullptr);
      if(plusmax->_size == 0){
        plusmax->front = insertNode;
        tmp = plusmax->front;
        plusmax->_size++;
        L1 = L1->next;
        L2 = L2->next;
      }
      else{
        tmp->next = insertNode;
        tmp = tmp->next;
        plusmax->_size++;
        L1 = L1->next;
        L2 = L2->next;
      }
    }
  }
  return plusmax;
}

  /**
 * func: sorted_clone
 * desc: returns a sorted TravelOptions object which contains the same elements as the current object
 */
TravelOptions * sorted_clone() {
  TravelOptions *sorted = new TravelOptions();
  Node *p = front;
  
  while(p != nullptr) {
    sorted->insert_sorted(p->price, p->time);
    p = p->next;
  }
  return sorted;
}

  /**
 * func: split_sorted_pareto
 * precondition:  given list must be both sorted and pareto (if not, nullptr is returned; 
 *    code already given).
 * desc: This function takes a sorted-pareto option list and splits into two option lists:
 *
 *        - the options with price less than or equal to max_price (if any) are retained in the calling
 *            object (and only those are retained in the calling object).
 *        - the other, more expensive options are used to populate a new TravelOptions object which
 *            is returned as a pointer (i.e., the expensive options are returned)..
 * 
 * returns: pointer to expensive options or nullptr if the calling object is not pareto-sorted.
 * RUNTIME:  linear in the length of the given list (O(n)).
 * */
  TravelOptions * split_sorted_pareto(double max_price) {
  if(!is_pareto_sorted())
    return nullptr;
    
  TravelOptions *greater = new TravelOptions();

  //if list is empty->return an empty list
  if(front == NULL){
    return greater;
  }

  Node* tmp = front;
  Node* before_tmp = front;
  greater->_size = _size;

  while(tmp->price <= max_price){
    before_tmp = tmp;
    tmp = tmp->next;
    greater->_size--;
    if(tmp == nullptr){
      greater->front = nullptr;
      return greater;
    }
  }
  if(tmp == front){
    front = nullptr;
    greater->front = tmp;
    return greater;
  }
  before_tmp->next = nullptr;
  greater->front = tmp;
  return greater;
}

  /**
 * func: display
 * desc: prints a string representation of the current TravelOptions object
 */
void display(){
  printf("   PRICE      TIME\n");
  printf("---------------------\n");
  Node * p = front;
  
  while(p!=nullptr) {
    printf("   %5.2f      %5.2f\n", p->price, p->time);
    p = p->next;
  }
}

/**
 * func:  checksum
 * desc:  Performs and XOR of all node pointers and returns result as
 *        an unsigned int.
 * 
 */
unsigned long int checksum() const {
  unsigned long int s = 0;
  Node *p = front;

  while (p != nullptr)
  {
    s = s ^ ((unsigned long int)p);
    p = p->next;
  }
  return s;
}
};
#endif



