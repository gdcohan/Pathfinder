/* pathfinder.cpp
 * ---------------
 * A starter file with some starting constants and handy utility routines.
 */
 
#include "genlib.h"
#include "extgraph.h"
#include <iostream>
#include <cmath>
#include "simpio.h"
#include <fstream>
#include "strutils.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "random.h"
#include "pqueue.h"

/* Constants
 * --------
 * A few program-wide constants concerning the graphical display.
 */
const double CircleRadius =.05;     	// the radius of a node
const int LabelFontSize = 9;          // for node name labels


/* Structures
 * ----------
 */

/* Type: coordT
 * ------------
 * Just a simple struct to handle a pair of x,y coordinate values.
 */
struct coordT {
	double x, y;
};

/* Type: arcT
 * ------------
 * A struct to represent an edge between two nodes. Contains
 * the name of each node as well as the distance between them.
 */
struct arcT {
	string city1;
	string city2;
	int distance;
};

/* Type: cityT
 * ------------
 * Represents a node on the graph as a city. Contains
 * the name and its location as a coordT.
 */

struct cityT {
	coordT location;
	string name;
};

/* Type: pathT
 * ------------
 * A struct to represent a path to a given node for use
 * in Dijkstra's algorithm. Holds the path as a stack and
 * the cumulative distance.
 */

struct pathT {
	double distance;
	Stack<cityT> path;
};


/* Comparison functions to initialize sets
 * --------------------------------------
 */

/* Function: CompareCities
 * -------------------------------------------------------------------
 * Compares two cities, first on their x-coordinates and second on
 * their y-coordinates. Returns 0 if the two cities have the same
 * coordinates.
 */

int CompareCities(cityT city1, cityT city2) {
	if (city1.location.x > city2.location.x) {
		return 1;
	} else if (city1.location.x < city2.location.x) {
		return -1;
	} else if (city1.location.y > city2.location.y) {
		return 1;
	} else if (city1.location.y < city2.location.y) {
		return -1;
	} else {
		return 0;
	}
}

/* Function: CompareArcs
 * -------------------------------------------------------------------
 * Compares two arcs, first on their distance and second on
 * their names. Returns 0 if the arcs are composed of the same
 * two cities.
 */

int CompareArcs(arcT arc1, arcT arc2) {
	if (arc1.distance < arc2.distance) {
		return 1;
	} else if (arc2.distance < arc1.distance) {
		return -1;
	} else if (arc1.city1 == arc2.city1 && arc1.city2 == arc2.city2) {
		return 0;
	} else if (arc1.city1 == arc2.city2 && arc1.city2 == arc2.city1) {
		return 0;
	} else if (arc1.city1 > arc2.city1) {
		return 1;
	} else if (arc1.city1 < arc2.city1) {
		return -1;
	} else if (arc1.city2 > arc2.city2) {
		return 1;
	} else if (arc1.city2 < arc2.city2) {
		return -1;
	} else {
		return 0;
	}
}

/* Function: ComparePaths
 * -------------------------------------------------------------------
 * Compares two paths, based on their distance
 */

int ComparePaths(pathT path1, pathT path2) {
	if (path1.distance < path2.distance) {
		return 1;
	} else if (path1.distance > path2.distance) {
		return -1;
	} else {
		return 0;
	}
}

/* Graphics Helpers
 * ---------------
 */

/* Function: DrawFilledCircleWithLabel
 * Usage:  DrawFilledCircleWithLabel(center, "Green", "You are here");
 * -------------------------------------------------------------------
 * Uses facilities from extgraph to draw a circle filled with
 * color specified. The circle is centered at the given coord has the
 * specified radius.  A label is drawn to the right of the circle.
 * You can leave off the last argument if no label is desired.
 */
void DrawFilledCircleWithLabel(coordT center, string color, string label = "")
{
	MovePen(center.x + CircleRadius, center.y);
	SetPenColor(color);
	StartFilledRegion(1.0);
	DrawArc(CircleRadius, 0, 360);
	EndFilledRegion();
	if (!label.empty()) {
		MovePen(center.x + CircleRadius, center.y);
		SetFont("Helvetica");
		SetPointSize(LabelFontSize);
		DrawTextString(label);
	}
}

/* Function: DrawLineBetween
 * Usage:  DrawLineBetween(coord1, coord2, "Black");
 * -------------------------------------------------
 * Uses facilities from extgraph to draw a line of the
 * specified color between the two given coordinates.
 */
void DrawLineBetween(coordT start, coordT end, string color)
{
	SetPenColor(color);
	MovePen(start.x, start.y);
	DrawLine(end.x - start.x, end.y - start.y);
}


/* Function: GetMouseClick
 * Usage:  loc = GetMouseClick();
 * ------------------------------
 * Waits for the user to click somewhere on the graphics window
 * and returns the coordinate of where the click took place.
 */
coordT GetMouseClick()
{
	coordT where;
	WaitForMouseDown();
	WaitForMouseUp();
	where.x = GetMouseX();
	where.y = GetMouseY();
	return where;
}

/* Auxillary functions for the graph and the data file
 * -----------------------------------------------
 */

/* Function: WithinDistance
 * Usage:  if (WithinDistance(click, pt))...
 * -----------------------------------------
 * Returns true if the Cartesian distance between the two coordinates
 * is <= the specified distance, false otherwise. If not specified,
 * the distance is assumed to be size of the node diameter on screen.
 */
bool WithinDistance(coordT pt1, coordT pt2, double maxDistance = CircleRadius*2)
{
	double dx = pt2.x - pt1.x;
	double dy = pt2.y - pt1.y;
	double distance = sqrt(dx*dx + dy*dy);
	return (distance <= maxDistance);
}

/* Function: GetCoord
 * -----------------------------------------
 * Returns the coordinates of a city identified by its name by
 * searching through the set of cities until it finds one with
 * the correct name
 */

coordT GetCoord(string city, Set<cityT> & cities) {
	Set<cityT>::Iterator it = cities.iterator();
	while (it.hasNext()) {
		cityT next = it.next();
		if (next.name == city) { //if names match, return the coordinates
			return next.location;
		}
	}
}

/* Function: ConvertToDobule
 * -----------------------------------------
 * Takes two integers that together represent a double 
 * and returns that double. The two integer inputs represent
 * the integers to the left and to the right of a decimal point.
 * The function divides the integer to the right of the decimal by
 * 10 until it is less than 1 before combining the two integers
 * into a double. This was necessitated by the format of the input
 * file.
 */

double ConvertToDouble(int leftOfDecimal, int rightOfDecimal) {
	double doubleX = rightOfDecimal;
	while (doubleX >= 1) {
		doubleX = doubleX / 10.0;
	}
	return leftOfDecimal + doubleX;
}

/* Function: GetMatch
 * -----------------------------------------
 * This function takes a coordT and returns a cityT
 * in the graph with the same coordinates. If there is
 * no city with these coordinates, the function returns
 * a cityT with an empty string as the name. This is used
 * when having the user select two cities with a mouse
 * click to run Dijkstra's algorithm on. 
 */

cityT GetMatch(coordT location, Set<cityT> & cities) {
	Set<cityT>::Iterator it = cities.iterator();
	while (it.hasNext()) {
		cityT city = it.next();
		if (WithinDistance(city.location, location)) return city;
	}
	cityT toReturn;
	toReturn.name = "";
	return toReturn;
}

/* Function: GetNameMatch
 * -----------------------------------------
 * This function takes the name of a city and
 * returns the cityT struct with the same name.
 * It does so by iterating through the set of cities.
 */

cityT GetNameMatch(string name, Set<cityT> & cities) {
	Set<cityT>::Iterator it = cities.iterator();
	while (it.hasNext()) {
		cityT city = it.next();
		if (name == city.name) return city;
	}
}


/* Part 1: Drawing the Graph
 * -------------------------
 */

/* Function: GetUserCities
 * -----------------------------------------
 * This function asks the user to click on a city
 * to be used as one of the cities in using
 * Dijkstra's algorithm. The function then returns
 * the city that was clicked on, or prompts the user
 * to click again if it can't find a city at the location
 * where the user clicked.
 */

cityT GetUserCities(Set<cityT> & cities) {
	cityT city1;
	while (true) {
		cout << "Please click on a city: ";
		coordT loc1 = GetMouseClick();
		city1 = GetMatch(loc1, cities);
		if (city1.name != "") break;
		cout << "Did not click close enough. Please try again. ";
	}
	cout << endl << "You've clicked on: " << city1.name << endl;
	return city1;
}
/* Function: DrawGraph
 * -----------------------------------------
 * Reads a text file formatted (for example):
 *
 * USA.bmp
 * NODES
 * WashingtonDC 5.71 2.25
 * Minneapolis 3.76 2.93
 * ARCS
 * Minneapolis WashingtonDC 1600
 *
 * The function takes the data from the file, creating a set
 * of cityT and a set of arcT. It also draws the graph by
 * placing a circle and label at the location of each city.
 */

void DrawGraph(Set<cityT> & cities, Set<arcT> & arcs) {
	InitGraphics();
	ifstream in;
	
	//get file name from user
	while (true) {
		cout << "Please enter the name of the input file: ";
		string FileName = GetLine();
		in.open(FileName.c_str());
		if (!in.fail()) break;
		cout << "Invalid filename. Please try again." << endl;
		in.clear();
	}
	
	//start reading data
	string str;
	in >> str;
	DrawNamedPicture(str);				//draw background image
	in >> str;							//skip "NODES"
	while (true) {
		coordT location;
		cityT city;						//going to add new city to set
		string NameCity;
		in >> NameCity;
		if (NameCity == "ARCS") break;  //Done with cities
		
		//convert string coordinates to doubles
		string xCoordString;			
		in >> xCoordString;
		string yCoordString;
		in >> yCoordString;
		int xCoord1 = StringToInteger(xCoordString.substr(0,1));
		int xCoord2 = StringToInteger(xCoordString.substr(2));
		double xCoord = ConvertToDouble(xCoord1, xCoord2);
		int yCoord1 = StringToInteger(yCoordString.substr(0,1));
		int yCoord2 = StringToInteger(yCoordString.substr(2));
		double yCoord = ConvertToDouble(yCoord1, yCoord2);
		location.x = xCoord;
		location.y = yCoord;
		
		
		city.location = location;								
		city.name = NameCity;									
		cities.add(city);										//add to set
		DrawFilledCircleWithLabel(location, "blue", NameCity);  //update picture
	}
	
	//done with NODES, now onto ARCS
	
	while (in.peek() != EOF) {
		arcT arc;     
		string City1;
		in >> City1;
		string City2;
		in >> City2;
		if (City1 == "" && City2 == "") break;			//was getting weird behavior where it was reading the last line twice
		int milesBetween;								//NEED TO EDIT THIS TO HANDLE NON-INTEGER DISTANCES
		in >> milesBetween;
		arc.city1 = City1;
		arc.city2 = City2;
		arc.distance = milesBetween;
		arcs.add(arc);									//add arc to set
		
		//if you wanted to draw all connections of the map, uncomment this:
		
//		coordT loc1 = GetCoord(City1, cities); 
//		coordT loc2 = GetCoord(City2, cities);
//		DrawLineBetween(loc1, loc2, "red");
	}	
}


/* Part 2: Dijkstra's Algorithm
 * ---------------------------
 */ 


/* Function: GetAllUnseenNeighbors
 * -----------------------------------------
 * This function takes a city, the set of arcs in the graph,
 * the set of cities in the graph, and a set of cities that
 * have already been seen. It then returns all of the neighbors
 * of the given city that have not been visited yet. It returns 
 * these neighbors in the form of a vector of arcT, where each arcT
 * will have the city as one node and its neighbor as the other node.
 * The function works by finding each arc in the graph that has
 * the city as one of the nodes and adding it to the vector if the
 * neighbor has not already been seen.
 */

Vector<arcT> GetAllUnseenNeighbors(cityT city, Set<arcT> & arcs, Set<cityT> & seen, Set<cityT> & cities) {
	Vector<arcT> toReturn;
	Set<arcT>::Iterator it = arcs.iterator();
	while (it.hasNext()) {
		arcT next = it.next();
		if (next.city1 == city.name) {
			cityT pair = GetNameMatch(next.city2, cities);		
			if (!seen.contains(pair)) {						//if the neighbor hasn't been seen, add it to the vector to return
				toReturn.add(next);
			}
		} else if (next.city2 == city.name) {
			cityT pair = GetNameMatch(next.city1, cities);
			if (!seen.contains(pair)) {						//if the neighbor hasn't been seen, add it to the vector to return
				toReturn.add(next);
			}
		}
	}
	return toReturn;
}

/* Function: MakePathsOfNeighbors
 * -----------------------------------------
 * This function takes a path current being examined
 * and a vector of all the unseen neighbors of the current 
 * city (represented in arcTs) and makes updated paths
 * with each of the neighbors. These paths are updated because
 * they will now contain the neighbors, and their distances will
 * be updated to reflect the distance to that city. This distance
 * will be used to prioritize the paths in the priority queue.
 * The function finds the neighbor from each arcT (i.e. the other city)
 * and adds it to the path, updating the distances as it goes.
 */


void MakePathsOfNeighbors(Vector<arcT> neighbors, pathT currPath, Set<cityT> & cities, Vector<pathT> & toReturn) {
	cityT currCity = currPath.path.peek();					 //get the city that is neighbors with all the other cities
	for (int i = 0; i < neighbors.size(); i++) {			 //so that we don't add the same city to the path
		cityT otherCity;
		arcT currArc = neighbors[i];
		if (currArc.city1 == currCity.name) {
			otherCity = GetNameMatch(currArc.city2, cities); //get the other city based on name
		} else if (currArc.city2 == currCity.name) {
			otherCity = GetNameMatch(currArc.city1, cities); //get the other city based on name
		} else {
			Error("shouldn't get here because one city should be a match");
		}
		cout << "other city is: " << otherCity.name << endl;
		pathT newPath;											//make a new path with a new stack of cities
		Stack<cityT> newCities;	
		newCities = currPath.path;
		newCities.push(otherCity);								//add the neighbor to the stack of cities
		double distance = currPath.distance;
		distance += currArc.distance;							//add the distance of the current edge
		newPath.path = newCities;
		newPath.distance = distance;
		toReturn.add(newPath);									//add the new pathT to a vector that will enqueue each pathT
	}
}


/* Function: Dijkstra
 * -----------------------------------------
 * This function takes two cities, a priority queue, 
 * and the set of all edges and nodes in the graph. It
 * then returns a pathT of the shortest path between the 
 * two cities. It does so using Dijkstra's algorithm.
 */

pathT Dijkstra(cityT city1, cityT city2, Set<arcT> & arcs, PQueue<pathT> & pq, Set<cityT> cities) {
	cout << "city1 is : " << city1.name << endl;
	cout << "city2 is : " << city2.name << endl;
	
	//initialize the search
	
	Set<cityT> seen(CompareCities);					//set to make sure we don't get caught in loops
	cityT currCity;
	pathT currPath;
	currCity = city1;								//start at city1
	Stack<cityT> path;
	path.push(currCity);							//initialize a path with city1 and distance of 0
	currPath.path = path;
	currPath.distance = 0;
	seen.add(currCity);
	pq.enqueue(currPath);
	
	//loop until the second city is found or all paths are exhausted
	
	while (!pq.isEmpty()) {
		currPath = pq.dequeueMax();
		currCity = currPath.path.peek();							//current city is the last node in the path so far
		if (currCity.name == city2.name) return currPath;			//found city2, so return
		seen.add(currCity);
		Vector<arcT> neighbors;
		neighbors = GetAllUnseenNeighbors(currCity, arcs, seen, cities);		//get all unseen neighbors
		Vector<pathT> neighborsToEnqueue;
		MakePathsOfNeighbors(neighbors, currPath, cities, neighborsToEnqueue);	//update paths to account for unseen neighbors
		for (int i = 0; i < neighborsToEnqueue.size(); i++) {
			pq.enqueue(neighborsToEnqueue[i]);									//enqueue new paths
		}
	}
	return currPath;
}


/* Part 3: Kruskal's Algorithm
 * ---------------------------
 */ 


/* Function: FindCityInIslandsAndRemove
 * -----------------------------------------
 * This function takes a city and a vector of "islands", 
 * where each island represents cities that are connected.
 * The function then finds the island that the city is in and
 * removes that island from the vector of islands (so it can be
 * modified and put back in). The function returns an empty vector 
 * if the city is not in any island (which means that the island it
 * was in was already taken out). 
 */

Vector<cityT> FindCityInIslandsAndRemove(cityT city, Vector<Vector<cityT> > & islands) {
	for (int i = 0; i < islands.size(); i++) {
		for (int j = 0; j < islands[i].size(); j++) {
			if (city.name == islands[i][j].name) {
				Vector<cityT> toReturn;
				toReturn = islands[i];				//found island, so remove it from the vector of islands and return the island
				islands.removeAt(i);
				return toReturn;
			}
		}
	}
	Vector<cityT> toReturn;
	return toReturn;								//didn't find an island with the city in it, so return empty vector
}


/* Function: Union
 * -----------------------------------------
 * This function takes two vectors of cityT (representing "islands")
 * and unions the two islands. The function takes advantage of the
 * set class, adding each island to a seperate set, unioning the two sets,
 * and then returning the result in the form of a vector.
 *
 * The reason the islands are not initially represented as sets is because
 * I could not figure out a way to have another container class that had sets in 
 * it (i.e. a vector of sets).
 */
Vector<cityT> Union(Vector<cityT> & set1, Vector<cityT> & set2) {
	Set<cityT> RealSet1(CompareCities);				//make Sets to hold contents of vectors
	Set<cityT> RealSet2(CompareCities);
	Vector<cityT> toReturn;
	for (int i = 0; i < set1.size(); i++) {			//add contents of each vector to its corresponding set
		RealSet1.add(set1[i]);
	}
	for (int i = 0; i < set2.size(); i++) {
		RealSet2.add(set2[i]);
	}
	RealSet1.unionWith(RealSet2);					//union sets
	Set<cityT>::Iterator it = RealSet1.iterator();
	while (it.hasNext()) {
		toReturn.add(it.next());					//put contents of unioned set into a vector to return it
	}
	return toReturn;
}
	

/* Function: Kruskal
 * -----------------------------------------
 * This function takes a graph represented by its
 * nodes and edges and a vector of edges to populate.
 * It then populates that vector of edges with the edges
 * needed to span the whole graph with minimal cost.
 * It does so using Kruskal's algorithm.
 */

void Kruskal(Set<cityT> & cities, Set<arcT> & arcs, Vector<arcT> & toReturn) {
	
	//initialize containers
	Vector<cityT> toSee;
	PQueue<arcT> pq(CompareArcs);
	Vector<Vector<cityT> > islands;					//each island is a vector of cityT, and these islands are contained in a vector
	
	//populate toSee, islands
	Set<cityT>::Iterator it = cities.iterator();	//to start, each node is in its own island (alone)
	while (it.hasNext()) { 
		cityT next = it.next();
		Vector<cityT> island;
		island.add(next);
		islands.add(island);
		toSee.add(next);
	}

	//populate priority queue of arcs
	Set<arcT>::Iterator it2 = arcs.iterator();		
	while (it2.hasNext()) { 
		pq.enqueue(it2.next());
	}
	
	//loop until there are enough edges to connect the whole graph
	while (toReturn.size() < toSee.size() - 1) { 
		arcT toExamine = pq.dequeueMax();
		cityT city1 = GetNameMatch(toExamine.city1, cities);					//get each city from the current edge
		cityT city2 = GetNameMatch(toExamine.city2, cities);
		Vector<cityT> setOfCity1 = FindCityInIslandsAndRemove(city1, islands);	//get the island containing each city in the current edge
		Vector<cityT> setOfCity2 = FindCityInIslandsAndRemove(city2, islands);

		//cases:
		
		if (setOfCity1.size() == 1 && setOfCity2.size() == 1) {					//neither city is connected to any other cities
			setOfCity1.add(city2);
			islands.add(setOfCity1);											//put both cities in same island and add current edge to toReturn
			toReturn.add(toExamine);
			
		} else if (setOfCity1.size() == 1) {									//city1 is unconnected, so add it to the island of city2
			setOfCity2.add(city1);
			islands.add(setOfCity2);
			toReturn.add(toExamine);
			
		} else if (setOfCity2.size() == 1) {									//city2 is unconnected, so add it to the island of city1
			setOfCity1.add(city2);
			islands.add(setOfCity1);
			toReturn.add(toExamine);
			
		} else if (setOfCity1.size() == 0 || setOfCity2.size() == 0) {			//the cities are in the same island, so put the island back in the vector and continue
			if (setOfCity1.size() == 0) islands.add(setOfCity2);
			if (setOfCity2.size() == 0) islands.add(setOfCity1);
			continue;
			
		} else {																//the cities are in different islands, so union the two islands
			Vector<cityT> unionedSet = Union(setOfCity1, setOfCity2);
			islands.add(unionedSet);
			toReturn.add(toExamine);
		}
	}
	for (int i = 0; i < toReturn.size(); i++) {									//draw connections on graph
		DrawLineBetween(GetNameMatch(toReturn[i].city1, cities).location, GetNameMatch(toReturn[i].city2, cities).location, "red");
	}
}


/* Part 4: Main
 * ---------------------------
 * The main function initializes the containers and deals with the UI. 
 * It prompts the user to select from four options: 1) choosing a new
 * file to get the graph data from, 2) using dijkstra's algorithm
 * on two cities, 3) using Kruskal's algorithm to find the minimum spanning
 * tree and 4) qutting.
 */ 

int main()
{
	//initialize containers
	
	PQueue<pathT> pq(ComparePaths);
	Set<cityT> cities(CompareCities);
	Set<arcT> arcs(CompareArcs);
	SetWindowTitle("Pathfinder");
	
	//give info to user
    cout << "This program presents a lovely visual presentation of a graph" << endl
        << "along with an implementation of Dijkstra's shortest path algorithm and" << endl
        << "the computation of a minimal spanning tree.  Enjoy!" << endl;
	
	while (true) {
		cout << "Please select an option: " << endl;
		cout << "(1) Choose a new graph data file" << endl;
		cout << "(2) Find shortest path using Dijkstra's algorithm" << endl;
		cout << "(3) Find minimum spanning tree using Kruskal's algorithm" << endl;
		cout << "(4) Quit" << endl;
		cout << "Option: ";
		int option;
		
		//get selection from user
		while (true) {
			option = GetInteger();
			if (option > 0 && option < 5) break;
			cout << "Invalid option. Please try again: " << endl;
		}
		if (option == 1) {							//input data from graph file
			DrawGraph(cities, arcs);
		} else if (option == 2) {					//Dijsktra:
			
			//get cities
			cityT city1 = GetUserCities(cities);	
			cityT city2 = GetUserCities(cities);
			
			//run Dijkstra
			pathT path = Dijkstra(city1, city2, arcs, pq, cities);
			
			//draw path on graph and output the path as text
			cout << "The two cities are " << path.distance << " far apart" << endl;
			cout << "This is the backwards path between them: ";
			if (!path.path.isEmpty()) {
					cityT city = path.path.pop();
				cout << city.name << endl;
					while (!path.path.isEmpty()) {
						cityT city2 = path.path.pop();
						DrawLineBetween(city.location, city2.location, "red");
						city = city2;
						cout << city2.name << endl;
					}
			}
		} else if (option == 3) {				//Kruskal
			Vector<arcT> toReturn;
			Kruskal(cities, arcs, toReturn);
		} else {								//Quit
			break;
		}
	}
    return (0);
}

