#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <typeinfo>
#include <climits>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstdlib>
#include <stdexcept>
#include <fstream>

using namespace std;

unsigned int readRandomFromDevUrandom() {
    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary); // Open /dev/urandom
    if (!urandom) {
        throw std::runtime_error("Failed to open /dev/urandom");
    }
    unsigned int num;
    urandom.read(reinterpret_cast<char*>(&num), sizeof(num)); // Read a number
    if (!urandom) {
        throw std::runtime_error("Failed to read from /dev/urandom");
    }
    urandom.close(); // Close the stream
    return num;
}

class Point{
    private:
        double x_coor;
        double y_coor;
    public:
        Point(double x, double y){
            this->x_coor = x;
            this->y_coor = y;
        }
        double getX(){
            return this->x_coor;
        }
        double getY(){
            return this->y_coor;
        }
};

class Line{
    private:
        Point* src;
        Point* dest;
    public:
        Line(Point* src, Point* dest){
            this->src = src;
            this->dest = dest;
        }
        Point* get_src(){
            return this->src;
        }
        Point* get_dest(){
            return this->dest;
        }
};
class Street{
    private:
        string name;
        vector<Line*> lines_list;
    public:
        Street(string name){
            this->name = name;
        }
        string getName(){
            return this->name;
        }
        void addLine(Line* line){
            this->lines_list.push_back(line);
        }
        vector<Line*> getLine(){
            return this->lines_list;
        }
};
class Graph{
    private:
        vector<Street*> streets_list;
    public:
        void addStreet(Street* street){
            this->streets_list.push_back(street);
        }
        vector<Street*> getStreet(){
            return this->streets_list;
        }
};

bool is_point_on_line(double px, double py, double x1, double y1, double x2, double y2) {
    // Check if the point is within the bounding box of the line segment
    if (px < std::min(x1, x2) || px > std::max(x1, x2) ||
        py < std::min(y1, y2) || py > std::max(y1, y2)) {
        return false; // The point is not within the bounding box
    }

    // Calculate the cross product to check if the point is on the line
    double crossProduct = (py - y1) * (x2 - x1) - (px - x1) * (y2 - y1);

    // Allow a small margin of error due to floating point arithmetic precision
    const double epsilon = 1e-10;
    if ((px == x1 && py == y1)|| (px == x2 && py == y2)){
        return false;
    }
    return std::abs(crossProduct) < epsilon;
}

bool check_intersect(Line* line1, Line* line2, bool last_line){
    double x1 = line1->get_src()->getX();
    double y1 = line1->get_src()->getY();
    double x2 = line1->get_dest()->getX();
    double y2 = line1->get_dest()->getY();
    double x3 = line2->get_src()->getX();
    double y3 = line2->get_src()->getY();
    double x4 = line2->get_dest()->getX();
    double y4 = line2->get_dest()->getY();
    double xnum; double xden; double xcoor;
    double ynum; double yden; double ycoor;
    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4));
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4));
    if(xden == 0){
        return true;
    }
    xcoor =  xnum / xden;
    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4);
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    if(yden == 0){
        return true;
    }
    ycoor = ynum / yden;
    // // Ensure xcoor is within the bounds of both lines' x coordinates
    // if ((  (xcoor <= std::min(x1, x2) && x1!=x2)  ||   ((xcoor >= std::max(x1, x2)) && x1!=x2)) ||
    //     ((xcoor <= std::min(x3, x4 )&& x3!=x4) || (xcoor >= std::max(x3, x4)&& x1!=x2))) {
    //     return false; // The x-coordinate of intersection is not within the x bounds of both lines
    // }

    // // Ensure ycoor is within the bounds of both lines' y coordinates
    // if (((ycoor <= std::min(y1, y2) && y1!=y2)|| (ycoor >= std::max(y1, y2) && y1!=y2))||
    //     ((ycoor <= std::min(y3, y4) && y3!=y4)|| (ycoor >= std::max(y3, y4) && y3!=y4))){
    //     return false; // The y-coordinate of intersection is not within the y bounds of both lines
    // }
    if(last_line){
    // Ensure xcoor is within the bounds of both lines' x coordinates
        if ( (  (xcoor <= std::min(x1, x2) || xcoor >= std::max(x1, x2)) && (ycoor <= std::min(y1, y2) || ycoor >= std::max(y1, y2)) )
            ||  ((xcoor <= std::min(x3, x4) || xcoor >= std::max(x3, x4)) && (ycoor <= std::min(y3, y4) || ycoor >= std::max(y3, y4)))  ) {
                if ((xcoor == x1 && ycoor == y1) || (xcoor == x2 && ycoor == y2)){  
                    if(is_point_on_line(xcoor,ycoor,x3,y3,x4,y4)){
                        return true;
                    }
                }
                else if((xcoor == x3 && ycoor == y3) || (xcoor == x4 && ycoor == y4)){  
                    if(is_point_on_line(xcoor,ycoor,x1,y1,x2,y2)){
                        return true;
                    }
                }
            return false; // The x-coordinate of intersection is not within the x bounds of both lines
        }
    }
    else{
        if ( (  (xcoor < std::min(x1, x2) || xcoor > std::max(x1, x2)) && (ycoor < std::min(y1, y2) || ycoor > std::max(y1, y2)) )
            ||  ((xcoor < std::min(x3, x4) || xcoor > std::max(x3, x4)) && (ycoor < std::min(y3, y4) || ycoor > std::max(y3, y4)))  ) {
                if ((xcoor == x1 && ycoor == y1) || (xcoor == x2 && ycoor == y2)){  
                    if(is_point_on_line(xcoor,ycoor,x3,y3,x4,y4)){
                        return true;
                    }
                }
                else if((xcoor == x3 && ycoor == y3) || (xcoor == x4 && ycoor == y4)){  
                    if(is_point_on_line(xcoor,ycoor,x1,y1,x2,y2)){
                        return true;
                    }
                }
            return false; // The x-coordinate of intersection is not within the x bounds of both lines
        }
    }
    // // Ensure ycoor is within the bounds of both lines' y coordinates
    // if ((ycoor <= std::min(y1, y2) || ycoor >= std::max(y1, y2)) ||
    //     (ycoor <= std::min(y3, y4) || ycoor >= std::max(y3, y4))) {
    //     return false; // The y-coordinate of intersection is not within the y bounds of both lines
    // }



    return true;
    
}

void create_street(int s_num, int n_num, int c_num, int l_num){
    // Create a new graph
    Graph* street_graph = new Graph();
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));
    // List of possible words to use in street names
    vector<string> words = {
        "Green", "Oak", "Pine", "Maple", "Cedar", "Victoria", "Corrine", "Queen",
        "Elm", "Willow", "Peach", "Third", "First", "Stroud", "Red", "King",
        "Lake", "Hill", "River", "Sunset", "Dale", "Mcknight", "University"
    };
    // List of possible street types
    vector<string> streetTypes = {
        "St", "Ave", "Blvd", "Rd", "Ln", "Way", "Dr", "Cres", "Cir"
    };
    vector<string> streetdirection = {
        "W", "S", "E", "N", "North", "South", "East", "West"
    };
    // Generate Random number of street that we need to generate
    int num_street = 0;
    int sleep_time = 0;
    while (num_street < 2){num_street = readRandomFromDevUrandom()% (s_num +1);}
    while (sleep_time < 5){sleep_time = readRandomFromDevUrandom() % (l_num+1);}
    for (int i=0; i<num_street; i++){
        // Generate a random index for words and streetTypes
        int randomWordIndex =readRandomFromDevUrandom() % words.size();
        int randomWordIndex1 = readRandomFromDevUrandom() % words.size();
        int randomStreetTypeIndex = readRandomFromDevUrandom() % streetTypes.size();
        int randomStreetDirection = readRandomFromDevUrandom()% streetdirection.size();
        // Generate the random street name
        string streetName = '"'+ words[randomWordIndex] + " " + words[randomWordIndex1] + " " + streetTypes[randomStreetTypeIndex] + " " + streetdirection[randomStreetDirection] + '"';
        //cout << streetName << endl;
        // generate random number of line and point
        int num_line = 0;
        while (num_line < 1){num_line = readRandomFromDevUrandom() % (n_num+1);}
        // point is line + 1
        int num_point = num_line + 1;
        // create a new street
        Street* street;
        Point* src;
        Point* dest;
        street = new Street(streetName);
        for(int j =0; j<num_point;j++)
        {
            int limit = 0;
            //limit++;
            int a,b;
            int c,d;
            Line* line;
            //cout << "hi" << endl;
            if(j==0)
            {
                a =(readRandomFromDevUrandom()-readRandomFromDevUrandom())%(c_num+1);
                b =(readRandomFromDevUrandom()-readRandomFromDevUrandom())%(c_num+1);
                //cout<<"("<<a<<","<<b<<") "<<endl;
                src = new Point(a,b);
            }
            bool check = false;
            while(!check){
                limit++;
                check = true;
                c =(readRandomFromDevUrandom()-readRandomFromDevUrandom())%(c_num+1);
                d =(readRandomFromDevUrandom()-readRandomFromDevUrandom())%(c_num+1);
                dest = new Point(c,d);
                line = new Line(src,dest);
                // not the starting point
                if (j>0){
                    vector<Line*> line_list = street->getLine();
                    int line_list_length = line_list.size();
                    int buff_line = 0;
                    bool last_line = false;
                    // cannot intersection with the previous line except for the src or dest point
                    for(Line* temp_line : line_list){
                        buff_line++;
                        if (buff_line == line_list_length){
                            last_line = true;
                        }
                        // cout << "checking: (" << temp_line->get_src()->getX() << "," << temp_line->get_src()->getY() << ")" << "->";
                        // cout << "(" << temp_line->get_dest()->getX() << "," << temp_line->get_dest()->getY() << ")" << endl;
                        // cout << "And: (" << line->get_src()->getX() << "," << line->get_src()->getY() << ")" << "->";
                        // cout << "(" << line->get_dest()->getX() << "," << line->get_dest()->getY() << ")" << endl;
                        // if (check_intersect(temp_line, line)){
                        //     cout << "check result:" << "True" << endl;
                        // }
                        // else{
                        //     cout << "check result: False" << endl;
                        // }
                        // //if they are not intersection and not length equal zero then output
                        // cout << to_string(limit) << endl;
                        if (!check_intersect(temp_line, line, last_line) && ((a-c)!=0 || (b-d) != 0)){
                            check = true;
                            //cout << "total result: True and add line" << endl;
                            if (limit > 1000){
                                cerr << "Error: failed to generate valid input for 200 simultaneous attempts." << endl;
                                cout << "Exits" << endl;
                                exit(0);
                            } 
                        }
                        else{
                            check = false;
                            //cout << "total result: False" << endl;
                            if (limit > 1000){
                                cerr << "Error: failed to generate valid input for 200 simultaneous attempts." << endl;
                                cout << "Exits" << endl;
                                exit(0);
                            } 
                            break;
                        }
                    }
                }
                  
            }
            // reset the src as the new dest
            street->addLine(line);
            // cout << "adding line:" << "(" << line->get_src()->getX() << "," << line->get_src()->getY() << ")" << "->";
            // cout << "(" << line->get_dest()->getX() << "," << line->get_dest()->getY() << ")" << endl;
            src = dest;
        }
        //while(i==num_street-1 && (!check_two_street(st,st_database)) ){
        street_graph->addStreet(street);
        //}
        cout << "add " << street->getName() << " ";
        vector <Line*> line_list = street->getLine();
        for (int j = 0; j < line_list.size(); j++){
            cout << '(' << line_list[j]->get_src()->getX() << ',' << line_list[j]->get_src()->getY() << ')'<< " ";
        }
        cout << endl;
    }
    // generate graph
    cout << "gg" << endl;
    // cout << endl;
    sleep(sleep_time);
    // removing all street
    vector <Street*> street_list = street_graph->getStreet();
    for (int j = 0; j < street_list.size(); j++){
        string name = street_list[j]->getName();
        cout << "rm " << name  << endl;
    }
}


int main(int argc, char** argv) {
    char opt;
    int s_num=10;
    int n_num=5;
    int l_num=5;
    int c_num=20;
    while ((opt = getopt (argc, argv, "s:n:l:c:")) != -1){
        switch(opt){
            case 's':
                s_num = atoi(optarg);
                if(s_num < 2){
                    cerr << "Error: -s is less than 2." << endl;
                    exit(0);
                } 
                break;
            case 'n':
                n_num = atoi(optarg);
                if(n_num < 1){
                    cerr << "Error: -n is less than 1." << endl;
                    exit(0);
                } 
                break;
            case 'l':
                l_num = atoi(optarg);
                if(l_num < 5){
                    cerr << "Error: -l is less than 5." << endl;
                    exit(0);
                } 
                break;
            case 'c':
                c_num = atoi(optarg);
                if(c_num < 1){
                    cerr << "Error: -c is less than 1." << endl;
                    exit(0);
                } 
                break;
        }
    }
    //cout << s_num << n_num << l_num << c_num<<endl;
    while(true){
        create_street(s_num, n_num, c_num, l_num);
        
    }
}