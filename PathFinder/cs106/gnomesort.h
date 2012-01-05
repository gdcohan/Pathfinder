/*
 *  gnomesort.h
 *  Assignment5
 *
 *  Created by Gregory Cohan on 12/29/11.
 *  Copyright 2011 Brown University. All rights reserved.
 *
 */

#ifndef _gnomesort_h
#define _gnomesort_h

template <typename Type>
void GnomeSort(Vector<Type> & toSort, int (cmp)(Type, Type) = OperatorCmp);

#endif