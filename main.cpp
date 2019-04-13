#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
int DEBUG = 1;
class State{
    int * layout;
    public:
    State(){  //Default Constructor
        //Not in use anymore
        layout = new(int[9]);
        for(int i=0;i<9;++i){
            layout[i]=i;
        }
    }
    State(State * old){ //Constructor For Duplicating
        layout = new(int[9]);
        for(int i=0;i<9;++i){
            layout[i]=old->layout[i];
        }
    }
    State(int * old){ //Constructor For Begin
        layout = new(int[9]);
        for(int i=0;i<9;++i){
            layout[i]=old[i];
        }
    }
    ~State(){  //Deconstructor
        delete layout;
    }
    int getItem(int x, int y){//Gets item at position x,y
        //This function works but isn't used anymore
        return layout[(x+3*y)];
    }
    int findO(){//Returns position of 0
        int i;
        for (i=0;i<9;i++){
            if (layout[i]==0){return i;}
        }
        if (DEBUG==1){
            //If there is no 0, this will happen
            cerr <<"ERROR:\t"<< "An error in findO has occured. (0 not found)"<<endl<<this->toString()<<endl;
        }
        return -1;//Error, 0 DNE
    }
    int findNum(int num){
        int i;
        for (i=0;i<9;i++){
            if (layout[i]==num){return i;}
        }
        if (DEBUG==1){//If a test file has duplicated numbers, this will spam terminal.
            cerr <<"ERROR:\t"<< "An error in findNum has occured. num "<< num <<" not found" <<endl;
        }
        return -1;//Error, 0 DNE
    }
    int getAt(int num){return layout[num];}
    string validMoves(){//Returns a list of valid moves
        int i = this->findO();
        if (i==0){
            return "UR";
        }
        else if(i==1){
            return "LUR";
        }
        else if(i==2){
            return "LU";
        }
        else if(i==3){
            return "URD";
        }
        else if(i==4){
            return "LRUD";
        }
        else if(i==5){
            return "LDU";
        }
        else if(i==6){
            return "DR";
        }
        else if(i==7){
            return "LDR";
        }
        else if(i==8){
            return "LD";
        }
        else {
            if (DEBUG==1){
                cerr <<"ERROR:\t"<< "An error in ValidMoves has occured."<< i << endl;
            }
            return NULL;
        }
    }
    void doMove(char move){
        int theO = this->findO();
        int notO, temp;
        if (move=='L'){
            notO = theO-1;
        }
        else if (move=='R'){
            notO = theO+1;
        }
        else if (move=='U'){
            notO = theO+3;
        }
        else if (move=='D'){
            notO = theO-3;
        }
        else{
            notO = theO;
            cerr << "The compiler was complaining that it'd be possible to have notO undefined, but it shouldn't be possible..."
                 << " please disregard, I just don't like warnings when I compile" << endl;
        }
        temp = layout[theO];
        layout[theO] = layout[notO];
        layout[notO] = temp;
        return;
    }
    string toString(){//Returns a string representation of the board
        string output="";
        for(int i =6;i<9;i++){
            output.push_back((char) to_string(layout[i])[0]);
            output.push_back((i+1)%3==0?'\n':' ');
        }for(int i =3;i<6;i++){
            output.push_back((char) to_string(layout[i])[0]);
            output.push_back((i+1)%3==0?'\n':' ');
        }for(int i =0;i<3;i++){
            output.push_back((char) to_string(layout[i])[0]);
            output.push_back((i+1)%3==0?'\n':' ');
        }return output;
    }
};
class Problem; //Lets Node::p exist
class Node{ //Node, Similar to the textbooks ideal node variable
    char action;
    int path_cost;
    int cost;
    State * node_state;
    Node * parent;
    Problem * p;
    public:
    //Node constructors are defined just above main due to compilation requirements
    Node(int * layout, Problem * prb, int starter);
    Node(int * layout, Problem * prb);
    Node(Problem *p,char action, int path_cost, State* node_state,Node* parent)// constructor
    :action(action), path_cost(path_cost),node_state(node_state), parent(parent), p(p){};
    Node(Node * old){//Constructor for duplication (no longer in use)
        action = old->action;
        path_cost = old->path_cost;
        cost = old->cost;
        node_state = new State( old->node_state ) ;
        parent = old->parent;
        p = old->p;
    }
    Node(Node * old, char actionn);
    char getAction(){return action;}
    int getCost(){return cost;}
    Node* getParent(){return parent;}
    State* getStatePointer(){return node_state;}
    string validMoves(){return node_state->validMoves();}
};
struct LLNode{ //An LLNode is the linked list that SortedLinkedList is a wrapper for.
    Node * mine;
    LLNode * next;
};
class SortedLinkedList{
    //SortedLinkedList is my implementation of a priority_queue
    LLNode * start;
    public:
    SortedLinkedList(){}
    SortedLinkedList(Node * first){//First node is what instantiates the SLL
        start = new LLNode();
        start->mine = first;
        start->next = NULL;
    }
    void push(Node* neue){ //Enters Node & keeps sorted.
        LLNode * nod = new LLNode();
        nod->mine = neue;
        if( neue->getCost() <= start->mine->getCost() ) {//Checks if need to replace the first item
            nod->next = start;
            start = nod;
            return;
        }//Or another
        LLNode* cursor = start;
        while (cursor->next != NULL){
            if( neue->getCost() > cursor->next->mine->getCost()){
                nod->next = cursor->next;
                cursor->next = nod;
                return;
            }
            cursor = cursor->next;
        }//or become last
        nod->next = NULL;
        cursor->next = nod;
        return;
    }
    Node* pop(){ //Pop a node from the priority_queue
        Node* output = start->mine;
        start = start->next;
        return output;
    }
};
int convertChar(char a){ //For the FileHandler class
    //Converts char to int
    if (a=='0'){
        return 0;
    }
    else if (a=='1'){
        return 1;
    }
    else if (a=='2'){
        return 2;
    }
    else if (a=='3'){
        return 3;
    }
    else if (a=='4'){
        return 4;
    }
    else if (a=='5'){
        return 5;
    }
    else if (a=='6'){
        return 6;
    }
    else if (a=='7'){
        return 7;
    }
    else if (a=='8'){
        return 8;
    }
    else if (a=='9'){
        return 9;
    }
    if (DEBUG==1){
            cerr <<"ERROR:\t"<< "An error in Conversion has occured.\nThe given char was:\""<< a << "\"" <<endl;
    }
    return -1;
}
class FileHandler{ //A file handler for problems
    string filename;
    int * start;
    int * goal;
    public:
    FileHandler(){}
    FileHandler(string fileName){
        filename = fileName;
        ifstream file;
        file.open(fileName);
        string temp = "";
        start = new(int[9]);
        goal = new(int[9]);
        // Loading my arrays like so
        // 6 7 8
        // 3 4 5
        // 0 1 2
        if (file.is_open()){
            getline(file,temp);//024
            start[6] = convertChar(temp[0]);
            start[7] = convertChar(temp[2]);
            start[8] = convertChar(temp[4]);
            getline(file,temp);//024
            start[3] = convertChar(temp[0]);
            start[4] = convertChar(temp[2]);
            start[5] = convertChar(temp[4]);
            getline(file,temp);//024
            start[0] = convertChar(temp[0]);
            start[1] = convertChar(temp[2]);
            start[2] = convertChar(temp[4]);
            getline(file,temp);//This Is A BlankLine
            getline(file,temp);//024
            goal[6] = convertChar(temp[0]);
            goal[7] = convertChar(temp[2]);
            goal[8] = convertChar(temp[4]);
            getline(file,temp);//024
            goal[3] = convertChar(temp[0]);
            goal[4] = convertChar(temp[2]);
            goal[5] = convertChar(temp[4]);
            getline(file,temp);//024
            goal[0] = convertChar(temp[0]);
            goal[1] = convertChar(temp[2]);
            goal[2] = convertChar(temp[4]);
            file.close();
        }
        else{
            cerr << "FILE "<< fileName <<" NOT FOUND"<<endl<<"Exiting..."<<endl;
        }
    }
    void out(string filename, int depth, int total, string solution, string fNvalues){
       ofstream file;
       file.open (filename);
       //1,2,3 Initial
       file << State(start).toString();
       //4 blank
       file << endl;
       //5,6,7 Goal
       file << State(goal).toString();
       //8 blank
       file << endl;
       //9 Goal Depth
       file << depth << endl;
        //10 total nodes generated
        file << total << endl;
        //11 the solution string
        file << solution << endl;
       //12 f(n) values
       file << fNvalues << endl;
        return;
    }
    int* getStart(){return start;}
    int* getGoal() {return  goal;}
};
int abs(int a){//returns the absolute value of a num
    return (a>=0)?a:0-a;
}
int max(int a, int b){//return the max of 2 nums
    return (b<a)?a:b;
}
int getMaxManhattan(int  n1, int  n2){
    //De-Linearized location manhattan distance calculation
    //this can be trivially changed to find the manhattan distances for any two linear locations in any N by N grid
    int temp = n2;
    if (n2<n1){n2=n1;n1=temp;}//make n2 the higher number if it aint.
    int manX=abs(n2%3-n1%3);//x manhattan distance
    int manY=0;//the manhattan distance y distance init
    while (n2%3<2){n2++;}
    while (n1%3<2){n1++;}
    while (n2!=n1){n1+=3; manY++;}//at the end of this loop manY will be Y manhattan distance.
    return max(manY,manX);
}
int getMinManhattan(int  n1, int  n2){
    //De-Linearized location manhattan distance calculation
    //this can be trivially changed to find the manhattan distances for any two linear locations in any N by N grid
    int temp = n2;
    if (n2<n1){n2=n1;n1=temp;}//make n2 the higher number if it aint.
    int manX=abs(n2%3-n1%3);//x manhattan distance
    int manY=0;//the manhattan distance y distance init
    while (n2%3<2){n2++;}
    while (n1%3<2){n1++;}
    while (n2!=n1){n1+=3; manY++;}//at the end of this loop manY will be Y manhattan distance.
    return manY<=manX?manY:manX;
}
class Problem{
    Node * start;
    Node * goal;
    Node * solution;
    int heuristic;
    int totalNodes;
    SortedLinkedList queue;
    FileHandler fh;
    public:
    Problem(){//Default Constructor
        //Not in use, but still here
        //See Problem::Problem(string file, int heur);
        //For what is in use.
        string file = "input.txt";
        solution = NULL;
        heuristic = 0;
        totalNodes = 1;
        fh = FileHandler(file);
        start = new Node( fh.getStart() , this);
        goal  = new Node( fh.getGoal()  , this);
        queue = SortedLinkedList(start);
    }
    Problem(string file, int heur){//Constructor
        fh = FileHandler(file);  
        solution = NULL;
        heuristic =  heur;
        totalNodes = 1;
        goal  = new Node(  fh.getGoal() , this,0);
        start = new Node( fh.getStart() , this);
        
        queue = SortedLinkedList(start);
    }
    Node* getStart(){return start;}
    int heuristicOf(State * a, State * b){
        //Sum of manhattan distances
        int aloc,bloc;
        int big,small;
        int manhattanDist = 0;
        int linearConf = 0;
        for (int i = 1; i<9; i++){ 
            aloc = a->findNum(i);
            bloc = b->findNum(i);
            big   = getMaxManhattan(aloc,bloc);
            small = getMinManhattan(aloc,bloc);
            manhattanDist += big;
            linearConf +=(big==1&&small==0)?1:0;
        }
        if (heuristic == 1)
            return manhattanDist;
        //cout<<linearConf<< " ";
        //double the size of linear conflicts and it'll prefer infinite loops.
        //Heuristic 1 is returned in both instances
        return  0+ manhattanDist;
    }
    int heuristicOf(State * a){
        return heuristicOf(a, goal->getStatePointer());
    }
    Node* solve(){
        Node* current = start;
        Node* pusher;
        if (this->heuristicOf( current->getStatePointer() ) == 0){
            return current;
        } //Edge case, start == goal
        string moves;
        queue.push(start);
        while (1){
            current = queue.pop();//Get next node
            if (current == NULL)
                goto Error;
            moves = current->validMoves();//generate valid moves
            for(int i=0;i<(int)moves.length();i++){
                pusher =( //This is the logic that prevents going left then right again or up and down again, etc.
                        ((moves[i]=='U')&&(current->getAction()=='D'))
                      ||((moves[i]=='D')&&(current->getAction()=='U'))
                      ||((moves[i]=='L')&&(current->getAction()=='R'))
                      ||((moves[i]=='R')&&(current->getAction()=='L')))?
                      NULL:new Node(current,moves[i]);
                if (pusher!=NULL){//add generated nodes to priority_queue
                    totalNodes+=1;
                    if (this->heuristicOf(pusher->getStatePointer())==0){solution = pusher;return pusher;}//If solution, return it
                    queue.push(pusher);
                }
            }
        }
        Error:;
        cerr << "It is mathematically impossible for code to reach here"<<endl;
        return current;//This part of the code will never be reached...
    }
    void outputToFile(string filename){//Outputs solution to file
        filename+=("."+to_string(heuristic)+".txt");
        int solnDepth=0;
        string solnStr="";
        string solStr="";
        string fnPath="";
        Node * cursor = solution;
        while (cursor->getParent() != NULL){
            solnStr+=cursor->getAction();
            solnStr+=" ";
            solnDepth++;
            fnPath=to_string(cursor->getCost()) +" "+ fnPath;
            cursor = cursor->getParent();
        }
        for(int i=solnStr.length()-1; i>0; i--){
            solStr+=solnStr[i-1];//  Un-Reverse solution path 
        }
        
        fh.out(filename, solnDepth, totalNodes, solStr, fnPath);
        cout<<"Filename for heuristic "<<heuristic<<" is: "<<filename<<endl;
    }
};

//Node constructors rely on Problem::heuristicOf(State * a)
//For this reason they've been defined here.
Node::Node(Node * old, char actionn){//Constructor used for all nodes created in Problem::solve();
        action = actionn;
        path_cost = old->path_cost+ 1;
        parent = old;
        p = old->p;
        node_state = new State(old->node_state);
        node_state->doMove(action);
        cost = p->heuristicOf( node_state )+ path_cost;
    }
Node::Node(int * layout, Problem * prb){//Constructor to start with only a layout
        action = NULL;
        path_cost = 0;
        node_state = new State(layout);
        parent = NULL;
        cost = prb->heuristicOf(node_state);
        p = prb;
    }   
Node::Node(int * layout, Problem * prb, int starter){//constructor to instantiate goal 
        action = NULL;
        path_cost = 0;
        node_state = new State(layout);
        parent = NULL;
        cost = starter;
        p = prb;
    }
    
    
    
    
    
int main(int argc, char *argv[]){
    if (argc == 1){
        cout << "Usage: main.exe <filename> <filename> <filename>...\nAt least one filename needed.\nUnsolvable puzzles will stop further puzzles from being solved." << endl;
        return -6;
    }
    for(int i=1;i<=argc;i++){
        Problem heur1(argv[i],1);
        Problem heur2(argv[i],2);
        heur1.solve();
        heur1.outputToFile(argv[i]);
        heur2.solve();
        heur2.outputToFile(argv[i]);
    }
    return 0;
}