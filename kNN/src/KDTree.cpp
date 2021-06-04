#include "KDTree.h"

#include "bits/stdc++.h"
#include "../../../Shared/include/DataSet/StatisticsManager.h"
#include "../../../Shared/include/DataSet/DataSet.h"
#include "../../../Shared/include/DataSet/Converter.h"
#include "Metric.h"

Converter* converter = new Converter();

KDTree::KDTree()
{
    this->marked = false;
    this->visited = false;
    this->feature = "";
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
    this->entry = nullptr;
}

void KDTree::growHelper(DataSet* d, std::string target, int index){
    // Each leaf only has some threshold.
    this->entry = new DataSet(*d);

    if(d->getEntryCount() <= 1){
        this->label = 0;
        this->type = d->getFeatureAt(d->getFeatureIndex(target)).second;
        this->feature = target;
        this->bounds.first = d->getEntryFeatureAt(0, target);
        this->bounds.second = d->getEntryFeatureAt(0, target);
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
        else if(i >= N/2){
            r->addEntry(d->getEntryAt(i));
        }
    }
    this->label = N/2;
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

KDTree* KDTree::traverse(std::map<std::string, std::string> query){
    if(this->left == nullptr || this->right == nullptr){
        if(this->visited == false && this->marked == false){
            return this;
        }
    }

    std::string entry = query[this->feature];
    if(entry == "")
        return nullptr;

    std::string l = this->entry->getEntryFeatureAt(label, feature);

    if(this->type == (int) DataSet::Categorical){
        if(entry <l && this->left != nullptr){
            if(! this->left->visited && ! this->left->marked)
                return this->left->traverse(query);
        }
        else if (entry >= l && this->right != nullptr){
            if(! this->right->visited && ! this->right->marked)
                return this->right->traverse(query);
        }
    }
    else{
        if(stod(entry) < stod(l) && this->left != nullptr){
            if(! this->left->visited && ! this->left->marked)
                return this->left->traverse(query);
        }
        else if(stod(entry) >= stod(l) && this->right != nullptr){
            if(! this->right->visited && ! this->right->marked)
                return this->right->traverse(query);
        }
    }

    if(! this->marked && ! this->visited)
        return this;
    return nullptr;
}

void KDTree::unvisitChildren(){
    this->visited = false;

    if(left != nullptr){
        left->unvisitChildren();
    }
    if(right != nullptr){
        right->unvisitChildren();
    }
}

void KDTree::unmarkChildren(){
    this->marked = false;

    if(left != nullptr){
        left->unmarkChildren();
    }
    if(right != nullptr){
        right->unmarkChildren();
    }
}


KDTree* KDTree::query(std::map<std::string, std::string> query){
    double best = -1;
    std::string answer = "";


    // Descend
    KDTree* node = this->traverse(query);
    KDTree* curr = nullptr;
    Metric* metric = new Metric();

    while(node != nullptr){
        if(node->marked || node->visited)
            break;
        curr = node;
        double distance;
        double boundary;

        std::string l = node->entry->getEntryFeatureAt(label, feature);

        if(node->type == (int) DataSet::Categorical){
            distance = metric->levenshtein(l , query[node->feature]);
            boundary = metric->levenshtein(node->bounds.first, node->bounds.second);
        }
        else{
            distance = metric->euclidean(l , query[node->feature]);
            boundary = std::max(metric->euclidean(node->bounds.first, query[node->feature]),
                                metric->euclidean(node->bounds.second, query[node->feature])
                        );
        }

        node->visited = true;
        if(best == -1 || distance < best){
            answer = l;
            best = distance;
        }

        if(boundary < best || node->parent == nullptr){
            node = node->traverse(query);
        }

        else{
            node = node->parent->traverse(query);
        }
    }
    // Mark the current node
    if(curr != nullptr)
        curr->marked = true;
    return curr;
}

int KDTree::getLabel(){
    return this->label;
}

DataSet* KDTree::getEntries(){
    return this->entry;
}
