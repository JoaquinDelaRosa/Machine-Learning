#include "KDTree.h"

#include "bits/stdc++.h"
#include "Dataset/DataSet.h"
#include "Dataset/StatisticsManager.h"
#include "Metric.h"

KDTree::KDTree()
{
    //ctor
}

void KDTree::growHelper(DataSet* d, std::string target, int index){
    if(d->getEntryCount() <= 0){
        return;
    }

    // Each leaf only has some threshold.
    if(d->getEntryCount() == 1){
        this->label = d->getEntryFeatureAt(0, target);
        return;
    }

    // Handle index
    if(d->getFeatureAt(index).first == target)
        index = (index + 1) % d->getFeatureCount();


    this->feature = d->getFeatureAt(index).first;
    this->type = d->getFeatureAt(index).second;

    // Make children
    this->left = new KDTree();
    this->right = new KDTree();
    left->parent = this;
    right->parent = this;

    // Make the partitions
    DataSet* l = new DataSet(*d, true);
    DataSet* r = new DataSet(*d, true);

    d->sortByFeature(this->feature);
    int N = d->getEntryCount();

    for(int i = 0; i < N; i++){
        if(i < N/2){
            l->addEntry(d->getEntryAt(i));
        }
        else{
            r->addEntry(d->getEntryAt(i));
        }
    }
    this->label = d->getEntryFeatureAt(N/2, this->feature);
    this->bounds.first = d->getEntryFeatureAt(0, this->feature);
    this->bounds.second = d->getEntryFeatureAt(N - 1, this->feature);

    // Grow Children
    index = (index + 1) % d->getFeatureCount();
    left->growHelper(l, target, index);
    right->growHelper(r, target, index);
}


void KDTree::grow(DataSet* d, std::string target){
    growHelper(d, target, 0);
}

KDTree* KDTree::traverse(KDTree* tree, std::map<std::string, std::string> query){
    if(tree->left == nullptr && tree->right == nullptr){
        return tree;
    }
    std::string entry = query[tree->feature];
    if(entry == "")
        return nullptr;

    if(tree->type == (int) DataSet::Categorical){
        if(entry < tree->label)
            return traverse(tree->left, query);
        else{
            return traverse(tree->right, query);
        }
    }
    else{
        if(stod(entry) < stod(tree->label))
            return traverse(tree->left, query);
        else
            return traverse(tree->right, query);
    }
}

std::string KDTree::query(std::map<std::string, std::string> query){
    double best = -1;
    std::string answer = "";

    // Descend
    KDTree* node = traverse(this, query);

    Metric* metric = new Metric();

    while(node != nullptr){
        double distance;
        double boundary;

        if(node->type == (int) DataSet::Categorical){
            distance = metric->levenshtein(node->label , query[node->feature]);
            boundary = metric->levenshtein(node->bounds.first, node->bounds.second);
        }
        else{
            distance = metric->euclidean(stod(node->label), stod(query[node->feature]));
            boundary = metric->euclidean(stod(node->bounds.first), stod(node->bounds.second));
        }

        if(best == -1 || distance < best){
            answer = node->label;
            best = distance;
        }
        else if(boundary < best){
            node = traverse(node, query);
        }

        else
            node = node->parent;
    }

    return answer;
}
