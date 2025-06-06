#include <math.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

//I'm lazy
typedef std::pair<long, long> pt;
typedef std::vector<pt> vpt;
typedef std::vector<int> vi;

//Get the distance squared between two points
long distanceSquared(pt one, pt two) {
    return (two.first - one.first) * (two.first - one.first) + (two.second - one.second) * (two.second - one.second); 
}

//Subtracts point two from point one
pt sub(pt one, pt two) {
    return pt(one.first - two.first, one.second - two.second);
}

//2-dimensional cross-product of one with two
int cross(pt one, pt two) {
    return one.first * two.second - one.second * two.first;
}

//Get the direction of pk relative to segment pi, pj
int direction(pt pi, pt pj, pt pk) {
	return cross(sub(pk, pi), sub(pj, pi));
}

//Runs Jarvis' March on points and returns a list of indices in points that make up the convex hull of points
vi jarvisMarch(vpt points) {
    vi hull;
    int hullIdx = 0;

    std::set<int> hullSet;

    int p = 0;
    do {
        hull.push_back(p);
        hullSet.insert(p);
        
        int k = 0;

        long dist = 10000000000000000;

        for (int i = 0; i < points.size(); i++) {
            // determine turn direction from curr->next->Q[i]
            int dir = direction(points[p], points[k], points[i]);

            long d = distanceSquared(points[p], points[i]);

            if (hullSet.find(i) != hullSet.end()) continue;
            
            if (dir > 0) {
                k = i;
                dist = d;
            }
            else if (dir == 0 && d < dist) {
                k = i;
                dist = d;
            }
        }

        p = k;
    } while (p != 0);

    return hull;
}

int main() {
    //Get the number of vertices given
    int verts;
    std::cin >> verts;

    //Create a list of all points in the colony plan
    vpt points;
    for (int i = 0; i < verts; i++) {
        int x, y;
        std::cin >> x >> y;

        points.push_back(pt(x, y));
    }

    //Remove duplicate points before running any convex hull operations
    std::sort(points.begin(), points.end());
    vpt::iterator dedupe_end = std::unique(points.begin(), points.end());
    points.resize(std::distance(points.begin(), dedupe_end));
    
    //Count the number of loops in the colony plan
    int loops = 0;
    int lastLoop = 100000;
    while (points.size() > 2) {
        //Run convex hull on the remaining points
        vi hull = jarvisMarch(points);

        //We need the higher indices first in the array so removal isn't an issue
        std::sort(hull.begin(), hull.end(), std::greater_equal<int>());

        //Delete the outermost loop we just extracted from the remaining points
        // for (int i = 0; i < hull.size(); i++)
        //     points.erase(points.begin() + hull[i]); 

        //Increment loop since we just finished a loop
        loops++;
        
        //If the hull size is more than the last loop, the colony is invalid
        // if (hull.size() >= lastLoop) {
        //     std::cout << "invalid" << std::endl;
        //     return 0;
        // }

        lastLoop = hull.size();
    }

    //If we've found the center of the colony and we don't have two buildings, the colony is invalid
    if (points.size() != 2 || loops == 0) std::cout << "invalid" << std::endl;

    //Otherwise by this point the colony is valid
    else std::cout << "valid " << std::to_string(loops) << std::endl;
}
