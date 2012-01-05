/*
 *  pathfinderextra.cpp
 *  PathFinder
 *
 *  Created by Gregory Cohan on 1/5/12.
 *  Copyright 2012 Brown University. All rights reserved.
 *
 */

#include "pathfinderextra.h"


/* Functions not used in final
 * ---------------------------
 *
 
 cityT ChooseOneRandomCity(Set<cityT> & cities) {
 int randomNum = RandomInteger(0, cities.size() - 1);
 int count = 0;
 Set<cityT>::Iterator it = cities.iterator();
 while (count < randomNum) {
 it.next();
 count += 1;
 }
 return it.next();
 }
 
 cityT ChooseSecondRandomCity(Set<cityT> & cities, cityT otherCity) {
 cityT toReturn;
 while (true) {
 toReturn = ChooseOneRandomCity(cities);
 if (toReturn.name != otherCity.name) break;
 }
 return toReturn;
 }
 
 
 template <typename Type>
 void GnomeSort(Vector<Type> & toSort, int (cmp)(Type, Type) = OperatorCmp) {
 int pos = 1;
 while (pos < toSort.size()) {
 if (cmp(toSort[pos], toSort[pos-1]) >= 0) {
 pos += 1;
 } else {
 Type temp = toSort[pos];
 toSort[pos] = toSort[pos-1];
 toSort[pos-1] = temp;
 if (pos > 1) {
 pos -= 1;
 } else {
 pos += 1;
 }
 }
 }
 }
 
 bool StackContains(Stack<cityT> path, string cityName) {
 for (int i = 0; i < path.size(); i++) {
 if (cityName == path.pop().name) return true;
 }
 return false;
 }
 
 /* Function: GetAllNeighbors
 * -----------------------------------------
 * This function takes the name of a city and
 * returns every edge of the graph that has
 * the city as either its start or end node. 
 * It does so by iterating through the set
 * of arcs.It returns these arcs in a vector.
 */
/*
 Vector<arcT> GetAllNeighbors(string name, Set<arcT> & arcs) {
 Vector<arcT> toReturn;
 Set<arcT>::Iterator it = arcs.iterator();
 while (it.hasNext()) {
 arcT next = it.next();
 if (next.city1 == name || next.city2 == name) toReturn.add(next);
 }
 return toReturn;
 }
 
 */

