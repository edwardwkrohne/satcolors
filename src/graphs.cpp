// April 2014

// TODO: This file is a bit of a dumping ground for code that stacked
// up in Solve.cpp recently.  I just don't need it for the moment.
// It's just different kinds of graphs (in incidence matrix form) that
// we can put relations on.
//
// At a minimum, it needs a matching header file.

#include <iostream>
#include "grid.h"
using Minisat::Lit;
using Minisat::Var;
using Minisat::mkLit;
using namespace std;

/////////////////////////////////////////////////////////////////////
//
//  A graph based on a given incidence matrix, specified in a stream
//
//  This class is inefficient except for some possible (bizarre) edge
//  cases.  I wrote it before I realized this, so it's not
//  well-tested.  The inefficiency comes from the fact that it creates
//  a large number of literals and specifies the values exactly; the
//  only way to use this information is to imply these literals are
//  true or false in certain scenarios, which will create a (probably)
//  ridiculous amount of unnecessary unit propagation.
//
class StreamGraph: public Grid<Lit> {
public:
  StreamGraph() = delete;
  StreamGraph(SolverManager* manager, istream& in, Var& startingVar = SolverManager::allocateNew);

  int order() const;

private:
  // Helper method to make it easier to compute the grid inside of the
  // constructor initialier list.
  static Grid createGrid(SolverManager* manager, istream& in, Var& startingVar);
};

Grid<Lit> StreamGraph::createGrid(SolverManager* manager, istream& in, Var& startingVar) {
  value_type order;
  in >> order;
  return Grid(order, order,
	      [&](int src, int dst) {
		Lit lit;
		if ( startingVar == SolverManager::allocateNew) {
		  lit = mkLit(manager->newVars(1));
		} else {
		  lit = mkLit(startingVar++);
		}

		if ( !in ) {
		  throw std::runtime_error("Error reading incidence matrix from stream. "
					   "Corrupted file? "
					   "Incorrect dimension specification? "
					   "Non-integer values?");
		}
		int elem;
		in >> elem;

		auto req = (elem == 0) ? ~lit : lit;
		manager->require(req);
		return lit;
	      });
}

StreamGraph::StreamGraph(SolverManager* manager, istream& in, Var& startingVar) :
  Grid(createGrid(manager, in, startingVar))
{
  ; // This function has no body, just an initializer list.
}

int StreamGraph::order() const {
  return height();
}

/////////////////////////////////////////////////////////////////////
//
//  A complete graph (K_n)
//
class CompleteGraph: public Grid<Lit> {
public:
  CompleteGraph() = delete;
  CompleteGraph(SolverManager* manager, int order, Var& startingVar = SolverManager::allocateNew);

  int order() const;
};

CompleteGraph::CompleteGraph(SolverManager* manager, int order, Var& startingVar) :
  Grid(order, order, 
       [&](int src, int dst) {
	 Lit lit;
	 if ( startingVar == SolverManager::allocateNew) {
	   lit = mkLit(manager->newVars(1));
	 } else {
	   lit = mkLit(startingVar++);
	 }
	 auto req = (src == dst) ? ~lit : lit;
	 manager->require(req);
	 return lit;
       })
{
  ; // This function has no body, just an initializer list.
}

int CompleteGraph::order() const {
  return height();
}

/////////////////////////////////////////////////////////////////////
//
//  The twisted torus graph
//
class TwistedTorusGraph: public Grid<Lit> {
public:
  TwistedTorusGraph() = delete;
  TwistedTorusGraph(SolverManager* manager, int order, Var& startingVar = SolverManager::allocateNew);

  int order() const;
};

TwistedTorusGraph::TwistedTorusGraph(SolverManager* manager, int length, Var& startingVar) :
  Grid(3*length, 3*length,
       [&](int src, int dst) {
	 // Symmetric matrix, so WLOG, src < dst.
	 if ( src > dst ) {
	   swap(src, dst);
	 }

	 // Note: these force srcCol <= dstCol with the swap above
	 int srcRow = src % 3;
	 int srcCol = src / 3;
	 int dstRow = dst % 3;
	 int dstCol = dst / 3;
	 
	 bool includeLink;
	 
	 // Case 1: horizontal links in the final column
	 if ( srcCol == 0 && dstCol == length -1 ) {
	   includeLink = 
	     (srcRow == 0 && dstRow == 1) ||
	     (srcRow == 1 && dstRow == 0) ||
	     (srcRow == 2 && dstRow == 2);
	 }
	 
	 // Case 2: horizontal links in other columns
	 else if (srcCol+1 == dstCol) {
	   includeLink = ( srcRow == dstRow );
	 }
	 
	 // Case 3: vertical links in other columns
	 else if (srcCol == dstCol) {
	   includeLink = srcRow != dstRow;
	 }
	 
	 // Case 4: all other links
	 else {
	   includeLink = false;
	 }
	 
	 // Fill in the corresponding part of the matrix
	 Lit lit;
	 if ( startingVar == SolverManager::allocateNew) {
	   lit = mkLit(manager->newVars(1));
	 } else {
	   lit = mkLit(startingVar++);
	 }

	 manager->require(includeLink ? lit : ~lit);
	 return lit;
       })
{
  ; // This function has no body, just an initializer list.
}

int TwistedTorusGraph::order() const {
  return height();
}

/////////////////////////////////////////////////////////////////////
//
//  The orientation reversal graph
//
class OrientationReversalGraph: public Grid<Lit> {
public:
  OrientationReversalGraph() = delete;
  OrientationReversalGraph(SolverManager* manager, int width, int height, Var& startingVar = SolverManager::allocateNew);

  int order() const;
};

// We seek a particular kind of coloring of a grid of nodes into a
// target graph gamma.  That is, we want gamma to include an odd cycle, and we
// want it to be possible to transform gamma into the reverse odd
// cycle by adding four-cycles.  Additionally, some node must be
// preserved at every stage.
//
// This condition can be formalized by saying that we want rows of
// elements of gamma such that there is a color U with 
//
// * Each row has even length and starts and ends with U
//
// * Rows may have different lengths, but the first and last node of
//   the nth row must be diagonally adjacent to the first and last
//   nodes of the (n+1)th row respectively
//  
// It is difficult to express this condition as a graph homomorphism,
// so care must be taken.  We take the graph as a rectangular grid of
// nodes, but with carefully arranged edges.  This rectangular grid is
// to be extended to an example of the condition above as in the
// following examples:
//
//     +--------+
//     | 0  1  2| 0
//    0| 3  4  5| 2 0 
//     +--------+
//       0  2  1
//  
//
//     +--------------+
//     | 0  1  2  3  4| 0
//    0| 5  6  7  8  9| 4 0 
// 0  5|10 11 12 13 14| 0
//    0|15 16 17 18 19|14 0
// 0 15|20 21 22 23 24| 0
//    0|25 26 27 28 29|24 0
//     +--------------+
//       0  4  3  2  1  0
//
// To be able to achieve this pattern, it is necessary and sufficient
// that the nodes in the box have links to nodes they are adjacent to
// in the pattern shown.
//
// For the first example, this leads to the adjacency matrix
//
//    0 1 2 3 4 5
//   +-----------
//  0|0 1 1 1 0 0
//  1|1 0 1 0 1 1
//  2|1 1 0 0 1 1
//  3|1 0 0 0 1 0
//  4|0 1 1 1 0 1
//  5|0 1 1 0 1 0
//  

OrientationReversalGraph::OrientationReversalGraph(SolverManager* manager, int cycleLength, int stages, Var& startingVar) :
  // Set up a long, complicated lambda function as one of the arguments for the Grid initialization
  Grid(cycleLength*stages, cycleLength*stages,
       [&](int src, int dst) {
	 // The cycle length must be odd, and the number of stages must be even
	 if ( !(cycleLength % 2 == 1 && stages % 2 == 0) ) {
	   std::ostringstream sout;
	   sout << "Invalid cycle length " << cycleLength << " or number of stages " << stages << " for an odd cycle orientation reversal.";
	   throw std::invalid_argument(sout.str());
	 }

	 // Symmetric matrix, so WLOG, src <= dst.
	 if ( src > dst ) {
	   swap(src, dst);
	 }

	 int srcRow = src / cycleLength;
	 int srcCol = src % cycleLength;
	 int dstRow = dst / cycleLength;
	 int dstCol = dst % cycleLength;
	 
	 bool includeLink = false;

	 // 
	 // Giant if tree.
	 // 
	 // Because of the swap above and the way srcRow and dstRow
	 // are defined, we will always have srcRow <= dstRow, and if
	 // equality holds, then srcCol <= dstCol.
	 //

	 // Case 1: links involving upper left corner 
	 if (srcRow == 0 && srcCol == 0) {
	   // Case 1a: The upper-left-corner node links to the node
	   // to its right
	   if ( dstRow == 0 && dstCol == 1 ) {
	     includeLink = true;
	   }
	   
	   // Case 1b: The upper-left-corner node links to the node on
	   // the left side, on odd-numbered rows
	   else if ( dstCol == 0 && dstRow % 2 == 1 ) {
	     includeLink = true;
	   }

	   // Case 1c: The upper-left-corner node links to the node on
	   // the right side, on even-numbered rows
	   else if ( dstCol == cycleLength-1 && dstRow % 2 == 0 ) {
	     includeLink = true;
	   }

	   // The upper-left-corner node links to no other nodes
	   else {
	     includeLink = false;
	   }
	 }

	 // Case 2: horizontal links that have not already been
	 // handled by case 1
	 else if ( srcRow == dstRow ) {
	   includeLink = ( srcCol+1 == dstCol );
	 }

	 // Case 3: all other links (i.e., just vertical links)
	 else {
	   // Case 3a: Vertical links from top row to bottom row.
	   // Involves a reversal.
	   if ( srcRow == 0 && dstRow == stages-1 ) {
	     includeLink = ( srcCol == (cycleLength-dstCol)%cycleLength );
	   }

	   // Case 3b: All other vertical links from top row to bottom
	   // row. These cases are not exclusive (they won't be if
	   // stages==2), hence the lack of "else" and use of "|="
	   if ( srcRow+1 == dstRow ) {
	     includeLink |= ( srcCol == dstCol );
	   }
	 }
	 // End of if tree ---------------------------------------------------


	 // Fill in the corresponding part of the matrix
	 Lit lit;
	 if ( startingVar == SolverManager::allocateNew) {
	   lit = mkLit(manager->newVars(1));
	 } else {
	   lit = mkLit(startingVar++);
	 }

	 manager->require(includeLink ? lit : ~lit);
	 return lit;
       })
{
  ; // This constructor has no body, just an initializer list with a long lambda function.
}

int OrientationReversalGraph::order() const {
  return height();
}

template<class T>
void printGraph(SolverManager* manager, const T& graph) {
  cout << "   ";
  for ( int i = 0; i < graph.order(); i++ ) {
    cout << (i%10) << " ";
  }
  cout << endl << endl;
  for ( int i = 0; i < graph.order(); i++ ) {
    cout << (i%10) << "  ";
    for ( int j = 0; j < graph.order(); j++ ) {
      cout << manager->modelValue(var(graph[i][j])) << " ";
    }
    cout << endl;
  }
  cout << endl << endl;
}
