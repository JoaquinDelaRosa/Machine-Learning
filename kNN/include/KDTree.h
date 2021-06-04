#ifndef KDTREE_H
#define KDTREE_H

#include <bits/stdc++.h>
#include <../../../Shared/include/DataSet/DataSet.h>
#include <../../../Shared/include/DataSet/StatisticsManager.h>

class KDTree
{
    private:
        std::string feature;
        int label;
        DataSet* entry;

        int type;

        // Children
        KDTree* left;
        KDTree* right;
        KDTree* parent;

        // Bounds
        std::pair<std::string, std::string> bounds;

        // Visiting
        bool marked;
        bool visited;

        void growHelper(DataSet* d, std::string target, int index);
        KDTree* traverse(std::map<std::string, std::string> query);

    public:
        KDTree();
        void grow(DataSet* d, std::string target);
        KDTree* query(std::map<std::string, std::string> query);

        void unmarkChildren();
        void unvisitChildren();
        int getLabel();

        DataSet* getEntries();
};

#endif // KDTREE_H
