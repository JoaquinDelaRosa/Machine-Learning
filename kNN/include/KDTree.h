#ifndef KDTREE_H
#define KDTREE_H

#include <bits/stdc++.h>
#include <Dataset/DataSet.h>
#include <Dataset/StatisticsManager.h>

class KDTree
{
    private:
        std::string feature;
        std::string label;
        int type;

        // Children
        KDTree* left;
        KDTree* right;
        KDTree* parent;

        // Bounds
        std::pair<std::string, std::string> bounds;

        void growHelper(DataSet* d, std::string target, int index);
        KDTree* traverse(KDTree* tree, std::map<std::string, std::string> query);

    public:
        KDTree();
        void grow(DataSet* d, std::string target);
        std::string query(std::map<std::string, std::string> query);
};

#endif // KDTREE_H
